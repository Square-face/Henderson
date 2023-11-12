<script lang="ts">
  import svelteLogo from './assets/svelte.svg'
  import viteLogo from '/vite.svg'
  import Counter from './lib/Counter.svelte'
  import Slider from './lib/Slider.svelte';

  let Pk = 0;
  let Ik = 0;
  let Dk = 0;  
  let speed = 0;

  function handleClick() {
    // Send the thing
  }
</script>

<main>
  <div class="appContainer">
    <div class="misc">
      <h1>Misc</h1>
      <span>Request</span>
      <input type="checkbox" />

      <button type="button" on:click={handleClick}>SEND</button>
    </div>
    <div class="PID">
      <h1>PID</h1>
      <Slider bind:value={Pk} text="P" max={0.02}/>
      <Slider bind:value={Ik} text="I" max={0.02}/>
      <Slider bind:value={Dk} text="D" max={0.02}/>
    </div>
    <div class="speed">
      <h1>Speed</h1>
      <Slider bind:value={speed} text="Speed" max={255}/>
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
