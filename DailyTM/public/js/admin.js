let adminUsers = [];
let adminTasks = [];

document.addEventListener('DOMContentLoaded', async () => {
  if (!document.querySelector('#adminStats')) return;
  const session = await App.api('/api/session');
  if (!session.user || session.user.role !== 'admin') {
    document.querySelector('#adminLoginView').hidden = false;
    document.querySelector('#adminDashboardView').hidden = true;
    return;
  }
  document.querySelector('#adminLoginView').hidden = true;
  document.querySelector('#adminDashboardView').hidden = false;
  await loadAdmin();
  document.querySelector('#userSearch').addEventListener('input', renderUsers);
});

async function loadAdmin() {
  const [stats, users, tasks, logs] = await Promise.all([
    App.api('/api/admin/stats'),
    App.api('/api/admin/users'),
    App.api('/api/admin/tasks'),
    App.api('/api/admin/logs')
  ]);
  adminUsers = users;
  adminTasks = tasks;
  document.querySelector('#adminStats').innerHTML = `
    ${adminStat('Users', stats.users)}
    ${adminStat('Disabled', stats.disabledUsers)}
    ${adminStat('Tasks', stats.tasks)}
    ${adminStat('Completed', stats.completedTasks)}
    ${adminStat('Logs', stats.activityLogs)}
  `;
  renderUsers();
  document.querySelector('#allTasksTable').innerHTML = tasks.map((task) => `<tr><td>${escapeAdmin(task.title)}</td><td>${task.priority}</td><td>${task.dueDate}</td><td>${task.completed ? 'Completed' : 'Pending'}</td></tr>`).join('') || '<tr><td colspan="4">No tasks yet.</td></tr>';
  document.querySelector('#logsTable').innerHTML = logs.map((log) => `<tr><td>${new Date(log.createdAt).toLocaleString()}</td><td>${escapeAdmin(log.actorRole)}</td><td>${escapeAdmin(log.action)}</td><td>${escapeAdmin(log.details)}</td></tr>`).join('') || '<tr><td colspan="4">No activity yet.</td></tr>';
}

function adminStat(label, value) {
  return `<section class="stat-card"><span>${label}</span><strong>${value}</strong></section>`;
}

function renderUsers() {
  const query = document.querySelector('#userSearch').value.toLowerCase();
  const users = adminUsers.filter((user) => [user.fullName, user.email, user.username].join(' ').toLowerCase().includes(query));
  document.querySelector('#usersTable').innerHTML = users.map((user) => `
    <tr>
      <td>${escapeAdmin(user.fullName)}</td>
      <td>${escapeAdmin(user.email)}</td>
      <td>${escapeAdmin(user.username)}</td>
      <td>${user.disabled ? 'Disabled' : 'Active'}</td>
      <td>
        <button class="btn" data-user-toggle="${user.id}" data-disabled="${!user.disabled}">${user.disabled ? 'Enable' : 'Disable'}</button>
        <button class="btn danger" data-user-delete="${user.id}">Delete</button>
      </td>
    </tr>`).join('') || '<tr><td colspan="5">No users found.</td></tr>';
  document.querySelectorAll('[data-user-toggle]').forEach((button) => {
    button.addEventListener('click', () => toggleUser(button.dataset.userToggle, button.dataset.disabled === 'true'));
  });
  document.querySelectorAll('[data-user-delete]').forEach((button) => {
    button.addEventListener('click', () => deleteUser(button.dataset.userDelete));
  });
}

async function toggleUser(id, disabled) {
  await App.api(`/api/admin/users/${id}/disable`, { method: 'PUT', body: JSON.stringify({ disabled }) });
  await loadAdmin();
}

async function deleteUser(id) {
  if (!confirm('Delete this user and all of their tasks?')) return;
  await App.api(`/api/admin/users/${id}`, { method: 'DELETE' });
  await loadAdmin();
}

function escapeAdmin(value) {
  return String(value || '').replace(/[&<>"']/g, (char) => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#039;' }[char]));
}
