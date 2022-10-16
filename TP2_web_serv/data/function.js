
const url = ws://${window.location.hostname}/ws;
const ws = new WebSocket(url);


const button = document.getElementById("button");
const led = document.getElementById("led");
const humidity = document.getElementById("humidity");
const temperature = document.getElementById("temperature");

function init() {
  led?.addEventListener("change", onToggle);
  ws.onclose = (() => alert("Veuillez recharger la page !"));
  ws.onopen = (() => ws.send("Client Ready!"));
  ws.onerror = ((event) => console.error(event));
  ws.onmessage = ((msg) => onMessage(msg));
}

function onMessage(event) {
  const status = JSON.parse(event.data);

 
  if (!button  !led  !humidity || !temperature) return;
  if (led instanceof HTMLInputElement) {
    led.checked = status.led;
  }

  button.innerHTML = status.button ? "ON" : "OFF";
  humidity.innerHTML = status.humidity ?? "--";
  temperature.innerHTML = status.temperature?.toString().substring(0, 4) ?? "--";
}

function onToggle() {
  ws.send("toggle");
}

window.onload = (() => init());