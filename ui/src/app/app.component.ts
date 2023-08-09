import { Component } from '@angular/core';
import { SignalingService } from './services/signaling.service';
import { PeerConnctionService } from './services/peer-connction.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  title = 'ui';

  constructor(
    public signalingService: SignalingService,
    public peerConnectionService: PeerConnctionService,
  ) {
    signalingService.start("client_id");

    signalingService.signalingServerDevices.asObservable().subscribe(() => peerConnectionService.start());
  }
}
