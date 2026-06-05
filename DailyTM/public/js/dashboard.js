let charts = [];

document.addEventListener('DOMContentLoaded', async () => {
  if (!document.querySelector('#statsGrid')) return;
  await App.requireRole('user');
});

function renderDashboard(taskData) {
  const now = new Date();
  const stats = {
    total: taskData.length,
    pending: taskData.filter((task) => !task.completed).length,
    completed: taskData.filter((task) => task.completed).length,
    overdue: taskData.filter((task) => !task.completed && new Date(`${task.dueDate}T${task.dueTime}`) < now).length,
    high: taskData.filter((task) => task.priority === 'High').length
  };
  document.querySelector('#statsGrid').innerHTML = `
    ${stat('Total Tasks', stats.total)}
    ${stat('Pending', stats.pending)}
    ${stat('Completed', stats.completed)}
    ${stat('Overdue', stats.overdue)}
    ${stat('High Priority', stats.high)}
  `;
  drawCharts(taskData, stats);
}

function stat(label, value) {
  return `<section class="stat-card"><span>${label}</span><strong>${value}</strong></section>`;
}

function drawCharts(taskData, stats) {
  if (!window.Chart) return;
  charts.forEach((chart) => chart.destroy());
  const priorityCounts = ['Low', 'Medium', 'High'].map((priority) => taskData.filter((task) => task.priority === priority).length);
  const week = [...Array(7)].map((_, index) => {
    const date = new Date();
    date.setDate(date.getDate() - (6 - index));
    const key = date.toISOString().slice(0, 10);
    return {
      label: date.toLocaleDateString(undefined, { weekday: 'short' }),
      count: taskData.filter((task) => task.completedAt && task.completedAt.slice(0, 10) === key).length
    };
  });
  charts = [
    new Chart(document.querySelector('#statusChart'), {
      type: 'doughnut',
      data: { labels: ['Pending', 'Completed', 'Overdue'], datasets: [{ data: [stats.pending, stats.completed, stats.overdue], backgroundColor: ['#3157d5', '#16805b', '#c72f4b'] }] },
      options: { maintainAspectRatio: false, plugins: { legend: { position: 'bottom' } } }
    }),
    new Chart(document.querySelector('#priorityChart'), {
      type: 'bar',
      data: { labels: ['Low', 'Medium', 'High'], datasets: [{ label: 'Tasks', data: priorityCounts, backgroundColor: ['#2f855a', '#b7791f', '#c53030'] }] },
      options: { maintainAspectRatio: false, plugins: { legend: { display: false } }, scales: { y: { beginAtZero: true, ticks: { precision: 0 } } } }
    }),
    new Chart(document.querySelector('#weeklyChart'), {
      type: 'line',
      data: { labels: week.map((day) => day.label), datasets: [{ label: 'Completed', data: week.map((day) => day.count), borderColor: '#3157d5', backgroundColor: 'rgba(49,87,213,.14)', fill: true, tension: 0.35 }] },
      options: { maintainAspectRatio: false, scales: { y: { beginAtZero: true, ticks: { precision: 0 } } } }
    })
  ];
}
