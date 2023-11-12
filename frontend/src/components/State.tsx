import { ChangeEvent } from "react";
import { props, states, stateStrings, CommandStructure } from '../types.ts';

export function State({config, update}: props) {

  const handleUpdate = (event: ChangeEvent<HTMLInputElement>) => {
    const newValue = states[event.target.value]

    update((prevState: CommandStructure)=>{
      return {
        ...prevState,
        state: newValue
      }
    });
  }

  return (
    <div>
      { stateStrings.map((state: string) => {
        <input type="radio" name="state" id={state} value={state}/> {state}
      });}
    </div>
  );
}
