<script lang="ts">
  import LogNumber from "./LogNumber.svelte";
  import type { Log } from "../types";
  export let log: Log;

  log = {type: 'log', cycles: 50, delta: 1, line: 50, p: 0.2, i: 0.4, d: 0.1, out: 128, left: 150, right: 150, sensors: [0, 0, 0, 0, 0, 0, 0, 0]}

</script>

<div class="log-number-container">
  {#each Object.entries(log) as [key, value]}
    <LogNumber text={key} value={value}/>
  {/each}
</div>

<style>
  .log-number-container {
    display: grid;
    grid-auto-flow: column;
    gap: 1rem;
  }
</style>