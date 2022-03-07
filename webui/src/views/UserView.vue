<template>
  <div>
    <h1>Welcome, {{ username }}!</h1>
  </div>

  <div class="form-wrap">
    <div class="tabs-content">
      <h3>Link identity</h3>
      <div class="active">
        <form class="login-form" action="" @submit.prevent="linkIdentity">
          <input type="text" class="input" autocomplete="off" placeholder="C0D5E1" minlength="6" maxlength="6" v-model="tempCode" />

          <input type="submit" class="button" value="Set link" />
        </form>
        <div class="error-text">{{ error }}</div>
      </div>
    </div>
  </div>
</template>

<script>
import { getToken } from '@/services/jwt';
import api from '../services/api';

export default {
  name: 'UserView',
  data: () => ({
    username: 'Dillan',
    tempCode: '',
    error: ''
  }),
  methods: {
    linkIdentity() {
      const code = this.tempCode;
      if (code == null) return;

      api
        .patch(
          '/identity',
          {
            code
          },
          {
            headers: {
              Authorization: `Bearer ${getToken()}`
            }
          }
        )
        .then(() => {
          this.error = '';
          this.code = '';
        })
        .catch(() => {
          this.error = 'An error occured';
        });
    }
  }
};
</script>

<style scoped>
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
.form-wrap .tabs-content h3 {
  margin-bottom: 1rem;
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
