document.addEventListener('DOMContentLoaded', () => {
  const bell = document.querySelector('#notificationBell');
  const panel = document.querySelector('#notificationPanel');
  if (!bell || !panel) return;
  bell.addEventListener('click', () => panel.classList.toggle('open'));
  document.querySelector('#closeNotifications')?.addEventListener('click', () => panel.classList.remove('open'));
  loadReminders();
  setInterval(loadReminders, 60000);
});

async function loadReminders() {
  try {
    const reminders = await App.api('/api/reminders');
    const list = document.querySelector('#notificationList');
    const count = document.querySelector('#notificationCount');
    if (!list) return;
    count.textContent = reminders.length;
    list.innerHTML = reminders.length ? reminders.map((task) => {
      const due = new Date(`${task.dueDate}T${task.dueTime}`);
      const overdue = due < new Date();
      return `<div class="notice"><strong>${task.title}</strong><p class="muted">${overdue ? 'Overdue' : 'Upcoming'}: ${due.toLocaleString()}</p></div>`;
    }).join('') : '<div class="empty">No upcoming alerts.</div>';
    if (reminders.length && Notification.permission === 'granted') {
      const task = reminders[0];
      new Notification(task.title, { body: `Due ${task.dueDate} at ${task.dueTime}` });
    }
  } catch {
    /* Session redirects handle unauthorized states elsewhere. */
  }
}

document.addEventListener('click', async (event) => {
  if (event.target.closest('#enableNotifications')) {
    if (!('Notification' in window)) return App.toast('Browser notifications are not supported.', 'error');
    const permission = await Notification.requestPermission();
    App.toast(permission === 'granted' ? 'Notifications enabled.' : 'Notifications blocked.');
  }
});
