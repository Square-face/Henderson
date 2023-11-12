import { type Config, type ConfigMask , MaskOn } from "./types";

const ws_url = "ws://192.168.1.192:8000/ws"

// class for managing communication with the server
export class Socket {
  websocket: WebSocket;

  constructor() {
    console.log(`Connecting to server at ${ws_url}`);
    this.websocket = new WebSocket(ws_url);

    this.websocket.addEventListener("error", this.onError);
    this.websocket.addEventListener("open", this.onOpen);
    this.websocket.addEventListener("message", this.onMessage);
    this.websocket.addEventListener("close", this.onClose);
  }

  onError(this: WebSocket, ev: Event) {
    console.error("Websocket error", ev)
  }

  onOpen(this: WebSocket, ev: Event) {
    console.log("Websocket connected", ev)
  }

  onMessage(this: WebSocket, ev: Event) {
    console.log("Recived message", ev);
  }

  onClose(this: WebSocket, ev: Event) {
    console.log("Websocket disconnected", ev);
  }

  sendConfig(data: Config, mask: ConfigMask = MaskOn) {}

  requestConfig() {}
}
