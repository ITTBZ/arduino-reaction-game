export default (to, from, next) => {
  if (!localStorage.getItem('access_token')) {
    next({
      path: '/',
      query: { redirect: to.fullPath },
    });
  } else {
    next();
  }
};
