import { ChangeEvent, useState } from 'react';
import { CommandStructure } from '../types.ts';

interface Range {
  min: number;
  max: number;
}

interface sliderProps {
  config: CommandStructure;
  update: (callback: (prevState: CommandStructure) => CommandStructure) => void;
  range: Range;
  field: string;
}

export function Slider({config, update, range, field}: sliderProps) {

  const handleInputChange = (event: ChangeEvent<HTMLInputElement>) => {
    const newValue = Number(event.target.value);
    update((prevState) => {
      return {
        ...prevState,
        [field]: newValue,
      };
    });
  };

  return (
    <div>
      <input type="number" min={range.min} max={range.max} onChange={handleInputChange } value={config[field]}/>
      <input type="range" min={range.min} max={range.max} onChange={handleInputChange } value={config[field]} step="0.01"/>
    </div>
  );
}
