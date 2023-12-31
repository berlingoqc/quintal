/**
 * Generated by the protoc-gen-ts.  DO NOT EDIT!
 * compiler version: 4.23.4
 * source: video_analysis.proto
 * git: https://github.com/thesayyn/protoc-gen-ts */
import * as pb_1 from "google-protobuf";
export class Rect extends pb_1.Message {
    #one_of_decls: number[][] = [];
    constructor(data?: any[] | {
        x?: number;
        y?: number;
        width?: number;
        height?: number;
    }) {
        super();
        pb_1.Message.initialize(this, Array.isArray(data) ? data : [], 0, -1, [], this.#one_of_decls);
        if (!Array.isArray(data) && typeof data == "object") {
            if ("x" in data && data.x != undefined) {
                this.x = data.x;
            }
            if ("y" in data && data.y != undefined) {
                this.y = data.y;
            }
            if ("width" in data && data.width != undefined) {
                this.width = data.width;
            }
            if ("height" in data && data.height != undefined) {
                this.height = data.height;
            }
        }
    }
    get x() {
        return pb_1.Message.getFieldWithDefault(this, 1, 0) as number;
    }
    set x(value: number) {
        pb_1.Message.setField(this, 1, value);
    }
    get y() {
        return pb_1.Message.getFieldWithDefault(this, 2, 0) as number;
    }
    set y(value: number) {
        pb_1.Message.setField(this, 2, value);
    }
    get width() {
        return pb_1.Message.getFieldWithDefault(this, 3, 0) as number;
    }
    set width(value: number) {
        pb_1.Message.setField(this, 3, value);
    }
    get height() {
        return pb_1.Message.getFieldWithDefault(this, 4, 0) as number;
    }
    set height(value: number) {
        pb_1.Message.setField(this, 4, value);
    }
    static fromObject(data: {
        x?: number;
        y?: number;
        width?: number;
        height?: number;
    }): Rect {
        const message = new Rect({});
        if (data.x != null) {
            message.x = data.x;
        }
        if (data.y != null) {
            message.y = data.y;
        }
        if (data.width != null) {
            message.width = data.width;
        }
        if (data.height != null) {
            message.height = data.height;
        }
        return message;
    }
    toObject() {
        const data: {
            x?: number;
            y?: number;
            width?: number;
            height?: number;
        } = {};
        if (this.x != null) {
            data.x = this.x;
        }
        if (this.y != null) {
            data.y = this.y;
        }
        if (this.width != null) {
            data.width = this.width;
        }
        if (this.height != null) {
            data.height = this.height;
        }
        return data;
    }
    serialize(): Uint8Array;
    serialize(w: pb_1.BinaryWriter): void;
    serialize(w?: pb_1.BinaryWriter): Uint8Array | void {
        const writer = w || new pb_1.BinaryWriter();
        if (this.x != 0)
            writer.writeInt32(1, this.x);
        if (this.y != 0)
            writer.writeInt32(2, this.y);
        if (this.width != 0)
            writer.writeInt32(3, this.width);
        if (this.height != 0)
            writer.writeInt32(4, this.height);
        if (!w)
            return writer.getResultBuffer();
    }
    static deserialize(bytes: Uint8Array | pb_1.BinaryReader): Rect {
        const reader = bytes instanceof pb_1.BinaryReader ? bytes : new pb_1.BinaryReader(bytes), message = new Rect();
        while (reader.nextField()) {
            if (reader.isEndGroup())
                break;
            switch (reader.getFieldNumber()) {
                case 1:
                    message.x = reader.readInt32();
                    break;
                case 2:
                    message.y = reader.readInt32();
                    break;
                case 3:
                    message.width = reader.readInt32();
                    break;
                case 4:
                    message.height = reader.readInt32();
                    break;
                default: reader.skipField();
            }
        }
        return message;
    }
    serializeBinary(): Uint8Array {
        return this.serialize();
    }
    static override deserializeBinary(bytes: Uint8Array): Rect {
        return Rect.deserialize(bytes);
    }
}
export class FacesRecognitionEvent extends pb_1.Message {
    #one_of_decls: number[][] = [];
    constructor(data?: any[] | {
        faces?: Rect[];
    }) {
        super();
        pb_1.Message.initialize(this, Array.isArray(data) ? data : [], 0, -1, [1], this.#one_of_decls);
        if (!Array.isArray(data) && typeof data == "object") {
            if ("faces" in data && data.faces != undefined) {
                this.faces = data.faces;
            }
        }
    }
    get faces() {
        return pb_1.Message.getRepeatedWrapperField(this, Rect, 1) as Rect[];
    }
    set faces(value: Rect[]) {
        pb_1.Message.setRepeatedWrapperField(this, 1, value);
    }
    static fromObject(data: {
        faces?: ReturnType<typeof Rect.prototype.toObject>[];
    }): FacesRecognitionEvent {
        const message = new FacesRecognitionEvent({});
        if (data.faces != null) {
            message.faces = data.faces.map(item => Rect.fromObject(item));
        }
        return message;
    }
    toObject() {
        const data: {
            faces?: ReturnType<typeof Rect.prototype.toObject>[];
        } = {};
        if (this.faces != null) {
            data.faces = this.faces.map((item: Rect) => item.toObject());
        }
        return data;
    }
    serialize(): Uint8Array;
    serialize(w: pb_1.BinaryWriter): void;
    serialize(w?: pb_1.BinaryWriter): Uint8Array | void {
        const writer = w || new pb_1.BinaryWriter();
        if (this.faces.length)
            writer.writeRepeatedMessage(1, this.faces, (item: Rect) => item.serialize(writer));
        if (!w)
            return writer.getResultBuffer();
    }
    static deserialize(bytes: Uint8Array | pb_1.BinaryReader): FacesRecognitionEvent {
        const reader = bytes instanceof pb_1.BinaryReader ? bytes : new pb_1.BinaryReader(bytes), message = new FacesRecognitionEvent();
        while (reader.nextField()) {
            if (reader.isEndGroup())
                break;
            switch (reader.getFieldNumber()) {
                case 1:
                    reader.readMessage(message.faces, () => pb_1.Message.addToRepeatedWrapperField(message, 1, Rect.deserialize(reader), Rect));
                    break;
                default: reader.skipField();
            }
        }
        return message;
    }
    serializeBinary(): Uint8Array {
        return this.serialize();
    }
    static override deserializeBinary(bytes: Uint8Array): FacesRecognitionEvent {
        return FacesRecognitionEvent.deserialize(bytes);
    }
}
