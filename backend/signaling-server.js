/*
 * libdatachannel example web server
 * Copyright (C) 2020 Lara Mackey
 * Copyright (C) 2020 Paul-Louis Ageneau
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

const http = require('http');
const websocket = require('websocket');

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

/**
 * How will it work ???
 * 
 * ok the signaling server map all devices with client (s)
 * 
 * when a device is running it's automatically connected to the websocket
 * sending it's connection information
 * 
 * the client can query the list of device connected
 * 
 * when it want to open a connection it send it's connection information
 * for it to initialize the connection
 */

const wsServer = new websocket.server({httpServer});

const devices_path = "devices";
const clients_path = "clients";

const mapClients = {};
const mapDevices = {};

wsServer.on('request', (req) => {
  console.log(`WS  ${req.resource}`);

  const {path} = req.resourceURL;


  const splitted = path.split('/');
  splitted.shift();
  const type = splitted[0];
  const id = splitted[1];

  if (type !== devices_path && type !== clients_path) {
      return;
  }

  const conn = req.accept(null, req.origin);

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

        console.log(mapDevices);

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
const hostname = splitted.join(':') || '127.0.0.1';

httpServer.listen(port, hostname,
                  () => { console.log(`Server listening on ${hostname}:${port}`); });
