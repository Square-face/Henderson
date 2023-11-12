import { forwardRef, useRef, useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import { compileFunction } from 'vm';

interface commandValues {
  state: states;
  Pk: number;
  Ik: number;
  Dk: number;
  speed: number;
}

interface commandToggles {
  requestToggle: boolean;
  PkToggle: boolean;
  IkToggle: boolean;
  DkToggle: boolean;
  speedToggle: boolean;
}

interface commandStructure {
  state?: states;
  Pk?: string;
  Ik?: string;
  Dk?: string;
  speed?: string;
  request?: boolean;
}

enum states
{
  NOTHING,
  STANDBY,
  RUNNING,
  CALIBRATING,
  CALIBRATING_MANUAL
}

const stateStrings = ["Nothing", "Standby", "Running", "Calibrating", "Calibrating Manual"];

const socket = new WebSocket("ws://192.168.1.192:8000/ws");

// Connection opened
// socket.addEventListener("open", event => {
//   socket.send('{"state": "standby", "speed": "100"}');
// });

// Listen for messages

const constResolution = 10000;

function App() {
  const [data, setData]: [data: commandStructure, setData: React.Dispatch<React.SetStateAction<states>>] = useState({})
  
  socket.addEventListener("message", event => {
    setData(event.data);
    console.log("Message from server ", event.data);
  });

  const [state, setState]             = useState(states.NOTHING);
  // const [currentState, setCurrentState] = useState(state);
  const [Pk, setPk]                   = useState(0);
  const [PkToggle, setPkToggle]       = useState(false);
  const [Ik, setIk]                   = useState(0);
  const [IkToggle, setIkToggle]       = useState(false);
  const [Dk, setDk]                   = useState(0);
  const [DkToggle, setDkToggle]       = useState(false);
  const [speed, setSpeed]             = useState(255);
  const [speedToggle, setSpeedToggle] = useState(false);
  const [requestToggle, setRequest]   = useState(false);

  const values  = { state,         Pk,       Ik,       Dk,       speed }
  const toggled = { requestToggle, PkToggle, IkToggle, DkToggle, speedToggle } 

  return (
    <div className="appContainer">
      <div className="sliderBlockContainer">
        <Slider updateCallback={setPk} toggleCallback={setPkToggle} maxValue={1}/>
        <Slider updateCallback={setIk} toggleCallback={setIkToggle} maxValue={1}/>
        <Slider updateCallback={setDk} toggleCallback={setDkToggle} maxValue={1}/>
        <Slider updateCallback={setSpeed} toggleCallback={setSpeedToggle} maxValue={255}/>
      </div>
      <div className="buttonBlockContainer">
        <StateDropdown updateCallback={setState} current={stateStrings[state]} />
        <RequestToggle updateCallback={setRequest}/>
        <SendCommandButton valuesBundle={values} togglesBundle={toggled}/>
      </div>
    </div>
  )
}

const Slider = forwardRef(({
  updateCallback,
  toggleCallback,
  maxValue,
  resolution = constResolution
}: {
  updateCallback: Function,
  toggleCallback: Function,
  maxValue: number,
  resolution?: number
}) => {
  
  const [max, setMax] = useState(maxValue);
  const [value, setValue] = useState(0);

  const handleInput = (event: any) => {
    const output = (max) * (event.target.value / resolution);
    updateCallback((output > 1) ? Math.round(output) : output);
    setValue((output > 1) ? Math.round(output) : output);
  }

  const handleMax = (event: any) => {
    setMax(event.target.value);
  }

  const handleToggle = (event: any) => {
    toggleCallback(event.target.checked);
  }

  return (
    <div className="slidercontainer">
      <span className="slider-value">{(value > 1) ? Math.round(value) : value}</span>
      <input type="range" min="0" max={resolution} className="slider" onChange={handleInput}/>
      <input type="text" defaultValue={max} className="slider-bounds" onChange={handleMax}/>
      <input type="checkbox" onChange={handleToggle}/>
    </div>
  );
});

const SendCommandButton = ({ 
  valuesBundle,
  togglesBundle 
}: { 
  valuesBundle: commandValues,
  togglesBundle: commandToggles
}) => {
  const handleClick = () => {
    const { state, Pk, Ik, Dk, speed } = valuesBundle;
    const { requestToggle, PkToggle, IkToggle, DkToggle, speedToggle } = togglesBundle;

    let output: commandStructure = {};

    if (state != states.NOTHING) output.state = state;
    if (PkToggle) output.Pk = String(Pk);
    if (IkToggle) output.Ik = String(Ik);
    if (DkToggle) output.Dk = String(Dk);
    if (speedToggle) output.speed = String(speed);
    if (requestToggle) output.request = true;

    if (Object.keys(output).length > 0)
    {
      console.log(JSON.stringify(output));
      socket.send(JSON.stringify(output));
    } 
  }

  return (
    <>
      <button type='button' onClick={handleClick} className="send-button">Send</button>
    </>
  )
}

const RequestToggle = ({
  updateCallback
}: {
  updateCallback: Function
}) => {
  const handleChange = (event: any) => {
    updateCallback(event.target.checked);
  }

  return (
    <label className="switch">
      <input type="checkbox" onChange={handleChange}/>
      <span className="switch-slider round"></span>
    </label>
  );
}

const StateDropdown = ({ 
  current, 
  updateCallback 
} : { 
  current: string,
  updateCallback: Function
}) => {
  const handleClick = (state: states) => {
    return (event: any) => {
      updateCallback(state);
    }
  }

  return (
    <div className="dropdown">
      <button className="dropbtn">{current}</button>
      <div className="dropdown-content">
        {stateStrings.map((state, i) => {
          if (state !== current)
            return (
              <button key={state} className="state-button" onClick={handleClick(i)}>{state}</button>
            );
        })}
      </div>
    </div>
  );
}

export default App
