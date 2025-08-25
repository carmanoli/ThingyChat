// =================================================================
//  ThingyChat Animation Editor - editor.js (Versão Completa)
// =================================================================

if (!window.AnimationEditor) {
  window.AnimationEditor = {
    // --- ESTADO DA APLICAÇÃO ---
    state: {
      name: "My Animation",
      gridSize: 8,
      loop: true,
      direction: "forward",
      frames: [],
      activeFrameIndex: 0,
      currentColor: "#FF0000",
      isPlaying: false,
      playbackTimeout: null,
      isDrawing: false,
      activeTool: "draw", // <-- NOVA PROPRIEDADE: 'draw' ou 'fill'
    },

    // --- FUNÇÕES AUXILIARES ---
    _createEmptyFrame: function (size) {
      return {
        duration: 100,
        brightness: 100,
        pixels: Array(size)
          .fill()
          .map(() => Array(size).fill(null)), // 'null' para transparente
      };
    },

    // --- GESTÃO DE ESTADO ---
    loadState: function () {
      const savedState = sessionStorage.getItem("animationEditorState");
      if (savedState) {
        Object.assign(this.state, JSON.parse(savedState));
      }
    },
    saveState: function () {
      // Não guarda o estado de playback, que é temporário
      const stateToSave = {
        ...this.state,
        isPlaying: false,
        playbackTimeout: null,
        isDrawing: false,
      };
      sessionStorage.setItem(
        "animationEditorState",
        JSON.stringify(stateToSave)
      );
    },

    // --- INICIALIZAÇÃO ---
    init: function () {
      this.cacheDOMElements();
      this.addEventListeners();
      if (this.state.frames.length === 0) {
        this.addNewFrame();
      } else {
        this.selectFrame(this.state.activeFrameIndex);
      }
      this.renderAll();
      this.checkPendingPhoto();
    },

    cacheDOMElements: function () {
      this.dom = {
        grid: document.getElementById("pixel-grid"),
        palette: document.getElementById("color-palette"),
        timeline: document.getElementById("timeline-frames"),
        animationNameInput: document.getElementById("animationName"),
        gridSizeRadios: document.querySelectorAll('input[name="gridSize"]'),
        addFrameBtn: document.getElementById("add-frame-btn"),
        cloneFrameBtn: document.getElementById("clone-frame-btn"),
        playBtn: document.getElementById("play-animation-btn"),
        clearBtn: document.getElementById("clear-btn"),
        copyBtn: document.getElementById("copy-btn"),
        saveBtn: document.getElementById("save-btn"),
        loadBtn: document.getElementById("load-btn"),
        codeOutput: document.getElementById("code-output"),
        jsonOutput: document.getElementById("json-output"),
        loopCheckbox: document.getElementById("loop-checkbox"),
        directionSelect: document.getElementById("direction-select"),

        confirmExportBtn: document.getElementById("confirm-export-btn"),
        confirmSaveBtn: document.getElementById("confirm-save-btn"),
        cancelSaveBtn: document.getElementById("cancel-save-btn"),
        saveForm: document.getElementById("save-library-form"),
        drawToolBtn: document.getElementById("draw-tool-btn"), // <-- NOVO
        fillToolBtn: document.getElementById("fill-tool-btn"),
        newAnimationBtn: document.getElementById("new-animation-btn"),
        pastePhotoBtn: document.getElementById("paste-photo-btn"),
      };
    },

    addEventListeners: function () {
      this.cacheDOMElements();
      this.dom.gridSizeRadios.forEach((radio) =>
        radio.addEventListener("change", (e) =>
          this.changeGridSize(parseInt(e.target.value))
        )
      );
      this.dom.animationNameInput.addEventListener("input", (e) => {
        this.state.name = e.target.value;
        this.updateOutputs();
        this.saveState();
      });
      this.dom.addFrameBtn.addEventListener("click", () => this.addNewFrame());
      this.dom.cloneFrameBtn.addEventListener("click", () =>
        this.addNewFrame(true)
      );
      this.dom.playBtn.addEventListener("click", () => this.togglePlayback());
      this.dom.clearBtn.addEventListener("click", () => this.clearFrame());
      this.dom.copyBtn.addEventListener("click", () => this.copyCode());
      this.dom.saveBtn.addEventListener("click", () => this.handleSave());
      this.dom.loadBtn.addEventListener("click", () => this.loadFromJson());
      this.dom.loopCheckbox.addEventListener("change", (e) => {
        this.state.loop = e.target.checked;
        this.updateOutputs();
        this.saveState();
      });
      this.dom.directionSelect.addEventListener("change", (e) => {
        this.state.direction = e.target.value;
        this.updateOutputs();
        this.saveState();
      });
      window.addEventListener("mouseup", () => {
        this.state.isDrawing = false;
      });
      if (this.dom.confirmSaveBtn) {
        this.dom.confirmSaveBtn.addEventListener("click", () =>
          this._executeSaveToLibrary()
        );
      }
      if (this.dom.cancelSaveBtn) {
        this.dom.cancelSaveBtn.addEventListener("click", () =>
          this.dom.saveForm.classList.add("d-none")
        );
      }
      if (this.dom.confirmExportBtn) {
        this.dom.confirmExportBtn.addEventListener("click", () =>
          this.handleExport()
        );
      }
      if (this.dom.drawToolBtn) {
        this.dom.drawToolBtn.addEventListener("click", () =>
          this.setActiveTool("draw")
        );
      }
      if (this.dom.fillToolBtn) {
        this.dom.fillToolBtn.addEventListener("click", () =>
          this.setActiveTool("fill")
        );
      }
      if (this.dom.newAnimationBtn) {
        this.dom.newAnimationBtn.addEventListener("click", () =>
          this.createNewAnimation()
        );
      }
      // Adicionar listener para o botão Paste Photo
      if (this.dom.pastePhotoBtn) {
        this.dom.pastePhotoBtn.addEventListener("click", () => {
          console.log("[Editor] Botão Paste Photo clicado");
          this.pastePhoto();
        });
      }
      window.addEventListener("photoready", () => {
        this.checkPendingPhoto();
      });
    },


    // --- FUNÇÕES DE RENDERIZAÇÃO ---
    renderAll: function () {
      this.dom.animationNameInput.value = this.state.name;
      document.querySelector(
        `input[name="gridSize"][value="${this.state.gridSize}"]`
      ).checked = true;
      this.dom.loopCheckbox.checked = this.state.loop;
      this.dom.directionSelect.value = this.state.direction;
      this.renderGrid();
      this.renderPalette();
      this.renderTimeline();

      this.updateOutputs();
    },

    renderGrid: function () {
      const frame = this.state.frames[this.state.activeFrameIndex];
      if (!frame) return;

      this.dom.grid.innerHTML = "";
      this.dom.grid.style.gridTemplateColumns = `repeat(${this.state.gridSize}, 1fr)`;

      // Função de pintura otimizada para usar a classe CSS
      const paintPixel = (pixelElement, x, y) => {
        if (frame.pixels[y][x] !== this.state.currentColor) {
          frame.pixels[y][x] = this.state.currentColor;

          // ALTERAÇÃO AQUI: Em vez de 'style.background', usamos 'backgroundColor' e a classe
          pixelElement.style.backgroundColor = this.state.currentColor; // Define a cor sólida
          // A função toggle() adiciona a classe se o segundo argumento for 'true', e remove se for 'false'.
          pixelElement.classList.toggle(
            "transparent-bg",
            this.state.currentColor === null
          );

          this.renderTimeline();
          this.updateOutputs();
          this.saveState();
        }
      };

      for (let y = 0; y < this.state.gridSize; y++) {
        for (let x = 0; x < this.state.gridSize; x++) {
          const pixel = document.createElement("div");
          pixel.className = "pixel";

          // ALTERAÇÃO AQUI: Lógica de inicialização para usar a classe CSS
          pixel.style.backgroundColor = frame.pixels[y][x];
          if (frame.pixels[y][x] === null) {
            pixel.classList.add("transparent-bg");
          }

          // O mousedown agora verifica qual a ferramenta ativa
          pixel.addEventListener("mousedown", (e) => {
            e.preventDefault();

            if (this.state.activeTool === "draw") {
              this.state.isDrawing = true;
              paintPixel(pixel, x, y);
            } else if (this.state.activeTool === "fill") {
              this.floodFill(x, y);
            }
          });
          pixel.addEventListener("mouseover", () => {
            if (this.state.isDrawing) {
              paintPixel(pixel, x, y);
            }
          });
          this.dom.grid.appendChild(pixel);
        }
      }
    },
    renderPalette: function () {
      this.dom.palette.innerHTML = "";
      this.colors.forEach((color) => {
        const colorBtn = document.createElement("div");
        colorBtn.className = "color-option";
        if (color === null) {
          colorBtn.style.background = "url(/image/transparent.png)";
          colorBtn.title = "Transparent";
        } else {
          colorBtn.style.backgroundColor = color;
        }
        if (color === this.state.currentColor)
          colorBtn.classList.add("selected");

        colorBtn.addEventListener("click", () => {
          this.state.currentColor = color;
          this.renderPalette();
          this.saveState();
        });
        this.dom.palette.appendChild(colorBtn);
      });
    },

    // Ficheiro: js/editor.js (Substituir esta função)

    renderTimeline: function () {
      this.dom.timeline.innerHTML = "";
      this.state.frames.forEach((frame, index) => {
        const frameEl = document.createElement("div");
        frameEl.className = "frame";
        if (index === this.state.activeFrameIndex)
          frameEl.classList.add("active");

        const previewEl = document.createElement("div");
        previewEl.className = "frame-preview";
        previewEl.style.gridTemplateColumns = `repeat(${this.state.gridSize}, 1fr)`;
        frame.pixels.forEach((row) => {
          row.forEach((color) => {
            const pixelPreview = document.createElement("div");
            pixelPreview.className = "frame-pixel";
            pixelPreview.style.backgroundColor = color || "transparent";
            previewEl.appendChild(pixelPreview);
          });
        });

        // --- ALTERAÇÃO PRINCIPAL AQUI ---
        // O HTML para a informação do frame agora inclui o brilho
        const infoEl = document.createElement("div");
        infoEl.className = "frame-info";
        infoEl.innerHTML = `
                    <div class="frame-control">
                        <span>${index + 1}</span>
                        <input type="number" class="frame-duration" value="${
                          frame.duration
                        }" title="Duration (ms)" min="10" step="10"> ms
                    </div>
                    <div class="frame-control">
                        <span>☀️</span>
                        <input type="number" class="frame-brightness" value="${
                          frame.brightness
                        }" title="Brightness (%)" min="0" max="100" step="1"> %
                    </div>
                `;

        const deleteBtn = document.createElement("button");
        deleteBtn.className = "frame-delete-btn";
        deleteBtn.innerHTML = "&times;";

        frameEl.append(previewEl, infoEl, deleteBtn);

        frameEl.addEventListener("click", (e) => {
          if (e.target.tagName !== "INPUT") this.selectFrame(index);
        });

        // Adiciona o listener para a DURAÇÃO
        infoEl
          .querySelector(".frame-duration")
          .addEventListener("change", (e) => {
            frame.duration = parseInt(e.target.value) || 100;
            this.updateOutputs();
            this.saveState();
          });

        // Adiciona o listener para o BRILHO
        infoEl
          .querySelector(".frame-brightness")
          .addEventListener("change", (e) => {
            frame.brightness = parseInt(e.target.value) || 100;
            this.updateOutputs();
            this.saveState();
          });

        deleteBtn.addEventListener("click", (e) => {
          e.stopPropagation();
          this.deleteFrame(index);
        });

        this.dom.timeline.appendChild(frameEl);
      });
    },

    // --- LÓGICA DE AÇÕES ---
    changeGridSize: function (newSize) {
      if (newSize === this.state.gridSize) return;
      if (
        confirm(
          "Changing grid size will clear the entire animation. Are you sure?"
        )
      ) {
        this.state.gridSize = newSize;
        this.state.frames = [];
        this.addNewFrame();
      } else {
        document.querySelector(
          `input[name="gridSize"][value="${this.state.gridSize}"]`
        ).checked = true;
      }
    },

    addNewFrame: function (clone = false) {
      let newFrame;
      if (clone && this.state.frames.length > 0) {
        newFrame = JSON.parse(
          JSON.stringify(this.state.frames[this.state.activeFrameIndex])
        );
      } else {
        newFrame = this._createEmptyFrame(this.state.gridSize);
      }
      this.state.frames.push(newFrame);
      this.selectFrame(this.state.frames.length - 1);
    },

    deleteFrame: function (index) {
      if (this.state.frames.length <= 1) {
        alert("You cannot delete the last frame.");
        return;
      }
      if (confirm(`Are you sure you want to delete frame ${index + 1}?`)) {
        this.state.frames.splice(index, 1);
        if (this.state.activeFrameIndex >= index) {
          this.selectFrame(Math.max(0, this.state.activeFrameIndex - 1));
        } else {
          this.renderAll();
        }
      }
    },

    selectFrame: function (index) {
      this.state.activeFrameIndex = index;
      this.renderAll();
      this.saveState();
    },

    clearFrame: function () {
      if (
        confirm(
          `Are you sure you want to clear frame ${
            this.state.activeFrameIndex + 1
          }?`
        )
      ) {
        const frame = this.state.frames[this.state.activeFrameIndex];
        frame.pixels = this._createEmptyFrame(this.state.gridSize).pixels;
        this.renderAll();
        this.saveState();
      }
    },

    togglePlayback: function () {
      this.state.isPlaying = !this.state.isPlaying;
      if (this.state.isPlaying) {
        this.dom.playBtn.innerHTML = "⏹️ Stop";
        this.state.playbackDirection = "forward";
        this.playNextFrame();
      } else {
        this.dom.playBtn.innerHTML = "▶️ Play";
        clearTimeout(this.state.playbackTimeout);
        delete this.state.playbackDirection;
        this.selectFrame(this.state.activeFrameIndex);
      }
    },

    playNextFrame: function () {
      if (!this.state.isPlaying) return;
      let currentIdx = this.state.activeFrameIndex;
      let currentDirection = this.state.playbackDirection || "forward";
      if (currentDirection === "forward") {
        currentIdx++;
        if (currentIdx >= this.state.frames.length) {
          if (!this.state.loop) {
            this.togglePlayback();
            return;
          }
          if (
            this.state.direction === "pingpong" &&
            this.state.frames.length > 1
          ) {
            currentIdx = this.state.frames.length - 2;
            this.state.playbackDirection = "backward";
          } else {
            currentIdx = 0;
          }
        }
      } else {
        // backward
        currentIdx--;
        if (currentIdx < 0) {
          if (this.state.direction === "pingpong") {
            currentIdx = 1;
            this.state.playbackDirection = "forward";
          } else {
            currentIdx = 0;
          }
        }
      }
      this.selectFrame(currentIdx < 0 ? 0 : currentIdx);
      const currentFrame = this.state.frames[this.state.activeFrameIndex];
      this.state.playbackTimeout = setTimeout(
        () => this.playNextFrame(),
        currentFrame.duration
      );
    },

    // --- GESTÃO DE DADOS ---
    updateOutputs: function () {
      this.updateJsonOutput();
      this.updateArduinoOutput();
    },

    updateJsonOutput: function () {
      const { name, gridSize, loop, direction, frames } = this.state;
      this.dom.jsonOutput.value = JSON.stringify(
        {
          version: 3,
          name,
          width: gridSize,
          height: gridSize,
          loop,
          direction,
          frames: frames.map((f) => ({
            duration: f.duration,
            brightness: f.brightness,
            pixels: f.pixels,
          })),
        },
        null,
        2
      );
    },

    updateArduinoOutput: function () {
      if (this.state.frames.length === 0) {
        this.dom.codeOutput.value =
          "// Add at least one frame to generate code.";
        return;
      }

      let code = `// Animation: ${this.state.name}\n`;
      code += `#include <FastLED.h>\n\n`;

      code += `const int FRAME_COUNT = ${this.state.frames.length};\n`;
      code += `const int FRAME_WIDTH = ${this.state.gridSize};\n`;
      code += `const int FRAME_HEIGHT = ${this.state.gridSize};\n`;

      // NOVO: Adiciona a direção da animação como uma string
      code += `const char* animation_direction = "${this.state.direction}";\n\n`;

      code += `const int frame_durations[FRAME_COUNT] = {${this.state.frames
        .map((f) => f.duration)
        .join(", ")}};\n\n`;
      code += `const uint8_t frame_brightness[FRAME_COUNT] = {${this.state.frames
        .map((f) => Math.round(f.brightness * 2.55))
        .join(", ")}};\n\n`;
      code += `const CRGB animation_data[FRAME_COUNT][FRAME_HEIGHT][FRAME_WIDTH] = {\n`;

      // O resto da geração dos dados dos frames não muda
      this.state.frames.forEach((frame, frameIndex) => {
        code += `  // Frame ${frameIndex + 1}\n  {\n`;
        frame.pixels.forEach((row, rowIndex) => {
          code += `    { ${row
            .map((hex) => {
              const rgb = this.hexToRgb(hex);
              return `CRGB(${rgb.r}, ${rgb.g}, ${rgb.b})`;
            })
            .join(", ")} }${rowIndex < frame.pixels.length - 1 ? ",\n" : "\n"}`;
        });
        code += `  }${
          frameIndex < this.state.frames.length - 1 ? ",\n\n" : "\n"
        }`;
      });
      code += `};\n`;

      this.dom.codeOutput.value = code;
    },

    loadFromJson: function () {
      try {
        const jsonText = this.dom.jsonOutput.value;
        if (!jsonText) throw new Error("JSON text box is empty.");
        const jsonData = JSON.parse(jsonText);
        if (jsonData.frames && Array.isArray(jsonData.frames)) {
          if (
            !jsonData.width ||
            jsonData.width !== jsonData.height ||
            jsonData.frames.length === 0
          )
            throw new Error("Invalid animation format.");
          this.state.name = jsonData.name || "Loaded Animation";
          this.state.gridSize = jsonData.width;
          this.state.loop = jsonData.loop !== undefined ? jsonData.loop : true;
          this.state.direction = jsonData.direction || "forward";
          this.state.frames = jsonData.frames.map((frame) => ({
            duration: frame.duration || 100,
            brightness: frame.brightness || 100,
            pixels:
              frame.pixels || this._createEmptyFrame(jsonData.width).pixels,
          }));
          this.selectFrame(0);
          alert(
            `Animation "${this.state.name}" loaded with ${this.state.frames.length} frames!`
          );
        } else if (jsonData.pixels && Array.isArray(jsonData.pixels)) {
          if (!jsonData.width || jsonData.width !== jsonData.height)
            throw new Error("Invalid static image format.");
          this.state.name = "Loaded Static Image";
          this.state.gridSize = jsonData.width;
          this.state.loop = false;
          this.state.direction = "forward";
          this.state.frames = [
            { duration: 1000, brightness: 100, pixels: jsonData.pixels },
          ];
          this.selectFrame(0);
          alert(
            `Static ${jsonData.width}x${jsonData.width} image loaded (converted to a single frame).`
          );
        } else {
          throw new Error("Unrecognized JSON format.");
        }
        this.saveState();
        this.renderAll();
      } catch (error) {
        alert("Error loading from JSON: " + error.message);
        console.error(error);
      }
    },

    // --- GUARDAR E EXPORTAR ---
    handleSave: function () {
      if (
        window.EmojiLibraryService &&
        window.EmojiLibraryService.directoryHandle
      ) {
        this.dom.saveForm.classList.remove("d-none");
        document.getElementById("save-filename-input").value =
          this.state.name.replace(/\s+/g, "_");
      } else {
        this.saveAsDownload();
      }
    },
    _executeSaveToLibrary: async function () {
      const fileName = document
        .getElementById("save-filename-input")
        .value.trim();
      if (!fileName) {
        alert("Please enter a file name.");
        return;
      }
      const emojiData = JSON.parse(this.dom.jsonOutput.value);
      const success = await window.EmojiLibraryService.saveEmoji(
        emojiData,
        fileName
      );
      if (success) {
        this.dom.saveForm.classList.add("d-none");
      }
    },
    saveAsDownload: function () {
      const dataStr = this.dom.jsonOutput.value;
      const dataBlob = new Blob([dataStr], { type: "application/json" });
      this._downloadFile(
        URL.createObjectURL(dataBlob),
        `${this.state.name.replace(/\s+/g, "_") || "animation"}.json`
      );
    },

    handleExport: async function () {
      const options = {
        format: document.querySelector('input[name="exportFormat"]:checked')
          .value,
        size: parseInt(document.getElementById("exportSize").value) || 256,
        grid: document.getElementById("exportGrid").checked,
        transparent: document.getElementById("exportTransparent").checked,
      };

      const useLibrary =
        window.EmojiLibraryService &&
        window.EmojiLibraryService.directoryHandle;
      let fileName;

      if (useLibrary) {
        const defaultName = `${
          this.state.name.replace(/\s+/g, "_") || "export"
        }.${options.format}`;
        fileName = prompt("Enter a name for the exported file:", defaultName);
        if (!fileName) return; // O utilizador cancelou
      }

      try {
        if (options.format === "png") {
          const frameIndex = this.state.activeFrameIndex + 1;
          const defaultName = `${this.state.name}_frame_${frameIndex}.png`;

          const dataUrl = this.exportAsPNG(options);
          if (!dataUrl) return;

          if (useLibrary) {
            const blob = this._dataURLtoBlob(dataUrl);
            await window.EmojiLibraryService.saveBinaryFile(
              blob,
              fileName || defaultName
            );
          } else {
            this._downloadFile(dataUrl, defaultName);
          }
        } else if (options.format === "gif") {
          const defaultName = `${this.state.name}.gif`;

          const blob = await this.exportAsGIF(options);
          if (!blob) return;

          if (useLibrary) {
            await window.EmojiLibraryService.saveBinaryFile(
              blob,
              fileName || defaultName
            );
          } else {
            this._downloadFile(URL.createObjectURL(blob), defaultName);
          }
        }
      } catch (error) {
        console.error("Export failed:", error);
        alert("An error occurred during export.");
      }

      // Fecha o painel
      const offcanvasEl = document.getElementById("exportOffcanvas");
      const offcanvasInstance = bootstrap.Offcanvas.getInstance(offcanvasEl);
      if (offcanvasInstance) offcanvasInstance.hide();
    },

    _drawFrameToCanvas: function (frame, options, customBgColor = "#FFFFFF") {
      const canvas = document.createElement("canvas");
      canvas.width = options.size;
      canvas.height = options.size;
      const ctx = canvas.getContext("2d");

      const pixelSize = options.size / this.state.gridSize;

      // ALTERAÇÃO: Usa a cor de fundo recebida se a imagem não for transparente.
      // Se for transparente, não desenha fundo nenhum.
      if (!options.transparent) {
        ctx.fillStyle = customBgColor;
        ctx.fillRect(0, 0, options.size, options.size);
      }

      // Desenha os pixeis com o brilho ajustado
      frame.pixels.forEach((row, y) => {
        row.forEach((color, x) => {
          if (color) {
            // Apenas se a cor não for null (transparente)
            const rgb = this._hexToRgb(color);
            let [h, s, l] = this._rgbToHsl(rgb.r, rgb.g, rgb.b);
            l = l * (frame.brightness / 100);
            ctx.fillStyle = this._hslToRgbString(h, s, l);
            ctx.fillRect(x * pixelSize, y * pixelSize, pixelSize, pixelSize);
          }
        });
      });

      // Lógica da grelha inteligente (sem alterações)
      if (options.grid) {
        ctx.strokeStyle = "rgba(0, 0, 0, 0.2)"; // Preto semi-transparente para melhor visibilidade
        ctx.lineWidth = Math.max(1, Math.floor(pixelSize / 16));

        for (let y = 0; y < this.state.gridSize - 1; y++) {
          for (let x = 0; x < this.state.gridSize; x++) {
            const thisPixel = frame.pixels[y][x];
            const pixelBelow = frame.pixels[y + 1][x];
            if (thisPixel !== null || pixelBelow !== null) {
              const lineY = (y + 1) * pixelSize;
              ctx.beginPath();
              ctx.moveTo(x * pixelSize, lineY);
              ctx.lineTo((x + 1) * pixelSize, lineY);
              ctx.stroke();
            }
          }
        }

        for (let y = 0; y < this.state.gridSize; y++) {
          for (let x = 0; x < this.state.gridSize - 1; x++) {
            const thisPixel = frame.pixels[y][x];
            const pixelRight = frame.pixels[y][x + 1];
            if (thisPixel !== null || pixelRight !== null) {
              const lineX = (x + 1) * pixelSize;
              ctx.beginPath();
              ctx.moveTo(lineX, y * pixelSize);
              ctx.lineTo(lineX, (y + 1) * pixelSize);
              ctx.stroke();
            }
          }
        }
      }

      return canvas;
    },

    exportAsPNG: function (options) {
      const activeFrame = this.state.frames[this.state.activeFrameIndex];
      if (!activeFrame) return null;

      const canvas = this._drawFrameToCanvas(activeFrame, options);
      return canvas.toDataURL("image/png"); // Devolve a imagem como dataURL
    },

    // Ficheiro: js/editor.js (Substituir esta função)

    exportAsGIF: function (options) {
      // Esta função agora devolve uma Promise
      return new Promise((resolve, reject) => {
        const loader = document.getElementById("export-loader");
        loader.classList.remove("d-none");

        let gifOptions = {
          /* ... as suas opções de GIF ... */
        };
        const transparentMagicColor = "#00ff00";
        if (options.transparent) {
          gifOptions.transparent = transparentMagicColor;
        }

        // Adicione as opções que faltam
        gifOptions.workers = 2;
        gifOptions.quality = 10;
        gifOptions.width = options.size;
        gifOptions.height = options.size;
        gifOptions.workerScript = "js/gif.worker.js";

        const gif = new GIF(gifOptions);

        const frameSequence =
          this.state.direction === "pingpong" && this.state.frames.length > 1
            ? [
                ...this.state.frames,
                ...[...this.state.frames.slice(1, -1)].reverse(),
              ]
            : this.state.frames;

        for (const frame of frameSequence) {
          const bgColor = options.transparent
            ? transparentMagicColor
            : "#FFFFFF";
          const canvas = this._drawFrameToCanvas(frame, options, bgColor);
          gif.addFrame(canvas, { delay: frame.duration });
        }

        gif.on("finished", (blob) => {
          loader.classList.add("d-none");
          resolve(blob); // Em vez de descarregar, resolve a Promise com o Blob
        });

        gif.on("abort", () => {
          loader.classList.add("d-none");
          reject(new Error("GIF rendering was aborted."));
        });

        gif.render();
      });
    },

    _downloadFile: function (url, filename) {
      const link = document.createElement("a");
      link.href = url;
      link.download = filename;
      document.body.appendChild(link);
      link.click();
      document.body.removeChild(link);
    },

    // --- DADOS ESTÁTICOS E FUNÇÕES AUXILIARES FINAIS ---
    colors: [
      null,
      "#000000",
      "#FFFFFF",
      "#FF0000",
      "#00FF00",
      "#0000FF",
      "#FFFF00",
      "#FF00FF",
      "#00FFFF",
      "#FFA500",
      "#800080",
      "#FF4500",
      "#DC143C",
      "#B22222",
      "#8B0000",
      "#CD5C5C",
      "#32CD32",
      "#228B22",
      "#008000",
      "#006400",
      "#ADFF2F",
      "#1E90FF",
      "#4169E1",
      "#000080",
      "#00008B",
      "#4682B4",
      "#FFD700",
      "#FF8C00",
      "#FF6347",
      "#FF7F50",
      "#DAA520",
      "#9932CC",
      "#8A2BE2",
      "#4B0082",
      "#9400D3",
      "#BA55D3",
      "#FF69B4",
      "#FF1493",
      "#DB7093",
      "#C71585",
      "#FFC0CB",
      "#A52A2A",
      "#8B4513",
      "#D2691E",
      "#CD853F",
      "#F4A460",
      "#808080",
      "#696969",
      "#2F4F4F",
      "#708090",
      "#C0C0C0",
      "#7CFC00",
      "#7FFFD4",
      "#F0E68C",
      "#E6E6FA",
      "#FFFACD",
      "#FFE4E1",
      "#FFE4B5",
      "#FAFAD2",
      "#F5F5DC",
      "#F0FFF0",
    ],
    hexToRgb: function (hex) {
      if (!hex) return { r: 0, g: 0, b: 0 }; // Para a cor transparente
      return {
        r: parseInt(hex.substring(1, 3), 16),
        g: parseInt(hex.substring(3, 5), 16),
        b: parseInt(hex.substring(5, 7), 16),
      };
    },
    copyCode: function () {
      navigator.clipboard
        .writeText(this.dom.codeOutput.value)
        .then(() => alert("Arduino code copied!"));
    },

    // --- NOVAS FUNÇÕES AUXILIARES DE COR ---
    _hexToRgb: function (hex) {
      if (!hex) return { r: 0, g: 0, b: 0 };
      const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
      return result
        ? {
            r: parseInt(result[1], 16),
            g: parseInt(result[2], 16),
            b: parseInt(result[3], 16),
          }
        : { r: 0, g: 0, b: 0 };
    },

    _rgbToHsl: function (r, g, b) {
      (r /= 255), (g /= 255), (b /= 255);
      const max = Math.max(r, g, b),
        min = Math.min(r, g, b);
      let h,
        s,
        l = (max + min) / 2;
      if (max == min) {
        h = s = 0;
      } else {
        const d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        switch (max) {
          case r:
            h = (g - b) / d + (g < b ? 6 : 0);
            break;
          case g:
            h = (b - r) / d + 2;
            break;
          case b:
            h = (r - g) / d + 4;
            break;
        }
        h /= 6;
      }
      return [h, s, l];
    },

    _hslToRgbString: function (h, s, l) {
      let r, g, b;
      if (s == 0) {
        r = g = b = l;
      } else {
        const hue2rgb = (p, q, t) => {
          if (t < 0) t += 1;
          if (t > 1) t -= 1;
          if (t < 1 / 6) return p + (q - p) * 6 * t;
          if (t < 1 / 2) return q;
          if (t < 2 / 3) return p + (q - p) * (2 / 3 - t) * 6;
          return p;
        };
        const q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        const p = 2 * l - q;
        r = hue2rgb(p, q, h + 1 / 3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1 / 3);
      }
      return `rgb(${Math.round(r * 255)}, ${Math.round(g * 255)}, ${Math.round(
        b * 255
      )})`;
    },

    setActiveTool: function (toolName) {
      this.state.activeTool = toolName;

      // Atualiza a classe 'active' nos botões
      this.dom.drawToolBtn.classList.toggle("active", toolName === "draw");
      this.dom.fillToolBtn.classList.toggle("active", toolName === "fill");

      // Atualiza a classe do cursor na grelha
      this.dom.grid.classList.toggle("fill-mode", toolName === "fill");
    },

    floodFill: function (startX, startY) {
      const frame = this.state.frames[this.state.activeFrameIndex];
      const pixels = frame.pixels;
      const targetColor = pixels[startY][startX];
      const replacementColor = this.state.currentColor;

      // 1. Caso base: Não faz nada se a cor de destino for a mesma que a de substituição
      if (targetColor === replacementColor) {
        return;
      }

      const queue = [[startX, startY]]; // A fila de pixeis a processar
      const size = this.state.gridSize;

      while (queue.length > 0) {
        const [x, y] = queue.shift(); // Retira o primeiro pixel da fila

        // 2. Verifica se as coordenadas são válidas e se a cor corresponde
        if (
          x >= 0 &&
          x < size &&
          y >= 0 &&
          y < size &&
          pixels[y][x] === targetColor
        ) {
          // 3. Pinta o pixel
          pixels[y][x] = replacementColor;

          // 4. Adiciona os vizinhos à fila para serem verificados
          queue.push([x + 1, y]); // Direita
          queue.push([x - 1, y]); // Esquerda
          queue.push([x, y + 1]); // Baixo
          queue.push([x, y - 1]); // Cima
        }
      }

      // 5. Atualiza a interface e guarda o estado
      this.renderAll();
      this.saveState();
    },

    _dataURLtoBlob: function (dataurl) {
      const arr = dataurl.split(","),
        mime = arr[0].match(/:(.*?);/)[1];
      const bstr = atob(arr[1]);
      let n = bstr.length;
      const u8arr = new Uint8Array(n);
      while (n--) {
        u8arr[n] = bstr.charCodeAt(n);
      }
      return new Blob([u8arr], { type: mime });
    },

    checkPendingPhoto: function () {
      // Mostra ou esconde o botão de "colar" com base no estado do serviço
      const hasPendingPhoto =
        window.WebSocketService && window.WebSocketService.pendingPhotoData;
      if (this.dom.pastePhotoBtn) {
        this.dom.pastePhotoBtn.classList.toggle("d-none", !hasPendingPhoto);
      }
    },

    // Ficheiro: js/editor.js (SUBSTITUIR esta função)

 pastePhoto: function () {
      console.log("[Editor] Iniciando pastePhoto...");
      const photoData = window.WebSocketService.pendingPhotoData;

      if (!photoData) {
        console.log("[Editor] ERRO: Nenhum dado de foto disponível em pendingPhotoData.");
        alert(
          "No photo data available to paste. This should not happen if the button is visible."
        );
        this.checkPendingPhoto();
        return;
      }

      console.log("[Editor] Dados da foto recebidos:", photoData);

      if (
        confirm(
          "This will overwrite your current active frame with the photo from the device. Are you sure?"
        )
      ) {
        const frame = this.state.frames[this.state.activeFrameIndex];
        if (!frame) {
          console.log("[Editor] ERRO: Nenhum frame ativo encontrado.");
          alert("Error: No active frame to paste into.");
          return;
        }

        console.log("[Editor] Frame ativo encontrado:", frame);

        // Verifica se o tamanho da grelha corresponde
        if (this.state.gridSize !== photoData.width) {
          console.log(
            `[Editor] ERRO: Tamanho da grelha não corresponde. Editor: ${this.state.gridSize}x${this.state.gridSize}, Foto: ${photoData.width}x${photoData.width}`
          );
          alert(
            `Grid size mismatch! The photo is ${photoData.width}x${photoData.width} but the editor is set to ${this.state.gridSize}x${this.state.gridSize}. Please change the grid size and try again.`
          );
          return;
        }

        console.log(
          "[Editor] Aplicando pixels da foto ao frame ativo, índice:",
          this.state.activeFrameIndex
        );

        // Copia os pixels da foto para o frame
        frame.pixels = photoData.pixels.map(row => row.slice()); // Cria uma cópia profunda

        console.log("[Editor] Pixels aplicados ao frame:", frame.pixels);

        // Limpa os dados pendentes
        window.WebSocketService.pendingPhotoData = null;
        console.log("[Editor] pendingPhotoData limpo.");

        // Dispara o evento photopasted
        window.dispatchEvent(new CustomEvent("photopasted"));
        console.log("[Editor] Evento photopasted disparado.");

        // Atualiza a interface
        console.log("[Editor] Chamando renderAll para atualizar a UI...");
        this.checkPendingPhoto(); // Esconde o botão Paste
        this.renderAll(); // Redesenha a grade com a foto
        this.saveState(); // Salva o estado

        console.log("[Editor] pastePhoto concluído com sucesso.");
        alert("Photo pasted successfully!");
      } else {
        console.log("[Editor] Usuário cancelou a operação de colar.");
      }
    },

    
    createNewAnimation: function () {
      if (
        confirm(
          "This will clear your entire current animation and start a new one. Are you sure?"
        )
      ) {
        // Reseta o estado para os valores por defeito
        this.state.name = "New Animation";
        this.state.frames = []; // Limpa todos os frames
        this.state.activeFrameIndex = 0;

        // Adiciona o primeiro frame em branco
        this.addNewFrame();

        // O addNewFrame já chama renderAll e saveState, por isso não é preciso aqui.
        alert("New animation created!");
      }
    },
  };

  window.AnimationEditor.loadState();
}

window.AnimationEditor.init();
