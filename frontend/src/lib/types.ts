
// All possible states
export enum State {
  STANDBY = "Standby",
  RUNNING = "Running",
  RUNNING_LOGGED = "Running Logged",
  CALIBRATING = "Calibrating",
  CALIBRATING_MANUAL = "Manual calibration",
}

// interface for the data type holding all values that can be sent to the server to modify Henderson
export interface Config {
  state: State,
  Pk: number,
  Ik: number,
  Dk: number,
  speed: number
}

export interface ConfigMask {
  state: boolean,
  Pk: boolean,
  Ik: boolean,
  Dk: boolean,
  speed: boolean,
}

export type ConfigField = 'state' | 'Pk' | 'Ik' | 'Dk' | 'speed';


export interface StatusUpdate {
  type: 'status'
  state: State,
  Pk?: number,
  Ik?: number,
  Dk?: number,
  speed?: number,
}

export interface Log {
  type: 'log'
  cycles: number,
  delta: number,
  line: number,
  p: number,
  i: number,
  d: number,
  out: number,
  left: number,
  right: number,
  sensors: number[],
}





// defaults

export const defaultConfig: Config = {
  state: State.STANDBY,
  Pk: 0,
  Ik: 0,
  Dk: 0,
  speed: 0,
}

export const MaskOn: ConfigMask = {
  state: true,
  Pk: true,
  Ik: true,
  Dk: true,
  speed: true,
}

export const MaskOff: ConfigMask = {
  state: false,
  Pk: false,
  Ik: false,
  Dk: false,
  speed: false,
}
