import { getToken } from '@/services/jwt';

const requireAuth = (to, from, next) => {
  if (getToken() === null) {
    next({
      path: '/auth',
      query: { redirect: encodeURIComponent(to.fullPath) }
    });
  } else {
    next();
  }
};

export { requireAuth };
