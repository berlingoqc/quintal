import { Injectable } from '@angular/core';


export interface Server {
  url: string
}

@Injectable({
  providedIn: 'root'
})
export class ConfigService {


  private urlSignaling = "";

  constructor() {
    const hostname = window.location.hostname;
    if (hostname === "localhost") {
      this.urlSignaling = "ws://localhost:8000"
    } else {
      this.urlSignaling = "wss://signaling.cars.growbe.ca";
    }
  }

  getSignalingServer(): Server {
    return {
      url: this.urlSignaling 
    };
  }
}
