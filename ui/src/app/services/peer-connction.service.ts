import { Injectable } from '@angular/core';
import { BehaviorSubject, Subject, of } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class PeerConnctionService {

  private pc?: RTCPeerConnection;
  private dc?: RTCDataChannel

  iceStatus = new BehaviorSubject<RTCSessionDescription | null>(null);
  onTrack = new BehaviorSubject<RTCTrackEvent | null>(null);
  connectionStatus = new BehaviorSubject<boolean>(false);

  get controlDataChannel(): RTCDataChannel | undefined {
    return this.dc;
  }

  constructor() {
    this.start();

    window.addEventListener('close', () => {
      if (this.pc) {
        this.pc.close();
      }
    });

    window.addEventListener('onbeforeunload', () => {
      if (this.pc) {
        this.pc.close();
      }
    }) 
  }

  start() {


    if (this.pc) {
      this.pc.close();
    }

    this.connectionStatus.next(false);
    this.iceStatus.next(null);
    this.onTrack.next(null);

    this.pc = new RTCPeerConnection({
      bundlePolicy: 'max-bundle',
      iceServers: [
        {
          urls: "stun:stun.l.google.com:19302"
        },
        {
          urls: "turn:turn.cars.growbe.ca:3478",
          username: "username1",
          credential: "key1"
        },
        /*
        {
          urls: "turn:a.relay.metered.ca:80",
          username: "1d5df4140ad2e51e190d2cc4",
          credential: "BggnkQ7ytzN7zepr",
        },
        {
          urls: "turn:a.relay.metered.ca:443",
          username: "1d5df4140ad2e51e190d2cc4",
          credential: "BggnkQ7ytzN7zepr",
        },@
        {
          urls: "turn:a.relay.metered.ca:443?transport=tcp",
          username: "1d5df4140ad2e51e190d2cc4",
          credential: "BggnkQ7ytzN7zepr",
        },*/
      ]
    });

    this.pc.ondatachannel = (evt) => {

      console.log('on data channel');

      this.dc = evt.channel;

      this.dc.onopen = () => {
        console.log('is open');
      };

      this.dc.onmessage = (evt) => {
        console.log('EVENT MESSAGE ', evt);
      }

      this.dc.onclose = () => {
        console.log('close');
      };

    };

    this.pc.onicegatheringstatechange = (state) => {
      if (this.pc?.iceGatheringState === "complete") {
        this.iceStatus.next(this.pc.localDescription);
      }
    };

    this.pc.onconnectionstatechange = () => {
      const connectionStatus = this.pc?.connectionState;
      if (["disconnected", "failed", "closed"].includes(connectionStatus || '')) {
        console.log("disconnected");
      }
    }; 

    this.pc.ontrack = (evt) => {
      this.connectionStatus.next(true);
      this.onTrack.next(evt);
    };

  }

  async initializeConnection(offer: RTCSessionDescriptionInit) {
    await this.pc?.setRemoteDescription(offer);

    await this.pc?.setLocalDescription(await this.pc.createAnswer());

    await this.waitGatheringComplete();

    const answer = this.pc?.localDescription;

    return {
      id: (offer as any).id,
      type: answer?.type,
      sdp: answer?.sdp,
    };
  }


  async waitGatheringComplete(): Promise<void> {
    return new Promise((resolve) => {
      console.log('RECEIVE STATE ', this.pc?.iceGatheringState);
      if (this.pc?.iceGatheringState === 'complete') {
        resolve();
      } else {
        this.pc?.addEventListener('icegatheringstatechange', () => {
          console.log('RECEIVE STATE CHANGE ', this.pc?.iceGatheringState);
          if (this.pc?.iceGatheringState === 'complete') {
            resolve();
          }
        });
      }
    });
  }
}
