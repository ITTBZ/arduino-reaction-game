const TOKEN_KEY = 'token';

const setToken = (token) => localStorage.setItem(TOKEN_KEY, token);
const getToken = () => localStorage.getItem(TOKEN_KEY);
const destroyToken = () => localStorage.removeItem(TOKEN_KEY);

export { setToken, getToken, destroyToken };
