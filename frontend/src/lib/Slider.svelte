<script lang="ts">
  export let max: number;
  export let value = 0;
  export let checked = false;
  export let text = "default text"
  export let update: Function = () => 0;
</script>

<div class="slider-container">
  <input type="number" max={max} step={(max < 10) ? 1 / 10000 : 1} class="slider-text" bind:value/>
  <input type="range" max={max} step={(max < 10) ? 1 / 10000 : 1} class="slider" bind:value on:change={()=>{update(value)}}>
  <input type="number" class="slider-text" bind:value={max}/>
  <label class="checkbox-container">{text}
    <input type="checkbox" bind:checked>
    <span class="checkmark"></span>
  </label>
</div>

<style>
  .slider{
    margin: 0;
    width: 15rem;
  }
  .slider-text {
    width: 100%;
    box-sizing: border-box;
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

  .slider-container {
    display: grid;
    grid-template-columns: 15rem 15rem 15rem auto;
    justify-items: flex-start;
    gap: .5rem;
  }

  /* Customize the label (the container) */
.checkbox-container {
  display: block;
  position: relative;
  padding-left: 35px;
  /* margin-bottom: 12px; */
  cursor: pointer;
  font-size: 22px;
  -webkit-user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
  user-select: none;
}

/* Hide the browser's default checkbox */
.checkbox-container input {
  position: absolute;
  opacity: 0;
  cursor: pointer;
  height: 0;
  width: 0;
}

/* Create a custom checkbox */
.checkmark {
  position: absolute;
  top: 0;
  left: 0;
  height: 25px;
  width: 25px;
  background-color: #eee;
}

/* On mouse-over, add a grey background color */
.checkbox-container:hover input ~ .checkmark {
  background-color: #ccc;
}

/* When the checkbox is checked, add a blue background */
.checkbox-container input:checked ~ .checkmark {
  background-color: cyan;
}

/* Create the checkmark/indicator (hidden when not checked) */
/* .checkmark:after {
  content: "";
  position: absolute;
  display: none;
}  */

/* Show the checkmark when checked */
/* .checkbox-container input:checked ~ .checkmark:after {
  display: block;
} */

/* Style the checkmark/indicator */
/* .checkbox-container .checkmark:after {
  left: 9px;
  top: 5px;
  width: 5px;
  height: 10px;
  border: solid white;
  border-width: 0 3px 3px 0;
  -webkit-transform: rotate(45deg);
  -ms-transform: rotate(45deg);
  transform: rotate(45deg);
} */
</style>