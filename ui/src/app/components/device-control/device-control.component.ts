import { AfterContentInit, AfterViewInit, Component, ElementRef, OnDestroy, OnInit, ViewChild } from '@angular/core';
import { Subscription, take } from 'rxjs';
import { FacesRecognitionEvent, MsgHeader } from 'src/app/proto';
import { ControlChannelService } from 'src/app/services/control-channel.service';
import { PeerConnctionService } from 'src/app/services/peer-connction.service';
import { SignalingService } from 'src/app/services/signaling.service';

@Component({
  selector: 'app-device-control',
  templateUrl: './device-control.component.html',
  styleUrls: ['./device-control.component.scss']
})
export class DeviceControlComponent implements AfterViewInit, OnDestroy {
  @ViewChild('video') video?: ElementRef<HTMLVideoElement>;
  @ViewChild('canvas') canvas?: ElementRef<HTMLCanvasElement>;

  subAnalysisVideo?: Subscription;


  latestEvent?: FacesRecognitionEvent;

  timeout?: any;

  constructor(
    public signalingService: SignalingService,
    public peerConnectionService: PeerConnctionService,
    public controlChannelService: ControlChannelService,
  ) { }

  ngAfterViewInit(): void {
    this.peerConnectionService.onTrack.asObservable().pipe(take(1)).subscribe(evt => {

      (this.video?.nativeElement as any).srcObject = evt?.streams[0];
      this.video?.nativeElement.play();


      const ctx = this.canvas?.nativeElement.getContext('2d');
      const sthis = this;

      this.video?.nativeElement.addEventListener('play', function () {
        if (this.paused || this.ended) {
          return;
        }

        if (sthis.canvas) {
          sthis.canvas.nativeElement.width = 640;
          sthis.canvas.nativeElement.height = 640;
        }


        const callback = () => {
          (sthis.canvas?.nativeElement as any).width = sthis.canvas?.nativeElement.width;
          if (!ctx) return;

          ctx.clearRect(0, 0, this.width, this.height);

          ctx.strokeStyle = 'red';
          ctx.lineWidth = 3;

          if (sthis.latestEvent) {

            for (const face of sthis.latestEvent.faces) {
              ctx.clearRect(0, 0, this.width, this.height); // Clear previous drawings
              ctx.strokeStyle = 'red'; // Set the color of the rectangle
              ctx.lineWidth = 3; // Set the width of the rectangle borde
              ctx.strokeRect(face.x, face.y, face.width, face.height);
            }

            sthis.latestEvent = undefined;
          }

          clearTimeout(sthis.timeout);
          sthis.timeout = setTimeout(() => {
            if (!sthis.latestEvent) {
              (sthis.canvas?.nativeElement as any).width = sthis.canvas?.nativeElement.width;
            }
          }, 500);
        };

        sthis.subAnalysisVideo = sthis.peerConnectionService.getDCMessage(MsgHeader.VIDEO_ANALYSIS_FACES, FacesRecognitionEvent.deserialize).subscribe((event) => {
          sthis.latestEvent = event;
          requestAnimationFrame(callback)
        });
      });

    });
  }

  ngOnDestroy(): void {
    this.subAnalysisVideo?.unsubscribe();
  }


}
