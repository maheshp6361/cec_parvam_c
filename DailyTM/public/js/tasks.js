let tasks = [];
let editingId = null;
let currentPage = 1;
const pageSize = 10;

const els = {};

document.addEventListener('DOMContentLoaded', async () => {
  if (!document.querySelector('#taskList')) return;
  Object.assign(els, {
    taskList: document.querySelector('#taskList'),
    completedList: document.querySelector('#completedList'),
    taskForm: document.querySelector('#taskForm'),
    modal: document.querySelector('#taskModal'),
    modalTitle: document.querySelector('#modalTitle'),
    search: document.querySelector('#searchInput'),
    priority: document.querySelector('#filterPriority'),
    status: document.querySelector('#filterStatus'),
    category: document.querySelector('#filterCategory'),
    dueDate: document.querySelector('#filterDueDate'),
    sort: document.querySelector('#sortTasks'),
    pageInfo: document.querySelector('#pageInfo')
  });
  await App.requireRole('user');
  wireTasks();
  await loadTasks();
});

function wireTasks() {
  document.querySelector('#newTaskBtn').addEventListener('click', () => openTaskModal());
  document.querySelector('#closeTaskModal').addEventListener('click', closeTaskModal);
  document.querySelector('#cancelTask').addEventListener('click', closeTaskModal);
  document.querySelector('#prevPage').addEventListener('click', () => { currentPage = Math.max(1, currentPage - 1); renderTasks(); });
  document.querySelector('#nextPage').addEventListener('click', () => { currentPage += 1; renderTasks(); });
  document.querySelector('#exportJson').addEventListener('click', () => download('tasks.json', JSON.stringify(tasks, null, 2), 'application/json'));
  document.querySelector('#exportCsv').addEventListener('click', exportCsv);
  document.querySelector('#importTasks').addEventListener('change', importTasks);
  [els.search, els.priority, els.status, els.category, els.dueDate, els.sort].forEach((el) => el.addEventListener('input', () => { currentPage = 1; renderTasks(); }));
  els.taskForm.addEventListener('submit', saveTask);
}

async function loadTasks() {
  els.taskList.innerHTML = '<div class="task-card skeleton"></div><div class="task-card skeleton"></div>';
  tasks = await App.api('/api/tasks');
  renderCategoryOptions();
  renderTasks();
  if (typeof renderDashboard === 'function') renderDashboard(tasks);
}

function filteredTasks() {
  const query = els.search.value.trim().toLowerCase();
  const dueDate = els.dueDate.value;
  let list = tasks.filter((task) => {
    const haystack = [task.title, task.description, task.category, ...(task.tags || [])].join(' ').toLowerCase();
    return (!query || haystack.includes(query))
      && (!els.priority.value || task.priority === els.priority.value)
      && (!els.status.value || (els.status.value === 'completed' ? task.completed : !task.completed))
      && (!els.category.value || task.category === els.category.value)
      && (!dueDate || task.dueDate === dueDate);
  });
  const rank = { High: 1, Medium: 2, Low: 3 };
  list.sort((a, b) => {
    if (els.sort.value === 'oldest') return new Date(a.createdAt) - new Date(b.createdAt);
    if (els.sort.value === 'priority') return rank[a.priority] - rank[b.priority];
    if (els.sort.value === 'dueDate') return new Date(`${a.dueDate}T${a.dueTime}`) - new Date(`${b.dueDate}T${b.dueTime}`);
    if (els.sort.value === 'alpha') return a.title.localeCompare(b.title);
    return new Date(b.createdAt) - new Date(a.createdAt);
  });
  return list;
}

function renderTasks() {
  const list = filteredTasks();
  const totalPages = Math.max(1, Math.ceil(list.length / pageSize));
  currentPage = Math.min(currentPage, totalPages);
  const page = list.slice((currentPage - 1) * pageSize, currentPage * pageSize);
  els.taskList.innerHTML = page.length ? page.map(taskCard).join('') : '<div class="empty">No tasks match your current view.</div>';
  els.completedList.innerHTML = tasks.filter((task) => task.completed).slice(0, 6).map(taskCard).join('') || '<div class="empty">Completed tasks will appear here.</div>';
  els.pageInfo.textContent = `Page ${currentPage} of ${totalPages}`;
  document.querySelector('#prevPage').disabled = currentPage === 1;
  document.querySelector('#nextPage').disabled = currentPage === totalPages;
  bindTaskActions();
}

