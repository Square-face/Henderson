import { useState } from 'react'
import { Slider } from './components/Slider'
import { CommandStructure, states } from './types.ts';
import './App.css'




function App() {
  
  const socket = new WebSocket("ws://192.168.1.192:8000/ws");

  socket.addEventListener("message", event => {
    console.log("Message from server ", event.data);
  });

  const [config, setConfig] = useState<CommandStructure>({
    state: states.STANDBY,
    Pk: 0.0,
    Ik: 0.0,
    Dk: 0.0,
    speed: 0,
    request: false,
  })

  return (
    <div className="appContainer">
      <div className='Misc'>
        <h2>Misc</h2>

      </div>
      <div className="PID">
        <h2>PID</h2>
        <Slider config={config} update={setConfig} range={{min:0, max:1}} field='Pk'/> 
        <Slider config={config} update={setConfig} range={{min:0, max:1}} field='Ik'/> 
        <Slider config={config} update={setConfig} range={{min:0, max:1}} field='Dk'/> 
      </div>
      <div className='Speed'>
        <h2>Speed</h2>
        <Slider config={config} update={setConfig} range={{min:0, max:255}} field='speed'/> 
      </div>
    </div>
  )
}
export default App
