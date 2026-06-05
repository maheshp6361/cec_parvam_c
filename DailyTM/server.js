const path = require('path');
const fs = require('fs/promises');
const express = require('express');
const session = require('express-session');
const bcrypt = require('bcryptjs');
const helmet = require('helmet');
const cors = require('cors');
const multer = require('multer');
const { v4: uuid } = require('uuid');

const app = express();
const PORT = process.env.PORT || 3000;
const SESSION_SECRET = process.env.SESSION_SECRET || 'change-this-secret-in-production';

const root = __dirname;
const dbDir = path.join(root, 'database');
const publicDir = path.join(root, 'public');
const uploadDir = path.join(publicDir, 'assets', 'uploads');

const files = {
  users: path.join(dbDir, 'users.json'),
  admins: path.join(dbDir, 'admins.json'),
  tasks: path.join(dbDir, 'tasks.json'),
  reminders: path.join(dbDir, 'reminders.json'),
  activityLogs: path.join(dbDir, 'activityLogs.json')
};

const upload = multer({
  dest: uploadDir,
  limits: { fileSize: 2 * 1024 * 1024 },
  fileFilter: (req, file, cb) => {
    if (!file.mimetype.startsWith('image/')) return cb(new Error('Only image uploads are allowed.'));
    cb(null, true);
  }
});

app.use(helmet({
  contentSecurityPolicy: {
    directives: {
      defaultSrc: ["'self'"],
      scriptSrc: ["'self'", 'https://cdn.jsdelivr.net', 'https://unpkg.com'],
      styleSrc: ["'self'", "'unsafe-inline'", 'https://fonts.googleapis.com'],
      fontSrc: ["'self'", 'https://fonts.gstatic.com'],
      imgSrc: ["'self'", 'data:', 'https://images.unsplash.com'],
      connectSrc: ["'self'"]
    }
  }
}));
app.use(cors({ origin: true, credentials: true }));
app.use(express.json({ limit: '2mb' }));
app.use(express.urlencoded({ extended: true }));
app.use(session({
  name: 'dtm.sid',
  secret: SESSION_SECRET,
  resave: false,
  saveUninitialized: false,
  cookie: {
    httpOnly: true,
    sameSite: 'lax',
    secure: false,
    maxAge: 1000 * 60 * 60 * 8
  }
}));
app.use(express.static(publicDir));

async function ensureDatabase() {
  await fs.mkdir(dbDir, { recursive: true });
  await fs.mkdir(uploadDir, { recursive: true });
  for (const file of Object.values(files)) {
    try {
      await fs.access(file);
    } catch {
      await fs.writeFile(file, '[]\n');
    }
  }
}

async function readJson(name) {
  const raw = await fs.readFile(files[name], 'utf8');
  return raw.trim() ? JSON.parse(raw) : [];
}

async function writeJson(name, data) {
  await fs.writeFile(files[name], `${JSON.stringify(data, null, 2)}\n`);
}

function publicUser(user) {
  if (!user) return null;
  const { passwordHash, ...safe } = user;
  return safe;
}

function requireUser(req, res, next) {
  if (req.session.user?.role === 'user') return next();
  return res.status(401).json({ message: 'Please log in.' });
}

function requireAdmin(req, res, next) {
  if (req.session.user?.role === 'admin') return next();
  return res.status(403).json({ message: 'Admin access required.' });
}

function validateEmail(email) {
  return /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(String(email).trim());
}

function validatePassword(password) {
  return typeof password === 'string' && password.length >= 8;
}

async function logActivity(actorId, actorRole, action, details = '') {
  const logs = await readJson('activityLogs');
  logs.unshift({
    id: uuid(),
    actorId,
    actorRole,
    action,
    details,
    createdAt: new Date().toISOString()
  });
  await writeJson('activityLogs', logs.slice(0, 500));
}

function taskMatchesOwner(task, req) {
  return task.userId === req.session.user.id;
}

app.get('/', (req, res) => {
  res.redirect(req.session.user ? '/pages/dashboard.html' : '/pages/login.html');
});

app.get('/api/session', (req, res) => {
  res.json({ user: req.session.user || null });
});

