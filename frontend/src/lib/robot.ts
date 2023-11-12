import { writable, type Writable } from 'svelte/store';
import { Socket } from './socket';
import { type Config, defaultConfig } from './types';


class Robot {
  
  config: Writable<Config>;
  private socket: Socket;

  constructor() {
    this.socket = new Socket();
    this.config = writable(defaultConfig);
  }
  
  get $(){
    return this.config
  }

  /**
   * Get the raw config.
   *
   * NOTE: It is adviced to use [Robot.subscribe] as it will update if the config changes
   * */
  getConfig(): Config {
    let current: Config = defaultConfig;

    this.config.subscribe((value) => {
      current = value
    });

    return current
  }

  /**
   * Upload the local config to the robot
   *
   * Everything in thew 
   * */
  write() {
    this.socket.sendConfig(this.getConfig());
  }


  /**
   * Request the robot to send its current config values.
   *
   * This method does not return anything by itself. Instead it will trigger a event when the server gets a response from the robot with the new values and will subsequently replace the saved local config with the one supplised by the server
   * */
  read() {
    this.socket.requestConfig();
  }
}


export const Henderson = new Robot()
