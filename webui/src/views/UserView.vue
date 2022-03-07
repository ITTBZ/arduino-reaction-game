<template>
  <div>
    <h2>Welcome, {{ username }}!</h2>
    <div>
      Link user code 
    </div>
  </div>
</template>

<script>
import api from '../services/api';

export default {
  name: 'UserView',
  data() {
    return {
      user: {
        username: '',
        tempCode: ''
      },
      errors: [],
    };
  },
  methods: {
    register() {
      axios
        .post('/users', this.user)
        .then((response) => {
          if (response.status == 201) {
            this.$router.push('/');
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
  width: 5rem;
}
</style>