app.post('/api/auth/register', async (req, res, next) => {
  try {
    const { fullName, email, username, password, confirmPassword } = req.body;
    if (!fullName || !email || !username || !password || !confirmPassword) {
      return res.status(400).json({ message: 'All fields are required.' });
    }
    if (!validateEmail(email)) return res.status(400).json({ message: 'Enter a valid email address.' });
    if (!validatePassword(password)) return res.status(400).json({ message: 'Password must be at least 8 characters.' });
    if (password !== confirmPassword) return res.status(400).json({ message: 'Passwords do not match.' });

    const users = await readJson('users');
    const admins = await readJson('admins');
    const emailTaken = [...users, ...admins].some((u) => u.email.toLowerCase() === email.toLowerCase());
    const usernameTaken = [...users, ...admins].some((u) => u.username.toLowerCase() === username.toLowerCase());
    if (emailTaken || usernameTaken) return res.status(409).json({ message: 'Email or username is already in use.' });

    const user = {
      id: uuid(),
      fullName: fullName.trim(),
      email: email.trim().toLowerCase(),
      username: username.trim(),
      passwordHash: await bcrypt.hash(password, 12),
      role: 'user',
      disabled: false,
      avatar: '',
      createdAt: new Date().toISOString(),
      updatedAt: new Date().toISOString()
    };
    users.push(user);
    await writeJson('users', users);
    await logActivity(user.id, 'user', 'Registration', 'New user account created.');
    res.status(201).json({ user: publicUser(user) });
  } catch (error) {
    next(error);
  }
});

app.post('/api/auth/login', async (req, res, next) => {
  try {
    const { username, password, remember } = req.body;
    const users = await readJson('users');
    const user = users.find((u) => u.username.toLowerCase() === String(username || '').toLowerCase() || u.email === String(username || '').toLowerCase());
    if (!user || user.disabled) return res.status(401).json({ message: 'Invalid credentials or disabled account.' });
    const valid = await bcrypt.compare(password || '', user.passwordHash);
    if (!valid) return res.status(401).json({ message: 'Invalid credentials.' });
    req.session.cookie.maxAge = remember ? 1000 * 60 * 60 * 24 * 30 : 1000 * 60 * 60 * 8;
    req.session.user = publicUser(user);
    await logActivity(user.id, 'user', 'Login', 'User signed in.');
    res.json({ user: req.session.user });
  } catch (error) {
    next(error);
  }
});

app.post('/api/admin/login', async (req, res, next) => {
  try {
    const { username, password } = req.body;
    const admins = await readJson('admins');
    const admin = admins.find((u) => u.username.toLowerCase() === String(username || '').toLowerCase() || u.email === String(username || '').toLowerCase());
    if (!admin) return res.status(401).json({ message: 'Invalid admin credentials.' });
    const valid = await bcrypt.compare(password || '', admin.passwordHash);
    if (!valid) return res.status(401).json({ message: 'Invalid admin credentials.' });
    req.session.user = publicUser(admin);
    await logActivity(admin.id, 'admin', 'Login', 'Admin signed in.');
    res.json({ user: req.session.user });
  } catch (error) {
    next(error);
  }
});

app.post('/api/admin/setup', async (req, res, next) => {
  try {
    const setupKey = process.env.ADMIN_SETUP_KEY;
    if (!setupKey || req.headers['x-setup-key'] !== setupKey) {
      return res.status(403).json({ message: 'Protected setup key required.' });
    }
    const { fullName, email, username, password } = req.body;
    if (!fullName || !validateEmail(email) || !username || !validatePassword(password)) {
      return res.status(400).json({ message: 'Valid full name, email, username, and 8+ character password are required.' });
    }
    const admins = await readJson('admins');
    const users = await readJson('users');
    if ([...admins, ...users].some((a) => a.email === email.toLowerCase() || a.username.toLowerCase() === username.toLowerCase())) {
      return res.status(409).json({ message: 'Account already exists.' });
    }
    const admin = {
      id: uuid(),
      fullName,
      email: email.toLowerCase(),
      username,
      passwordHash: await bcrypt.hash(password, 12),
      role: 'admin',
      createdAt: new Date().toISOString()
    };
    admins.push(admin);
    await writeJson('admins', admins);
    await logActivity(admin.id, 'admin', 'Admin Setup', 'Protected setup route created an admin.');
    res.status(201).json({ user: publicUser(admin) });
  } catch (error) {
    next(error);
  }
});

app.post('/api/auth/logout', async (req, res, next) => {
  try {
    const user = req.session.user;
    if (user) await logActivity(user.id, user.role, 'Logout', 'Session ended.');
    req.session.destroy(() => res.json({ message: 'Logged out.' }));
  } catch (error) {
    next(error);
  }
});

app.get('/api/tasks', requireUser, async (req, res, next) => {
  try {
    const tasks = (await readJson('tasks')).filter((task) => taskMatchesOwner(task, req));
    res.json(tasks);
  } catch (error) {
    next(error);
  }
});

