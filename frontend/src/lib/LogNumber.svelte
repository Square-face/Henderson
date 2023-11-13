<script lang="ts">
  export let text: string = "default text";
  export let value: any = 0;
</script>

<div class="item">
  <div class="key-text">{text}</div>
  <div>{value}</div>
</div>

<style>
  .item {
    width: inherit;
    background: cyan;
    color: #242424;
    text-align: center;
    border-radius: 1rem;
  }

  .key-text {
    border-bottom: 2px solid #242424;
  }
</style>