import { Component, ElementRef, ViewChild } from '@angular/core';
import { Router } from '@angular/router';
import { take } from 'rxjs';
import { PeerConnctionService } from 'src/app/services/peer-connction.service';
import { SignalingService } from 'src/app/services/signaling.service';

@Component({
  selector: 'app-devices-selection',
  templateUrl: './devices-selection.component.html',
  styleUrls: ['./devices-selection.component.scss']
})
export class DevicesSelectionComponent {

  @ViewChild('video') video?: ElementRef<HTMLVideoElement>;


  constructor(
    public router: Router,
    public signalingService: SignalingService,
    public peerConnectionService: PeerConnctionService,
  ) {}



  async onConnectClick(payload: any) {
    const message = await this.peerConnectionService.initializeConnection(payload);

    this.signalingService.sendSdp(message);


  }

}