app.post('/api/tasks', requireUser, async (req, res, next) => {
  try {
    const { title, description, dueDate, dueTime, priority, category, tags, reminderTime } = req.body;
    if (!title || !dueDate || !dueTime || !priority) {
      return res.status(400).json({ message: 'Title, due date, due time, and priority are required.' });
    }
    const tasks = await readJson('tasks');
    const task = {
      id: uuid(),
      userId: req.session.user.id,
      title: title.trim(),
      description: (description || '').trim(),
      dueDate,
      dueTime,
      priority,
      category: (category || 'General').trim(),
      tags: Array.isArray(tags) ? tags : String(tags || '').split(',').map((tag) => tag.trim()).filter(Boolean),
      reminderTime: reminderTime || '',
      completed: false,
      completedAt: null,
      createdAt: new Date().toISOString(),
      updatedAt: new Date().toISOString()
    };
    tasks.unshift(task);
    await writeJson('tasks', tasks);
    if (task.reminderTime) {
      const reminders = await readJson('reminders');
      reminders.push({ id: uuid(), taskId: task.id, userId: task.userId, remindAt: task.reminderTime, seen: false, createdAt: new Date().toISOString() });
      await writeJson('reminders', reminders);
    }
    await logActivity(req.session.user.id, 'user', 'Task Creation', task.title);
    res.status(201).json(task);
  } catch (error) {
    next(error);
  }
});

app.put('/api/tasks/:id', requireUser, async (req, res, next) => {
  try {
    const tasks = await readJson('tasks');
    const index = tasks.findIndex((task) => task.id === req.params.id && taskMatchesOwner(task, req));
    if (index === -1) return res.status(404).json({ message: 'Task not found.' });
    const current = tasks[index];
    const allowed = ['title', 'description', 'dueDate', 'dueTime', 'priority', 'category', 'tags', 'reminderTime', 'completed'];
    const updates = {};
    for (const key of allowed) {
      if (Object.prototype.hasOwnProperty.call(req.body, key)) updates[key] = req.body[key];
    }
    const nextTask = {
      ...current,
      ...updates,
      tags: updates.tags === undefined
        ? current.tags
        : Array.isArray(updates.tags)
          ? updates.tags
          : String(updates.tags || '').split(',').map((tag) => tag.trim()).filter(Boolean),
      updatedAt: new Date().toISOString()
    };
    if (updates.completed !== undefined && updates.completed !== current.completed) {
      nextTask.completedAt = updates.completed ? new Date().toISOString() : null;
      await logActivity(req.session.user.id, 'user', 'Task Completion', nextTask.title);
    }
    tasks[index] = nextTask;
    await writeJson('tasks', tasks);
    res.json(nextTask);
  } catch (error) {
    next(error);
  }
});

app.delete('/api/tasks/:id', requireUser, async (req, res, next) => {
  try {
    const tasks = await readJson('tasks');
    const task = tasks.find((item) => item.id === req.params.id && taskMatchesOwner(item, req));
    if (!task) return res.status(404).json({ message: 'Task not found.' });
    await writeJson('tasks', tasks.filter((item) => item.id !== req.params.id));
    const reminders = await readJson('reminders');
    await writeJson('reminders', reminders.filter((reminder) => reminder.taskId !== req.params.id));
    await logActivity(req.session.user.id, 'user', 'Task Deletion', task.title);
    res.json({ message: 'Task deleted.' });
  } catch (error) {
    next(error);
  }
});

app.get('/api/reminders', requireUser, async (req, res, next) => {
  try {
    const tasks = (await readJson('tasks')).filter((task) => taskMatchesOwner(task, req));
    const now = new Date();
    const alerts = tasks
      .filter((task) => !task.completed)
      .map((task) => ({ ...task, dueAt: new Date(`${task.dueDate}T${task.dueTime || '00:00'}`) }))
      .filter((task) => task.dueAt <= new Date(now.getTime() + 24 * 60 * 60 * 1000) || (task.reminderTime && new Date(task.reminderTime) <= now))
      .sort((a, b) => a.dueAt - b.dueAt);
    res.json(alerts);
  } catch (error) {
    next(error);
  }
});

app.get('/api/profile', requireUser, (req, res) => res.json(req.session.user));

app.put('/api/profile', requireUser, async (req, res, next) => {
  try {
    const { fullName, email, username } = req.body;
    if (!fullName || !validateEmail(email) || !username) return res.status(400).json({ message: 'Valid profile details are required.' });
    const users = await readJson('users');
    const index = users.findIndex((user) => user.id === req.session.user.id);
    if (index === -1) return res.status(404).json({ message: 'User not found.' });
    if (users.some((user) => user.id !== req.session.user.id && (user.email === email.toLowerCase() || user.username.toLowerCase() === username.toLowerCase()))) {
      return res.status(409).json({ message: 'Email or username already exists.' });
    }
    users[index] = { ...users[index], fullName, email: email.toLowerCase(), username, updatedAt: new Date().toISOString() };
    await writeJson('users', users);
    req.session.user = publicUser(users[index]);
    await logActivity(req.session.user.id, 'user', 'Profile Updates', 'Profile details updated.');
    res.json(req.session.user);
  } catch (error) {
    next(error);
  }
});

