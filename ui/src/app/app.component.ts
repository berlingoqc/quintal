import { Component, OnInit } from '@angular/core';
import { SwUpdate, VersionReadyEvent } from '@angular/service-worker';
import { SignalingService } from './services/signaling.service';
import { PeerConnctionService } from './services/peer-connction.service';
import { filter, map } from 'rxjs';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {
  title = 'ui';
  isOnline: boolean = false;
  modalVersion: boolean = false;

  constructor(
    private swUpdate: SwUpdate,
    public signalingService: SignalingService,
    public peerConnectionService: PeerConnctionService,
  ) {
    signalingService.start("client_id");

    signalingService.signalingServerDevices.asObservable().subscribe(() => peerConnectionService.start());
  }

  public ngOnInit(): void {
    this.updateOnlineStatus();

    window.addEventListener('online',  this.updateOnlineStatus.bind(this));
    window.addEventListener('offline', this.updateOnlineStatus.bind(this));

    if (this.swUpdate.isEnabled) {
      this.swUpdate.versionUpdates.pipe(
        filter((evt: any): evt is VersionReadyEvent => evt.type === 'VERSION_READY'),
        map((evt: any) => {
          console.info(`currentVersion=[${evt.currentVersion} | latestVersion=[${evt.latestVersion}]`);
          this.modalVersion = true;
        }),
      );
    }
  }

  private updateOnlineStatus(): void {
    this.isOnline = window.navigator.onLine;
    console.info(`isOnline=[${this.isOnline}]`);
  }

  public updateVersion(): void {
    this.modalVersion = false;
    window.location.reload();
  }

  public closeVersion(): void {
    this.modalVersion = false;
  }

}
