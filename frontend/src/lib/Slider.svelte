<script lang="ts">
  export let max: number;
  export let value = 0;
  export let checked = false;
  export let text = "default text"
  export const update: Function = () => 0;
</script>

<div>
  <input type="number" max={max} step={(max < 1) ? max / 10000 : 1} class="slider-text" bind:value/>
  <input type="range" max={max} step={(max < 1) ? max / 10000 : 1} bind:value on:change={()=>{update(value)}}>
  <input type="number" class="slider-text" bind:value={max}/>
  <div class="span-check-block">
    <span>{text}</span>
    <input type="checkbox" bind:checked/>
  </div>
</div>

<style>
  .slider-text {
    width: 10vw;
    appearance: none;
    border: none;
    border-bottom: 3px solid cyan;
    outline: none;
    border-radius: .5em .5em .1em .1em;
    background-color: #242424;
    color: #ccc;
    position: relative;
    bottom: 4px;
  }

  .span-check-block {
    width: 5vw;
    display: inline-flex;
    justify-content: space-between;
  }
</style>