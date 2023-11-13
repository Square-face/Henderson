<script lang="ts">
  import svelteLogo from './assets/svelte.svg'
  import viteLogo from '/vite.svg'
  import Counter from './lib/Counter.svelte'
  import Slider from './lib/Slider.svelte';
  import StateDropdown from './lib/State-dropdown.svelte';
  import Button from './lib/Button.svelte';
  import LogNumber from './lib/LogNumber.svelte';
  import { Henderson } from './lib/robot';
  import type { Config, ConfigField, Log, State } from './lib/types';
  import { createEventDispatcher } from 'svelte';
  
  let config: Config;
  let logs: Log[];
  let PkToggle: boolean, IkToggle: boolean, DkToggle: boolean, SpeedToggle: boolean;

  Henderson.config.subscribe(value => {
    config = value;
  });

  // Henderson.log_buffer.subscribe(value => {
  //   logs = value;
  // })
  
  const updateState = (state: State) => {
    Henderson.config.update((previous: Config) => {
      return {
        ...previous,
        state: state
      }
    });
  }

  const updateConfig = (field: ConfigField) => {
    switch(field)
    {
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
              Ik: ik
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

      case 'speed':
        return (speed: number) => {
          Henderson.config.update((previous: Config) => {
            return {
              ...previous,
              speed: speed
            }
          });
        }
    }
  }

  const sendButton = () => {
    Henderson.write();
  }

  const requestButton = () => {
    Henderson.what_the_hell_are_you_doing('?');
  }


  // const stopButton = () => {
  //   Henderson.config.update
  // }
</script>

<main>
  <div class="appContainer">
    <div class="send-section">
      <h1>Misc</h1>
      <div class="misc">
        <StateDropdown event={updateState}/>
        <Button event={sendButton}>Send</Button>
        <Button event={requestButton}>Request</Button>
        <Button>Stop</Button>
      </div>
      <div class="PID">
        <h1>PID</h1>
        <Slider bind:checked={PkToggle} bind:value={config.Pk} text="P" max={0.02} update={updateConfig('Pk')}/>
        <Slider bind:checked={IkToggle} bind:value={config.Ik} text="I" max={0.02} update={updateConfig('Ik')}/>
        <Slider bind:checked={DkToggle} bind:value={config.Dk} text="D" max={0.02} update={updateConfig('Dk')}/>
      </div>
      <div class="speed">
        <h1>Speed</h1>
        <Slider bind:checked={SpeedToggle} bind:value={config.speed} text="Speed" max={255} update={updateConfig('speed')}/>
      </div>
    </div>
    <div class="log-section">
      <h1>Logs</h1>
      <div class="raw-log-numbers">
        <!-- {#each Object.entries(logs[0]) as logValue}
          <LogNumber />
        {:else}
          <LogNumber />
        {/each} -->
      </div>
    </div>
  </div>
</main>

<style>
  .misc {
    display: grid;
    grid-template: 1fr 1fr / 15rem 15rem;
    gap: 8px;
    align-items: center;
  }

</style>
