
const http = require('http');
const websocket = require('websocket');

const jwt = require('jsonwebtoken');

const ENV_TOKEN_KEY = "TOKEN";

const SECRET_KEY = process.env[ENV_TOKEN_KEY];

const httpServer = http.createServer((req, res) => {
  console.log(`${req.method.toUpperCase()} ${req.url}`);

  const respond = (code, data, contentType = 'text/plain') => {
    res.writeHead(code, {
      'Content-Type' : contentType,
      'Access-Control-Allow-Origin' : '*',
    });
    res.end(data);
  };

  respond(404, 'Not Found');
});


const wsServer = new websocket.server({httpServer});

const devices_path = "devices";
const clients_path = "clients";

const mapUser = {}

wsServer.on('request', (req) => {
  console.log(`WS  ${req.resource}`);

  const {query} = req.resourceURL;

  let token = undefined;

  if (req.httpRequest.headers.authorization) {
    token = req.httpRequest.headers.authorization.split(' ').pop()
  } else if (query.token) {
    token = query.token
  }

  if (!token) {
    return;
  }

  // parse token

  


  console.log('auth token = ' + token);

  const conn = req.accept(null, req.origin);

  if (!mapUser[token]) {
    mapUser[token] = {
      mapClients: {},
      mapDevices: {}
    }
  }

  let { mapClients, mapDevices } = mapUser[token];

  if (type === clients_path) {
    conn.send(JSON.stringify(Object.values(mapDevices).map(x => x.message)));

    mapClients[id] = { conn };
  } else {
    mapDevices[id] = { conn };
  }

  conn.on('message', (data) => {
    if (data.type === 'utf8') {
      const message = JSON.parse(data.utf8Data);

      if (type === devices_path) {
        console.log(`Devices ${id} << ${data.utf8Data}`);

        Object.values(mapClients).forEach(client => {
          client.conn.send(JSON.stringify(message))
        });

        mapDevices[id].message = message;

      } else if (type === clients_path) {
        console.log(`Clients ${id} << ${data.utf8Data}`);

        // get the wright device from the request
        const idTarget = message.id;
        const device = mapDevices[idTarget];

        device.conn.send(JSON.stringify(message));

        mapClients[id].message = message;
      }
    }
  });
  conn.on('close', () => {
    if (type === clients_path) {
      delete mapClients[id];
      console.error(`Client ${id} disconnected`);
    } else {
      delete mapDevices[id];
      console.error(`Device ${id} disconnected`);
      Object.values(mapClients).forEach(client => {
        client.conn.send(JSON.stringify({id, type: 'disconnect'}));
      });
    }
  });

});

const endpoint = process.env.PORT || '8000';
const splitted = endpoint.split(':');
const port = splitted.pop();
const hostname = splitted.join(':') || '0.0.0.0';

httpServer.listen(port, hostname,
  () => { console.log(`Server listening on ${hostname}:${port}`); });
