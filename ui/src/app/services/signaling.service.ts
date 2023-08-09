import { Injectable } from '@angular/core';
import { ConfigService, Server } from './config.service';
import { BehaviorSubject } from 'rxjs';

export interface DeviceConnectionInfo {
  id: string;
  type: string;
  sdp: string;
}

@Injectable({
  providedIn: 'root'
})
export class SignalingService {

  private signalingServer: Server;
  private ws?: WebSocket;

  private serverDevices: { [id: string]: DeviceConnectionInfo } = {};

  signalingServerStatus = new BehaviorSubject(false);
  signalingServerDevices = new BehaviorSubject<DeviceConnectionInfo[]>([]);

  constructor(
    configService: ConfigService,
  ) {
    this.signalingServer = configService.getSignalingServer();
  }

  start(id: string) {
    this.ws = new WebSocket(this.signalingServer.url + "/clients/" + id);

    this.ws.onopen = () => {
      this.signalingServerStatus.next(true);
    };

    this.ws.onerror = (ev) => {
      console.error("error websocket signaling ", ev);
    };

    this.ws.onclose = () => {
      this.signalingServerStatus.next(false);
      this.start(id);
    };

    this.ws.onmessage = async (evt) => {
      if (typeof evt.data !== 'string') {
        console.error('receive none text ws message ', evt.data);
        return;
      }

      const message = JSON.parse(evt.data);

      if (message instanceof Array) {
        for (const item of message) {
          this.serverDevices[item.id] = item;
          this.signalingServerDevices.next(Object.values(this.serverDevices));
        }
        return;
      }

      if (message.type === "disconnect") {
        delete this.serverDevices[message.id];
      } else {
        if (this.serverDevices[message.id]) {
          delete this.serverDevices[message.id]
        }

        this.serverDevices[message.id] = message;
      }

      this.signalingServerDevices.next(Object.values(this.serverDevices));
    }
  }

  sendSdp(answer: any) {
    this.ws?.send(JSON.stringify(answer));
  }

}
