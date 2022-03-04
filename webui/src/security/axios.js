import axios from 'axios';


export default axios.create({
  baseURL: `localhost:/8080`,
  headers: { "Accept": "application/json", "Content-Type": "application/json" },
});
