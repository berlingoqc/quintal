const jwt = require('jsonwebtoken');

const ENV_TOKEN_KEY = "TOKEN";

const SECRET_KEY = process.env[ENV_TOKEN_KEY];

const args = process.argv.slice(2);

const token = jwt.sign({ group_id: args[2], id: args[1], type: args[0] }, SECRET_KEY, { expiresIn: '365d' });

console.log(token);