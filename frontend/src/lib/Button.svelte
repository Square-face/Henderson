<script>
  export const event = () => 0;
</script>

<button type="button" on:click={event}><slot /></button>

<style>
  button {
    box-sizing: border-box;
    width: 15rem;
    height: 3rem;
    padding: 16px;
    font-size: 16px;
    border: none;
    background-color: cyan;
    color: #242424;
  }

  button:hover {
    background-color: rgb(0, 228, 228);
  }
</style>