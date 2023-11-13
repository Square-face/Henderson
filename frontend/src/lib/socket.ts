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

  /**
   * Event handler for socket errors.
   * Should not be triggered manualy and should instead be registered as an event listener for [websocket]
   *
   * ```
   * this.websocket.addEventListener("error", (ev: Event) => {this.onError(ev)});
   * ```
   * */
  onError(ev: Event) {
    console.error("Websocket error", ev)
  }


  /** 
   * Event handler for when the socket connection is opened
   * Will imediatly request an updated version of the local config to make sure the local data is up-to-date
   * Should not be triggered manualy and should instead be registered as an event listener for [websocket]
   * */
  onOpen(ev: Event) {
    this.requestConfig()
    console.log("Websocket connected", ev)
  }

  /**
   * Event handler for when the socket recives a message
   * Runs the coresponding handler for a message depending on what the messages type field is set to.
   * i.e if the message has its type as "log" the handler.log will be used
   *
   * Should not be triggered manualy and should instead be registered as an event listener for [websocket]
   * */
  onMessage(ev: MessageEvent) {

    // parse
    let msg = ev.data;
    let data = JSON.parse(msg);

    switch (data.type) {
      case "log":
        // use log handler
        if (this.handlers.log === undefined) { return; }

        this.handlers.log(data)
        break;

      case "status":
        // use status update handler
        if (this.handlers.status === undefined) { return; }


        // Replace state ordinal with enum value
        let states = Object.values(State)

        if (data.state == 0) data.state = null
        else data.state = states.at(data.state)

        this.handlers.status(data)
        break;

      default:
        break;
    }
  }

  /**
   * Event handler for websocket close events
   * 
   * Should not be triggered manualy and should instead be registered as an event listener for [websocket]
   * */
  onClose(ev: Event) {
    console.log("Websocket disconnected", ev);
  }

  /**
   * Send a config object to the host device.
   * A mask can be used to only send necesary.
   *
   * @param data The config holding the to change data
   * @param mask A mask of booleans indicating wich data fields to care about
   * */
  sendConfig(data: Config, mask: ConfigMask = MaskOn) {
    let msg = {};
    
    
    // apply mask
    if (mask.state) msg["state"] = data.state

    if (mask.Pk) msg["state"] = data.Pk
    if (mask.Ik) msg["state"] = data.Ik
    if (mask.Dk) msg["state"] = data.Dk

    if (mask.speed) msg["state"] = data.speed

    this.websocket.send(JSON.stringify(msg))
  }

  /**
   * Send a request to the server to make it pool the robot its stored config and return it.
   * This function does not return the new config, it is instead handled by the message reciver wich dispatches an event to the status handler when it resives a status update.
   * */
  requestConfig() {
    let data = {
      request_status: true
    }
    this.websocket.send(JSON.stringify(data))
  }
}
