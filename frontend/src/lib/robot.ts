import { writable, type Writable } from 'svelte/store';
import { Socket } from './socket';
import { defaultConfig, MaskOff, type Config, type ConfigMask, type Log, type StatusUpdate } from './types';


/** 
 * Provide a store and some quickhands for managing a remote robot.
 * This class implements most of the feaures required to communicate with the server
*/
class Robot {
  
  // current config
  config: Writable<Config>;
  mask: Writable<ConfigMask>;

  // Buffer of n most recent logs. Should not exed the number defined by max_logs
  log_buffer: Writable<Log[]>;
  // max number of logs in the log buffer
  max_logs:number;


  // Socket object for communicating with the server
  private socket: Socket;


  constructor(max_logs:number = 100) {
    this.socket = new Socket();

    this.config = writable(defaultConfig);
    this.mask = writable(MaskOff);
    this.log_buffer = writable([]);

    this.max_logs = max_logs;


    // initialize handlers
    this.socket.handlers.log = (data: Log) => {
      // update the log buffer
      this.log_buffer.update((buffer: Log[]) => {

        buffer.unshift(data); // place new log at the start of the buffer
        return buffer.slice(0, 100); // only save the most recent logs

      });
    };

    this.socket.handlers.status = (data: StatusUpdate) => {
      // update the config
      this.config.update((prev: Config) => {
        return {
          ...prev,
          ...data
        };
      });
    };
  }
  
  /**
   * Get the raw config as a value.
   *
   * NOTE: It is adviced to use [Robot.subscribe] as it will update if the config changes
   * */
  getConfig(): Config {
    let current: Config = defaultConfig;

    this.config.subscribe((value: Config) => {
      current = value
    });

    return current
  }

  /**
   * Upload the local config to the robot
   * */
  write() {
    this.socket.sendConfig(this.getConfig());
  }
  

  /**
   * Request the robot to send its current config values.
   *
   * This method does not return anything by itself. Instead it will trigger a event when the server gets a response from the robot with the new values and will subsequently replace the saved local config with the one supplised by the server
   * */
  what_the_hell_are_you_doing(_: '?') {
    this.socket.requestConfig();
  }
}


export const Henderson = new Robot()
