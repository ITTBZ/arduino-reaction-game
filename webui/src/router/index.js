import { createRouter, createWebHistory } from 'vue-router'
import LoginPage from '../views/LoginPage.vue'
import RegisterPage from '../views/RegisterPage.vue'
import LeaderboardPage from '../views/LeaderboardPage.vue'

const routes = [
  {
    path: '/register',
    name: 'register',
    component: RegisterPage
  },
  {
    path: '/',
    name: 'login',
    component: LoginPage
  },
  {
    path: '/leaderboard',
    name: 'leaderboard',
    beforeEnter: requireAuth,
    component: LeaderboardPage
  },
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

function requireAuth (to, from, next) {
  if (!localStorage.getItem('access_token')) {
    next({
      path: '/',
      query: { redirect: to.fullPath }
    })
  } else {
    next()
  }
}


export default router
