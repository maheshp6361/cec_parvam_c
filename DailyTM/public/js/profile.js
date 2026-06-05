document.addEventListener('DOMContentLoaded', async () => {
  const profileForm = document.querySelector('#profileForm');
  if (!profileForm) return;
  const user = await App.requireRole('user');
  fillProfile(user);

  profileForm.addEventListener('submit', async (event) => {
    event.preventDefault();
    try {
      const updated = await App.api('/api/profile', { method: 'PUT', body: JSON.stringify(Object.fromEntries(new FormData(profileForm))) });
      fillProfile(updated);
      App.toast('Profile updated.', 'success');
    } catch (error) {
      App.toast(error.message, 'error');
    }
  });

  document.querySelector('#passwordForm').addEventListener('submit', async (event) => {
    event.preventDefault();
    try {
      await App.api('/api/profile/password', { method: 'PUT', body: JSON.stringify(Object.fromEntries(new FormData(event.target))) });
      event.target.reset();
      App.toast('Password changed.', 'success');
    } catch (error) {
      App.toast(error.message, 'error');
    }
  });

  document.querySelector('#avatarForm').addEventListener('submit', async (event) => {
    event.preventDefault();
    try {
      const response = await fetch('/api/profile/avatar', { method: 'POST', credentials: 'include', body: new FormData(event.target) });
      const data = await response.json();
      if (!response.ok) throw new Error(data.message);
      fillProfile(data);
      App.toast('Profile picture uploaded.', 'success');
    } catch (error) {
      App.toast(error.message, 'error');
    }
  });
});

function fillProfile(user) {
  document.querySelector('#profileName').textContent = user.fullName;
  document.querySelector('#avatarPreview').src = user.avatar || '/assets/avatar.svg';
  ['fullName', 'email', 'username'].forEach((key) => { document.querySelector(`[name="${key}"]`).value = user[key] || ''; });
}
