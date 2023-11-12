<script lang="ts">
  import { State } from "./types";

  const stateArray = Object.values(State);

  let currentState = State.STANDBY;

  export let event = (state: State) => {};
</script>

<div class="dropdown">
  <button class="dropbtn">{currentState}</button>
  <div class="dropdown-content">
    {#each stateArray as state}
      <button type="button" class="state-button" on:click={() => {
        currentState = state;
        event(state);
      }}>{state}</button>
    {/each}
  </div>
</div>

<style>
  /* Dropdown Button */
  .dropbtn {
    background-color: cyan;
    color: black;
    padding: 16px;
    font-size: 16px;
    border: none;
    width: 15rem;
    height: 3rem;
  }

  /* The container <div> - needed to position the dropdown content */
  .dropdown {
    position: relative;
    display: inline-block;
  }

  /* Dropdown Content (Hidden by Default) */
  .dropdown-content {
    display: none;
    position: absolute;
    background-color: #f1f1f1;
    min-width: 160px;
    box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
    z-index: 1;
  }

  /* Links inside the dropdown */
  .dropdown-content button {
    background-color: cyan;
    color: black;
    padding: 12px 16px;
    text-decoration: none;
    display: block;
    border: none;
    width: 15rem;
  }

  /* Change color of dropdown links on hover */
  .dropdown-content button:hover {background-color: rgb(0, 228, 228);}

  /* Show the dropdown menu on hover */
  .dropdown:hover .dropdown-content {display: block;}

  /* Change the background color of the dropdown button when the dropdown content is shown */
  .dropdown:hover .dropbtn {background-color: rgb(0, 228, 228);}
</style>