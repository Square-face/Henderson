
// All possible states
export enum State {
  STANDBY = "Standby",
  RUNNING = "Running",
  CALIBRATING = "Calibrating",
  CALIBRATING_MANUAL = "Manual calibration",
}

// interface for the data type holding all values that can be sent to the server to modify Henderson
export interface Config {
  State: State,
  Pk: number,
  Ik: number,
  Dk: number,
  Speed: number
}

export interface ConfigMask {
  State: boolean,
  Pk: boolean,
  Ik: boolean,
  Dk: boolean,
  Speed: boolean,
}

export type ConfigField = 'State' | 'Pk' | 'Ik' | 'Dk' | 'Speed';


export interface StatusUpdate {
  type: 'status'
  state: State | null,
  p?: number,
  i?: number,
  d?: number,
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
  State: State.STANDBY,
  Pk: 0,
  Ik: 0,
  Dk: 0,
  Speed: 0,
}

export const MaskOn: ConfigMask = {
  State: true,
  Pk: true,
  Ik: true,
  Dk: true,
  Speed: true,
}
