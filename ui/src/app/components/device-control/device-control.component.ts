import { AfterContentInit, AfterViewInit, Component, ElementRef, OnInit, ViewChild } from '@angular/core';
import { take } from 'rxjs';
import { PeerConnctionService } from 'src/app/services/peer-connction.service';
import { SignalingService } from 'src/app/services/signaling.service';

@Component({
  selector: 'app-device-control',
  templateUrl: './device-control.component.html',
  styleUrls: ['./device-control.component.scss']
})
export class DeviceControlComponent implements AfterViewInit {
  @ViewChild('video') video?: ElementRef<HTMLVideoElement>;
  @ViewChild('canvas') canvas?: ElementRef<HTMLCanvasElement>;


  constructor(
    public signalingService: SignalingService,
    public peerConnectionService: PeerConnctionService,
  ) {}

  ngAfterViewInit(): void {
    this.peerConnectionService.onTrack.asObservable().pipe(take(1)).subscribe(evt => {

      (this.video?.nativeElement as any).srcObject = evt?.streams[0];
      this.video?.nativeElement.play();


      const ctx = this.canvas?.nativeElement.getContext('2d');
      const sthis = this;

      this.video?.nativeElement.addEventListener('play', function() {
        if (this.paused || this.ended) {
          return;
        }

        if (sthis.canvas) {
          sthis.canvas.nativeElement.width = 640;
          sthis.canvas.nativeElement.height = 640;
        }


        const callback = () => {
          ctx?.clearRect(0, 0, this.width, this.height);

          // Draw your 2D shapes. Here's an example rectangle:
          (ctx as any).fillStyle = 'red';
          ctx?.fillRect(50, 50, 100, 100);

          console.log('draw of canvas');

          // Continue drawing loop
          requestAnimationFrame(callback);
        };

        requestAnimationFrame(callback);
        
      });

    });
  }


}
