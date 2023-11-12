<script lang="ts">
  import svelteLogo from './assets/svelte.svg'
  import viteLogo from '/vite.svg'
  import Counter from './lib/Counter.svelte'
  import Slider from './lib/Slider.svelte';
  import { Henderson } from './lib/robot';
  import type { Config, ConfigField, State } from './lib/types';
  
  let config: Config;

  Henderson.config.subscribe(value => {
    config = value;
  });
  
  const updateConfig = (field: ConfigField) => {
    switch(field)
    {
      case 'State':
        return (state: State) => {
          Henderson.config.update((previous: Config) => {
            return {
              ...previous,
              State: state
            }
          });
        }

      case 'Pk':
        return (pk: number) => {
          Henderson.config.update((previous: Config) => {
            return {
              ...previous,
              Pk: pk
            }
          });
        }

      case 'Ik':
        return (ik: number) => {
          Henderson.config.update((previous: Config) => {
            return {
              ...previous,
              Pk: ik
            }
          });
        }

      case 'Dk':
        return (dk: number) => {
          Henderson.config.update((previous: Config) => {
            return {
              ...previous,
              Dk: dk
            }
          });
        }

      case 'Speed':
        return (speed: number) => {
          Henderson.config.update((previous: Config) => {
            return {
              ...previous,
              Speed: speed
            }
          });
        }
    }
  }
</script>

<main>
  <div class="appContainer">
    <div class="send-section">
      <div class="misc">
        <h1>Misc</h1>
        <span>Request</span>
        <input type="checkbox" />
  
        <button type="button" >SEND</button>
      </div>
      <div class="PID">
        <h1>PID</h1>
        <Slider bind:value={config.Pk} text="P" max={0.02}/>
        <Slider bind:value={config.Ik} text="I" max={0.02}/>
        <Slider bind:value={config.Dk} text="D" max={0.02}/>
      </div>
      <div class="speed">
        <h1>Speed</h1>
        <Slider bind:value={config.Speed} text="Speed" max={255}/>
      </div>
    </div>
    <div class="recieve-section">

    </div>
  </div>
</main>

<style>
  button {
    margin-left: 10rem;
    height: 5rem;
    width: 10rem;
    background-color: cyan;
    color: #242424;
    border: none; 
  }
  button:hover{
    background-color: rgb(0, 228, 228);
  }
</style>
