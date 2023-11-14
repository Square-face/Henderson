<script lang="ts">
  import { type Socket } from '../socket';

  export let socket: Socket;
</script>

<div class="socket">
  <span class="socket url">{ socket.websocket.url }</span>
  <span class="socket state">{ socket.websocket.readyState }</span>

  <button class="socket button" on:click={socket.disconnect}>Disconnect</button>
  
</div>

<style>
  .socket {
    display: grid;
    gap: 1rem;
    grid-template-columns: 1fr 1fr;
    grid-template-rows: 1fr 1fr;
    background: 0#303030;
  }

  .socket > * {
    background: 0#505050;
    width: 100%;
  }

  .socket .url {
    grid-row: 1;
    grid-column-start: 1;
    grid-column-end: 3;
  }

  .socket .state {
    grid-row: 2;
  }
  .socket .button {
    grid-row: 2;
  }

</style>
