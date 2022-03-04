<template>
  <div>
    <h2>Register</h2>
    <form @submit.prevent="register">
      <label><input v-model="username" placeholder="username" /></label>
      {{firstName}}
      <label
        ><input v-model="pass" placeholder="password" type="password"
      /></label>
      <button type="submit">Register</button>
      <p v-if="error" class="error">Bad Register information</p>
    </form>
  </div>
</template>

<script>
import axios from "axios";
export default {
  name: "RegisterPage",
  data() {
    return {
      user: {
        username: "",
        pass: ""
      }
    };
  },
  methods: {
    register() {

      axios
        .post("/users", this.user)
        .then((response) => {
          if(response.status == 201) {
            this.$router.push('/')
          }
        })
        .catch((e) => {
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
  width:5rem;
}
</style>