function taskCard(task) {
  const dueAt = new Date(`${task.dueDate}T${task.dueTime}`);
  const overdue = !task.completed && dueAt < new Date();
  return `
    <article class="task-card ${task.completed ? 'completed' : ''}" data-id="${task.id}">
      <div class="task-head">
        <div class="task-title">
          <input type="checkbox" ${task.completed ? 'checked' : ''} data-complete aria-label="Toggle complete">
          <h3>${escapeHtml(task.title)}</h3>
        </div>
        <span class="badge priority-${task.priority.toLowerCase()}">${task.priority}</span>
      </div>
      <p class="muted">${escapeHtml(task.description || 'No description')}</p>
      <div class="badge-row">
        <span class="badge">${escapeHtml(task.category || 'General')}</span>
        <span class="badge">${task.dueDate} at ${task.dueTime}</span>
        <span class="badge">${task.completed ? 'Completed' : overdue ? 'Overdue' : 'Pending'}</span>
      </div>
      <div class="badge-row">${(task.tags || []).map((tag) => `<span class="badge">#${escapeHtml(tag)}</span>`).join('')}</div>
      <div class="task-actions">
        <button class="btn icon-btn" data-edit title="Edit task"><i data-lucide="pencil"></i></button>
        <button class="btn icon-btn danger" data-delete title="Delete task"><i data-lucide="trash-2"></i></button>
        ${task.completedAt ? `<span class="muted">Completed ${App.formatDate(task.completedAt)}</span>` : ''}
      </div>
    </article>`;
}

function bindTaskActions() {
  document.querySelectorAll('[data-edit]').forEach((button) => button.addEventListener('click', () => openTaskModal(tasks.find((task) => task.id === button.closest('.task-card').dataset.id))));
  document.querySelectorAll('[data-delete]').forEach((button) => button.addEventListener('click', () => deleteTask(button.closest('.task-card').dataset.id)));
  document.querySelectorAll('[data-complete]').forEach((checkbox) => checkbox.addEventListener('change', () => toggleComplete(checkbox.closest('.task-card').dataset.id, checkbox.checked)));
  if (window.lucide) window.lucide.createIcons();
}

function openTaskModal(task = null) {
  editingId = task?.id || null;
  els.modalTitle.textContent = editingId ? 'Edit task' : 'Create task';
  els.taskForm.reset();
  if (task) {
    for (const [key, value] of Object.entries(task)) {
      const field = els.taskForm.elements[key];
      if (field) field.value = Array.isArray(value) ? value.join(', ') : value || '';
    }
  }
  els.modal.classList.add('open');
}

function closeTaskModal() {
  els.modal.classList.remove('open');
  editingId = null;
}

async function saveTask(event) {
  event.preventDefault();
  const body = Object.fromEntries(new FormData(els.taskForm));
  body.tags = String(body.tags || '').split(',').map((tag) => tag.trim()).filter(Boolean);
  const url = editingId ? `/api/tasks/${editingId}` : '/api/tasks';
  const method = editingId ? 'PUT' : 'POST';
  try {
    await App.api(url, { method, body: JSON.stringify(body) });
    App.toast(editingId ? 'Task updated.' : 'Task created.', 'success');
    closeTaskModal();
    await loadTasks();
  } catch (error) {
    App.toast(error.message, 'error');
  }
}

async function toggleComplete(id, completed) {
  const task = tasks.find((item) => item.id === id);
  await App.api(`/api/tasks/${id}`, { method: 'PUT', body: JSON.stringify({ ...task, completed }) });
  await loadTasks();
}

async function deleteTask(id) {
  if (!confirm('Delete this task? This cannot be undone.')) return;
  await App.api(`/api/tasks/${id}`, { method: 'DELETE' });
  App.toast('Task deleted.', 'success');
  await loadTasks();
}

function renderCategoryOptions() {
  const categories = [...new Set(tasks.map((task) => task.category).filter(Boolean))].sort();
  els.category.innerHTML = '<option value="">All categories</option>' + categories.map((cat) => `<option>${escapeHtml(cat)}</option>`).join('');
}

function download(filename, content, type) {
  const blob = new Blob([content], { type });
  const link = document.createElement('a');
  link.href = URL.createObjectURL(blob);
  link.download = filename;
  link.click();
  URL.revokeObjectURL(link.href);
}

function exportCsv() {
  const rows = [['Title', 'Description', 'Due Date', 'Due Time', 'Priority', 'Category', 'Tags', 'Completed']];
  tasks.forEach((task) => rows.push([task.title, task.description, task.dueDate, task.dueTime, task.priority, task.category, (task.tags || []).join('|'), task.completed]));
  download('tasks.csv', rows.map((row) => row.map((cell) => `"${String(cell || '').replaceAll('"', '""')}"`).join(',')).join('\n'), 'text/csv');
}

async function importTasks(event) {
  const file = event.target.files[0];
  if (!file) return;
  try {
    const imported = JSON.parse(await file.text());
    for (const task of imported) {
      await App.api('/api/tasks', {
        method: 'POST',
        body: JSON.stringify({
          title: task.title,
          description: task.description,
          dueDate: task.dueDate,
          dueTime: task.dueTime,
          priority: task.priority || 'Medium',
          category: task.category || 'Imported',
          tags: task.tags || [],
          reminderTime: task.reminderTime || ''
        })
      });
    }
    App.toast('Tasks imported.', 'success');
    await loadTasks();
  } catch {
    App.toast('Import failed. Use a valid JSON task export.', 'error');
  }
}

function escapeHtml(value) {
  return String(value || '').replace(/[&<>"']/g, (char) => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#039;' }[char]));
}
