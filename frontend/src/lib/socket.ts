import { type Config, type ConfigMask , MaskOn, type Log, type StatusUpdate, State } from "./types";

const ws_url = "ws://192.168.1.192:8000/ws"

interface Handlers {
  log?: ((data: Log) => any) 
  status?: ((data: StatusUpdate) => any) 
}

// class for managing communication with the server
export class Socket {
  websocket: WebSocket;
  handlers: Handlers;

  constructor() {
    console.log(`Connecting to server at ${ws_url}`);
    this.websocket = new WebSocket(ws_url);
    this.handlers = {};

    this.websocket.addEventListener("error", (ev: Event) => {this.onError(ev)});
    this.websocket.addEventListener("open", (ev: Event) => {this.onOpen(ev)});
    this.websocket.addEventListener("message", (ev: MessageEvent) => {this.onMessage(ev)});
    this.websocket.addEventListener("close", (ev: Event) => {this.onClose(ev)});

  }

  onError(ev: Event) {
    console.error("Websocket error", ev)
  }

  onOpen(ev: Event) {
    this.requestConfig()
    console.log("Websocket connected", ev)
  }

  onMessage(ev: MessageEvent) {
    console.log("Recived message");
    let msg = ev.data;
    let data = JSON.parse(msg);

    if (this.handlers === undefined) { return; }

    switch (data.type) {
      case "log":
        if (this.handlers.log === undefined) { return; }

        this.handlers.log(data)
        
        break;

      case "status":
        if (this.handlers.status === undefined) { return; }
        
        let states = Object.values(State)

        if (data.state == 0) data.state = null
        else data.state = states.at(data.state-1)

        console.log(data);
        

        this.handlers.status(data)

      default:
        break;
    }
  }

  onClose(ev: Event) {
    console.log("Websocket disconnected", ev);
  }

  sendConfig(data: Config, mask: ConfigMask = MaskOn) {}

  requestConfig() {
    let data = {
      request_status: true
    }
    this.websocket.send(JSON.stringify(data))
  }
}
