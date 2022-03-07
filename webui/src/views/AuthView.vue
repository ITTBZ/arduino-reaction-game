<template>
  <div class="form-wrap">
    <div class="tabs">
      <h3 class="login-tab"><a :class="{ active: isLoginTabActive }" href="#" @click.prevent="switchForm(0)">Login</a></h3>
      <h3 class="signup-tab"><a :class="{ active: isRegisterTabActive }" href="#" @click.prevent="switchForm(1)">Sign Up</a></h3>
    </div>

    <div class="tabs-content">
      <div class="active">
        <form class="login-form" action="" @submit.prevent="sendAuth">
          <input type="text" class="input" autocomplete="off" placeholder="Username" v-model="username" />
          <input type="password" class="input" autocomplete="off" placeholder="Password" v-model="password" />

          <input type="submit" class="button" :value="isLoginTabActive ? 'Login' : 'Create Account'" />
        </form>
        <div class="error-text">{{ error }}</div>
      </div>
    </div>
  </div>
</template>

<script>
import api from '../services/api';
import { setToken } from '../services/jwt';

export default {
  name: 'AuthView',
  data: () => ({
    type: 0, // 0 Login | 1 Register,
    error: '',
    username: '',
    password: ''
  }),

  computed: {
    isLoginTabActive() {
      return this.type === 0;
    },
    isRegisterTabActive() {
      return this.type === 1;
    }
  },

  methods: {
    switchForm(formType) {
      this.type = formType;
      this.password = '';
      this.error = '';
    },
    sendAuth() {
      const context = this.isLoginTabActive ? '/users/@me' : '/users';

      const username = this.username;
      const password = this.password;

      if (username.length < 1 || password.length < 1) return;
      api
        .post(context, {
          username,
          password
        })
        .then((res) => {
          if (this.isLoginTabActive) {
            setToken(res.data.token);
            this.$router.push('/profile');
          } else {
            this.switchForm(0);
          }
        })
        .catch((err) => {
          console.err(err);
          this.error = this.isLoginTabActive ? 'Invalid username or password.' : 'Username is already taken.';
        });
    }
  }
};
</script>

<style>
@import url(https://fonts.googleapis.com/css?family=Raleway:400,100,200,300);
* {
  margin: 0;
  padding: 0;
}

a {
  color: #666;
  text-decoration: none;
}
a:hover {
  color: #4fda8c;
}

input {
  font: 16px/26px 'Raleway', sans-serif;
}

body {
  color: #666;
  background-color: #f1f2f2;
  font: 16px/26px 'Raleway', sans-serif;
}

.form-wrap {
  background-color: #fff;
  width: 320px;
  margin: 3em auto;
  box-shadow: 0px 1px 8px #bebebe;
  -webkit-box-shadow: 0px 1px 8px #bebebe;
  -moz-box-shadow: 0px 1px 8px #bebebe;
}
.form-wrap .tabs {
  overflow: hidden;
}
.form-wrap .tabs h3 {
  float: left;
  width: 50%;
}
.form-wrap .tabs h3 a {
  padding: 0.5em 0;
  text-align: center;
  font-weight: 400;
  background-color: #e6e7e8;
  display: block;
  color: #666;
}
.form-wrap .tabs h3 a.active {
  background-color: #fff;
}
.form-wrap .tabs-content {
  padding: 1.5em;
}
.form-wrap .tabs-content div[id$='tab-content'] {
  display: none;
}
.form-wrap .tabs-content .active {
  display: block !important;
}
.form-wrap form .input {
  box-sizing: border-box;
  -moz-box-sizing: border-box;
  color: inherit;
  font-family: inherit;
  padding: 0.8em 0 10px 0.8em;
  border: 1px solid #cfcfcf;
  outline: 0;
  display: inline-block;
  margin: 0 0 0.8em 0;
  padding-right: 2em;
  width: 100%;
}
.form-wrap form .button {
  width: 100%;
  padding: 0.8em 0 10px 0.8em;
  background-color: #28a55f;
  border: none;
  color: #fff;
  cursor: pointer;
  text-transform: uppercase;
}
.form-wrap form .button:hover {
  background-color: #4fda8c;
}
.form-wrap form .checkbox {
  visibility: hidden;
  padding: 20px;
  margin: 0.5em 0 1.5em;
}
.form-wrap form .checkbox:checked + label:after {
  -ms-filter: 'progid:DXImageTransform.Microsoft.Alpha(Opacity=100)';
  filter: alpha(opacity=100);
  opacity: 1;
}
.form-wrap form label[for] {
  position: relative;
  padding-left: 20px;
  cursor: pointer;
}
.form-wrap form label[for]:before {
  content: '';
  position: absolute;
  border: 1px solid #cfcfcf;
  width: 17px;
  height: 17px;
  top: 0px;
  left: -14px;
}
.form-wrap form label[for]:after {
  -ms-filter: 'progid:DXImageTransform.Microsoft.Alpha(Opacity=0)';
  filter: alpha(opacity=0);
  opacity: 0;
  content: '';
  position: absolute;
  width: 9px;
  height: 5px;
  background-color: transparent;
  top: 4px;
  left: -10px;
  border: 3px solid #28a55f;
  border-top: none;
  border-right: none;
  -webkit-transform: rotate(-45deg);
  -moz-transform: rotate(-45deg);
  -o-transform: rotate(-45deg);
  -ms-transform: rotate(-45deg);
  transform: rotate(-45deg);
}
.form-wrap .error-text {
  margin-top: 0.6em;
  color: red;
  text-align: center;
  font-size: 14px;
}
</style>
