## POST /users -- Register a new user

### Request

| Property |  Type  | Note                |
| -------- | ------ | ------------------- |
| username | String | The user's username |
| password | String | The user's password |

### Response

Get the user object.

```json
HTTP/1.1 201 Created
{
  "id": 1,
  "username": "ExampleUsername"
}
```

## POST /users/@me -- Login to the website

### Request

| Property |  Type  | Note                |
| -------- | ------ | ------------------- |
| username | String | The user's username |
| password | String | The user's password |

### Response

Get the authorization token as a header.

```json
HTTP/1.1 200 OK
Authorization: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c
```

## GET /users/@me -- Get your user

### Response

Get the user object.

```json
HTTP/1.1 200 Ok
{
  "id": 1,
  "username": "ExampleUsername"
}
```

## POST /games -- Create a new game

### Request

| Property |  Type   | Note                             |
| -------- | ------- | -------------------------------- |
| score    | Integer | The user's score in milliseconds |

### Response

Get the game object.

```json
HTTP/1.1 201 Created
{
  "id": 1,
  "userRef": 190,
  "score": 100
}
```

## GET /leaderboard -- Get the leaderboard

### Response

Get an array with all players sorted by their average score.

```json
HTTP/1.1 200 Ok
[
  {
    "id": 190,
    "username": "ProGamer123",
    "score": 100
  }
  {
    "id": 33,
    "username": "FancyGamer121",
    "score": 234
  },{
    "id": 182,
    "username": "GamerGirl12",
    "score": 235
  }
]
```
