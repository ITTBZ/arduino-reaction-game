<template>
  <div>
    <h2>Login</h2>
    <p v-if="$route.query.redirect">You need to login first.</p>
    <form @submit.prevent="login">
      <label><input v-model="user.username" placeholder="username" /></label>
      <label
        ><input v-model="user.password" placeholder="password" type="password"
      /></label>
      <button type="submit">login</button>
      <p v-if="error" class="error">Bad login information</p>
    </form>
  </div>
</template>

<script>
import axios from '../security/axios';
export default {
  name: 'LoginPage',
  data() {
    return {
      user: {
        username: '',
        password: '',
      },
      errors: [],
    };
  },

  methods: {
    login() {
      axios
        .post('/users/@me', this.user)
        .then((resp) => {
          console.log(resp.headers);
          const token = resp.data.token;
          localStorage.setItem('access_token', token); // store the token in localstorage
          this.$router.push('/leaderboard');
        })
        .catch((e) => {
          localStorage.removeItem('access_token'); // if the request fails, remove any possible user token if possible
          this.errors.push(e);
        });
    },
  },
};
</script>

<style>
.error {
  color: red;
}
form {
  display: flex;
  flex-direction: column;
  align-items: center;
}
button {
  width: 5rem;
}
</style>
