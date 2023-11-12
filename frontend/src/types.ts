export interface CommandStructure {
  state: number;
  Pk: number;
  Ik: number;
  Dk: number;
  speed: number;
  request: boolean;
}


export interface props {
  config: CommandStructure;
  update: (callback: (prevState: CommandStructure) => CommandStructure) => void;
}


export enum states
{
  NOTHING,
  STANDBY,
  RUNNING,
  CALIBRATING,
  CALIBRATING_MANUAL
}

export const stateStrings = [ "standby", "running", "calibrating", "calibrating_manual" ];

