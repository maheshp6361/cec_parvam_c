const App = {
  async api(url, options = {}) {
    const response = await fetch(url, {
      credentials: 'include',
      headers: { 'Content-Type': 'application/json', ...(options.headers || {}) },
      ...options
    });
    const data = await response.json().catch(() => ({}));
    if (!response.ok) throw new Error(data.message || 'Request failed.');
    return data;
  },
  toast(message, type = 'info') {
    let stack = document.querySelector('.toast-stack');
    if (!stack) {
      stack = document.createElement('div');
      stack.className = 'toast-stack';
      document.body.appendChild(stack);
    }
    const toast = document.createElement('div');
    toast.className = 'toast';
    toast.style.borderLeftColor = type === 'error' ? 'var(--danger)' : type === 'success' ? 'var(--success)' : 'var(--primary)';
    toast.textContent = message;
    stack.appendChild(toast);
    setTimeout(() => toast.remove(), 3600);
  },
  applyTheme() {
    const theme = localStorage.getItem('dtm-theme') || 'light';
    document.documentElement.dataset.theme = theme;
    document.body.dataset.theme = theme;
  },
  toggleTheme() {
    const next = (localStorage.getItem('dtm-theme') || 'light') === 'dark' ? 'light' : 'dark';
    localStorage.setItem('dtm-theme', next);
    App.applyTheme();
  },
  formatDate(date) {
    if (!date) return 'Not set';
    return new Intl.DateTimeFormat(undefined, { dateStyle: 'medium', timeStyle: date.includes('T') ? 'short' : undefined }).format(new Date(date));
  },
  initChrome() {
    App.applyTheme();
    document.querySelectorAll('[data-theme-toggle]').forEach((button) => button.addEventListener('click', App.toggleTheme));
    document.querySelectorAll('[data-logout]').forEach((button) => {
      button.addEventListener('click', async () => {
        await App.api('/api/auth/logout', { method: 'POST' });
        location.href = '/pages/login.html';
      });
    });
    const menu = document.querySelector('[data-mobile-menu]');
    const sidebar = document.querySelector('.sidebar');
    if (menu && sidebar) menu.addEventListener('click', () => sidebar.classList.toggle('open'));
    if (window.lucide) window.lucide.createIcons();
  },
  async requireRole(role) {
    const session = await App.api('/api/session');
    if (!session.user || session.user.role !== role) {
      location.href = role === 'admin' ? '/pages/admin.html' : '/pages/login.html';
      return null;
    }
    document.querySelectorAll('[data-user-name]').forEach((node) => { node.textContent = session.user.fullName; });
    return session.user;
  }
};

window.App = App;
document.addEventListener('DOMContentLoaded', App.initChrome);

document.addEventListener('DOMContentLoaded', () => {
  const registerForm = document.querySelector('#registerForm');
  if (registerForm) {
    registerForm.addEventListener('submit', async (event) => {
      event.preventDefault();
      const body = Object.fromEntries(new FormData(registerForm));
      try {
        await App.api('/api/auth/register', { method: 'POST', body: JSON.stringify(body) });
        App.toast('Account created. You can sign in now.', 'success');
        setTimeout(() => { location.href = '/pages/login.html'; }, 800);
      } catch (error) {
        App.toast(error.message, 'error');
      }
    });
  }

  const loginForm = document.querySelector('#loginForm');
  if (loginForm) {
    loginForm.addEventListener('submit', async (event) => {
      event.preventDefault();
      const body = Object.fromEntries(new FormData(loginForm));
      body.remember = Boolean(body.remember);
      try {
        await App.api('/api/auth/login', { method: 'POST', body: JSON.stringify(body) });
        location.href = '/pages/dashboard.html';
      } catch (error) {
        App.toast(error.message, 'error');
      }
    });
  }

  const adminLoginForm = document.querySelector('#adminLoginForm');
  if (adminLoginForm) {
    adminLoginForm.addEventListener('submit', async (event) => {
      event.preventDefault();
      const body = Object.fromEntries(new FormData(adminLoginForm));
      try {
        await App.api('/api/admin/login', { method: 'POST', body: JSON.stringify(body) });
        location.href = '/pages/admin.html';
      } catch (error) {
        App.toast(error.message, 'error');
      }
    });
  }
});
