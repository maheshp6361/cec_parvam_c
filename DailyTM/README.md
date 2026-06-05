# Daily Task Management System

A complete task management web app built with HTML5, CSS3, vanilla JavaScript, Node.js, Express, and JSON files as the storage layer.

## Features

- User registration and login with bcrypt password hashing
- Session authentication with Remember Me support
- Protected admin login and dashboard
- JSON database files for users, admins, tasks, reminders, and activity logs
- Task create, edit, delete, complete, search, filter, sort, pagination, import, and export
- Browser reminder notifications and overdue/upcoming alerts
- Dashboard statistics with Chart.js charts
- Profile editing, password change, and profile picture upload
- Dark mode saved in local storage
- Responsive SaaS-style UI for desktop, tablet, and mobile

## Setup

```bash
npm install
npm start
```

Open:

```text
http://localhost:3000
```

## Admin Access

Admin registration is not public. You can create admins in either of these ways:

1. Edit `database/admins.json` directly and store a bcrypt password hash.
2. Use the protected setup route with an environment key:

```bash
$env:ADMIN_SETUP_KEY="your-secret-key"
npm start
```

Then send a `POST` request to `/api/admin/setup` with header `x-setup-key: your-secret-key` and body:

```json
{
  "fullName": "Admin User",
  "email": "admin@example.com",
  "username": "admin",
  "password": "choose-a-strong-password"
}
```

## Project Structure

```text
task-manager/
|-- server.js
|-- package.json
|-- database/
|   |-- users.json
|   |-- admins.json
|   |-- tasks.json
|   |-- reminders.json
|   `-- activityLogs.json
|-- public/
|   |-- css/
|   |   |-- style.css
|   |   `-- dashboard.css
|   |-- js/
|   |   |-- auth.js
|   |   |-- dashboard.js
|   |   |-- tasks.js
|   |   |-- reminders.js
|   |   |-- admin.js
|   |   `-- profile.js
|   |-- pages/
|   |   |-- login.html
|   |   |-- register.html
|   |   |-- dashboard.html
|   |   |-- admin.html
|   |   `-- profile.html
|   `-- assets/
`-- README.md
```

## API Overview

- `POST /api/auth/register`
- `POST /api/auth/login`
- `POST /api/auth/logout`
- `GET /api/session`
- `GET /api/tasks`
- `POST /api/tasks`
- `PUT /api/tasks/:id`
- `DELETE /api/tasks/:id`
- `GET /api/reminders`
- `GET /api/profile`
- `PUT /api/profile`
- `PUT /api/profile/password`
- `POST /api/profile/avatar`
- `POST /api/admin/login`
- `POST /api/admin/setup`
- `GET /api/admin/users`
- `PUT /api/admin/users/:id/disable`
- `DELETE /api/admin/users/:id`
- `GET /api/admin/tasks`
- `GET /api/admin/stats`
- `GET /api/admin/logs`

## Notes

- JSON storage is suitable for learning, demos, and small local deployments. For multi-user production traffic, migrate to a transactional database.
- Change `SESSION_SECRET` before deploying.
- Browser notifications require the user to grant permission in the browser.
