let cols = 24; // por exemplo: 3 matrizes 8x8 alinhadas → 8x24
let rows = 8;
let cellSize = 30;

let leds = [];
let selectedColor;

function setup() {
  let canvas = createCanvas(cols * cellSize, rows * cellSize);
  canvas.parent('canvasContainer');
  selectedColor = color('#ff0000');

  // Inicializa matriz com tudo apagado
  for (let y = 0; y < rows; y++) {
    leds[y] = [];
    for (let x = 0; x < cols; x++) {
      leds[y][x] = color(0);
    }
  }

  // Lê a cor do picker
  let picker = document.getElementById('colorPicker');
  picker.addEventListener('input', () => {
    selectedColor = color(picker.value);
  });
}

function draw() {
  background(50);
  for (let y = 0; y < rows; y++) {
    for (let x = 0; x < cols; x++) {
      fill(leds[y][x]);
      stroke(100);
      rect(x * cellSize, y * cellSize, cellSize, cellSize);
    }
  }
}

function mousePressed() {
  let x = floor(mouseX / cellSize);
  let y = floor(mouseY / cellSize);

  if (x >= 0 && x < cols && y >= 0 && y < rows) {
    leds[y][x] = selectedColor;
  }
}