app.put('/api/profile/password', requireUser, async (req, res, next) => {
  try {
    const { currentPassword, newPassword } = req.body;
    if (!validatePassword(newPassword)) return res.status(400).json({ message: 'New password must be at least 8 characters.' });
    const users = await readJson('users');
    const index = users.findIndex((user) => user.id === req.session.user.id);
    const valid = await bcrypt.compare(currentPassword || '', users[index].passwordHash);
    if (!valid) return res.status(401).json({ message: 'Current password is incorrect.' });
    users[index].passwordHash = await bcrypt.hash(newPassword, 12);
    users[index].updatedAt = new Date().toISOString();
    await writeJson('users', users);
    await logActivity(req.session.user.id, 'user', 'Profile Updates', 'Password changed.');
    res.json({ message: 'Password updated.' });
  } catch (error) {
    next(error);
  }
});

app.post('/api/profile/avatar', requireUser, upload.single('avatar'), async (req, res, next) => {
  try {
    if (!req.file) return res.status(400).json({ message: 'Choose an image file.' });
    const users = await readJson('users');
    const index = users.findIndex((user) => user.id === req.session.user.id);
    users[index].avatar = `/assets/uploads/${req.file.filename}`;
    users[index].updatedAt = new Date().toISOString();
    await writeJson('users', users);
    req.session.user = publicUser(users[index]);
    await logActivity(req.session.user.id, 'user', 'Profile Updates', 'Avatar uploaded.');
    res.json(req.session.user);
  } catch (error) {
    next(error);
  }
});

app.get('/api/admin/users', requireAdmin, async (req, res, next) => {
  try {
    res.json((await readJson('users')).map(publicUser));
  } catch (error) {
    next(error);
  }
});

app.put('/api/admin/users/:id/disable', requireAdmin, async (req, res, next) => {
  try {
    const users = await readJson('users');
    const index = users.findIndex((user) => user.id === req.params.id);
    if (index === -1) return res.status(404).json({ message: 'User not found.' });
    users[index].disabled = Boolean(req.body.disabled);
    await writeJson('users', users);
    await logActivity(req.session.user.id, 'admin', users[index].disabled ? 'Disable User' : 'Enable User', users[index].username);
    res.json(publicUser(users[index]));
  } catch (error) {
    next(error);
  }
});

app.delete('/api/admin/users/:id', requireAdmin, async (req, res, next) => {
  try {
    const users = await readJson('users');
    const user = users.find((item) => item.id === req.params.id);
    if (!user) return res.status(404).json({ message: 'User not found.' });
    await writeJson('users', users.filter((item) => item.id !== req.params.id));
    const tasks = await readJson('tasks');
    await writeJson('tasks', tasks.filter((task) => task.userId !== req.params.id));
    const reminders = await readJson('reminders');
    await writeJson('reminders', reminders.filter((reminder) => reminder.userId !== req.params.id));
    await logActivity(req.session.user.id, 'admin', 'Delete User', user.username);
    res.json({ message: 'User deleted.' });
  } catch (error) {
    next(error);
  }
});

app.get('/api/admin/tasks', requireAdmin, async (req, res, next) => {
  try {
    res.json(await readJson('tasks'));
  } catch (error) {
    next(error);
  }
});

app.get('/api/admin/logs', requireAdmin, async (req, res, next) => {
  try {
    res.json(await readJson('activityLogs'));
  } catch (error) {
    next(error);
  }
});

app.get('/api/admin/stats', requireAdmin, async (req, res, next) => {
  try {
    const users = await readJson('users');
    const tasks = await readJson('tasks');
    const logs = await readJson('activityLogs');
    const now = new Date();
    res.json({
      users: users.length,
      disabledUsers: users.filter((u) => u.disabled).length,
      tasks: tasks.length,
      completedTasks: tasks.filter((t) => t.completed).length,
      overdueTasks: tasks.filter((t) => !t.completed && new Date(`${t.dueDate}T${t.dueTime}`) < now).length,
      activityLogs: logs.length
    });
  } catch (error) {
    next(error);
  }
});

app.use((error, req, res, next) => {
  console.error(error);
  res.status(error.status || 500).json({ message: error.message || 'Something went wrong.' });
});

ensureDatabase().then(() => {
  app.listen(PORT, () => console.log(`Daily Task Manager running at http://localhost:${PORT}`));
});
