import { Injectable } from '@angular/core';


export interface Server {
  url: string
}

@Injectable({
  providedIn: 'root'
})
export class ConfigService {

  constructor() { }

  getSignalingServer(): Server {
    return {
      url: "ws://localhost:8000"
    };
  }
}
