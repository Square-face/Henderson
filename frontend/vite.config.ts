import { defineConfig } from 'vite'
import { svelte } from '@sveltejs/vite-plugin-svelte'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [svelte()],
  ssr:{
    noExternal:['chart.js'],
  },
  server: {
    host: "127.0.0.1"
  }
});
