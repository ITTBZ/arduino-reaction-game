<template>
  <div class="leaderboardPage">
    <h1>LeaderBoard</h1>
    <div class="layout">
      <div class="leaderboard">
        <table class="styled-table">
          <thead>
            <tr>
              <th>username</th>
              <th>score</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>Max</td>
              <td>17</td>
            </tr>
            <tr class="active-row">
              <td>Dillan</td>
              <td>358</td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>
<script>
//import store from "../store/index.js";
export default {
  name: "LeaderboardPage",
  data() {
    return {
      username: "username",
      score: 0,
    };
  },
};
</script>
<style>
body {
  background-color: rgb(216, 216, 216);
  height: 100%;
  width: 100%;
}
.layout {
  display: flex;
  justify-content: center;
}
.styled-table {
    border-collapse: collapse;
    margin: 25px 0;
    font-size: 0.9em;
    font-family: sans-serif;
    min-width: 400px;
    box-shadow: 0 0 20px rgba(0, 0, 0, 0.15);
}
.styled-table thead tr {
    background-color: #009879;
    color: #ffffff;
}
.styled-table tbody td,
.styled-table thead tr {
  text-align: left; 
}
.styled-table th,
.styled-table td {
    padding: 12px 15px;
}
.styled-table tbody tr {
    border-bottom: 1px solid #dddddd;
}

.styled-table tbody tr:nth-of-type(even) {
    background-color: #f3f3f3;
}

.styled-table tbody tr:last-of-type {
    border-bottom: 2px solid #009879;
}
.styled-table tbody tr.active-row {
    font-weight: bold;
    color: #009879;
}
</style>
