import { Injectable } from "@angular/core";
import { PeerConnctionService } from "./peer-connction.service";

import * as pb_1 from "google-protobuf";

import {
    MsgHeader,
    ControlEvent,
} from 'src/app/proto';

@Injectable({
  providedIn: 'root'
})
export class ControlChannelService {

    keystate = {
        front: false,
        back: false,
        left: false,
        right: false,
    
        hasChange: false
    };

    get isReady(): boolean {
        return !(!this.peerConnection.controlDataChannel);
    }

    constructor(
        private peerConnection: PeerConnctionService
    ) {
        document.addEventListener("keydown", (event) => this.onKeyDown(event));
        document.addEventListener("keyup", (event) => this.onKeyUp(event));
    }

    private onKeyStateUpdate() {
        if (!this.peerConnection.connectionStatus || !this.keystate.hasChange) return;

        const controlEvent = new ControlEvent();
        if (this.keystate.front) {
            controlEvent.y = 100;
        } else if (this.keystate.back) {
            controlEvent.y = -100;
        } 

        if (this.keystate.left) {
            controlEvent.x = 100;
        } else if (this.keystate.right) {
            controlEvent.x = -100;
        }

        controlEvent.throtle = 100;

        this.sendMsg(MsgHeader.CONTROL_EVENT, controlEvent);

        this.keystate.hasChange = false;

    }

    private onKeyDown(event: KeyboardEvent) {
        var keyCode = event.keyCode || event.which;

        // Check which key was pressed
        switch (keyCode) {
            case 87: // W key
                if (this.keystate.front != true) {
                    this.keystate.front = true;
                    this.keystate.hasChange = true;
                }
                break;
            case 65: // A key
                if (this.keystate.left != true) {
                    this.keystate.left = true;
                    this.keystate.hasChange = true;
                }
                break;
            case 83: // S key
                if (this.keystate.back != true) {
                    this.keystate.back = true;
                    this.keystate.hasChange = true;
                }
                break;
            case 68: // D key
                if (this.keystate.right != true) {
                    this.keystate.right = true;
                    this.keystate.hasChange = true;
                }
                break;
            default:
                // Ignore other keys
                break;
        }

        this.onKeyStateUpdate();
    }

    private onKeyUp(event: KeyboardEvent) {
        var keyCode = event.keyCode || event.which;

        // Check which key was released
        switch (keyCode) {
            case 87: // W key
                if (this.keystate.front != false) {
                    this.keystate.front = false;
                    this.keystate.hasChange = true;
                }
                break;
            case 65: // A key
                if (this.keystate.left != false) {
                    this.keystate.left = false;
                    this.keystate.hasChange = true;
                }
                break;
            case 83: // S key
                if (this.keystate.back != false) {
                    this.keystate.back = false;
                    this.keystate.hasChange = true;
                }
                break;
            case 68: // D key
                if (this.keystate.right != false) {
                    this.keystate.right = false;
                    this.keystate.hasChange = true;
                }
                break;
            default:
                // Ignore other keys
                break;
        }

        this.onKeyStateUpdate();
    }

    private sendMsg(header: MsgHeader, payload: pb_1.Message) {

        console.log('SENDING MSG ' + header + ' ' + JSON.stringify(payload));
        let arr = payload.serializeBinary();
        let newArray = new Uint8Array(arr.length + 1);
        newArray[0] = header.valueOf();
        newArray.set(arr, 1);

        this.peerConnection.controlDataChannel?.send(newArray);
    }

}