// Ficheiro: js/emoji16_display.js (VERSÃO COMPLETA E CORRIGIDA)

if (!window.EmojiDisplayPage) {
  window.EmojiDisplayPage = {
    init: function () {
      // Guarda referências aos elementos da UI para fácil acesso
      this.ui = {
        status: document.getElementById("status"),
        hostInput: document.getElementById("hostInput"),
        connectBtn: document.getElementById("connectBtn"),
        emojiSelector: document.getElementById("emoji-selector"),
        sendEmojiBtn: document.getElementById("sendEmojiBtn"),
        commandSelector: document.getElementById("command-selector"),
        sendCommandBtn: document.getElementById("sendCommandBtn"),
        emojiPreview: document.getElementById("emoji-preview"),
        takePhotoBtn: document.getElementById("takePhotoBtn"),
      };

      // --- INICIALIZAÇÃO E EVENTOS ---
      this.updateConnectionStatus();
      this.populateEmojiDropdown();
      this.ui.takePhotoBtn.addEventListener("click", () =>
        this.takePixelPhoto()
      );

      // "Ouve" por atualizações dos serviços centrais
      window.WebSocketService.addListener(() => this.updateConnectionStatus());
      window.addEventListener("emojilibraryupdated", () =>
        this.populateEmojiDropdown()
      );

      // Adiciona eventos aos botões e dropdowns
      this.ui.connectBtn.addEventListener("click", () => {
        const host = this.ui.hostInput.value.trim();
        if (host) {
          window.WebSocketService.connect(host);
        } else {
          alert("Please enter a device host or IP address.");
        }
      });
      this.ui.sendEmojiBtn.addEventListener("click", () =>
        this.sendSelectedEmoji()
      );
      this.ui.sendCommandBtn.addEventListener("click", () =>
        this.sendSelectedCommand()
      );
      this.ui.emojiSelector.addEventListener("change", (event) =>
        this.previewEmoji(event.target.value)
      );
    },

    // --- FUNÇÕES DE INTERFACE ---
    updateConnectionStatus: function () {
      const connectionInfo = window.WebSocketService.getConnectionStatus();
      this.ui.status.textContent = connectionInfo.status;
      this.ui.status.style.color = connectionInfo.isConnected ? "green" : "red";
      this.ui.hostInput.value = window.WebSocketService.ip; // Mantém o input sincronizado

      // Ativa/desativa os controlos com base no estado da conexão
      const isConnected = connectionInfo.isConnected;
      this.ui.emojiSelector.disabled = !isConnected;
      this.ui.sendEmojiBtn.disabled = !isConnected;
      this.ui.commandSelector.disabled = !isConnected;
      this.ui.sendCommandBtn.disabled = !isConnected;
      this.ui.takePhotoBtn.disabled = !isConnected;

      // Muda o texto e a cor do botão de conexão
      this.ui.connectBtn.textContent = isConnected ? "Disconnect" : "Connect";
      this.ui.connectBtn.classList.toggle("btn-success", !isConnected);
      this.ui.connectBtn.classList.toggle("btn-danger", isConnected);
    },

    populateEmojiDropdown: function () {
      const previouslySelected = this.ui.emojiSelector.value;
      this.ui.emojiSelector.innerHTML =
        '<option value="">Select an emoji...</option>';
      const emojis = window.EmojiLibraryService.getEmojis();
      emojis.forEach((emoji) => {
        const option = document.createElement("option");
        option.value = emoji.fileName;
        option.textContent = emoji.fileName;
        this.ui.emojiSelector.appendChild(option);
      });
      this.ui.emojiSelector.value = previouslySelected;
      this.previewEmoji(this.ui.emojiSelector.value);
    },

    previewEmoji: function (fileName) {
      this.ui.emojiPreview.innerHTML = "";
      if (!fileName) return;

      const emojis = window.EmojiLibraryService.getEmojis();
      const emojiData = emojis.find((e) => e.fileName === fileName);

      if (emojiData) {
        const grid = document.createElement("div");
        grid.className = "emoji-grid-container";
        grid.style.gridTemplateColumns = `repeat(${emojiData.width}, 1fr)`;
        emojiData.pixels.flat().forEach((color) => {
          const pixel = document.createElement("div");
          pixel.className = "emoji-grid-pixel";
          pixel.style.backgroundColor = color;
          grid.appendChild(pixel);
        });
        this.ui.emojiPreview.appendChild(grid);
      }
    },

    // --- FUNÇÕES DE COMUNICAÇÃO ---
    sendSelectedEmoji: function () {
      const selectedFileName = this.ui.emojiSelector.value;
      if (!selectedFileName) {
        alert("Please select an emoji first.");
        return;
      }
      const emojis = window.EmojiLibraryService.getEmojis();
      const selectedEmoji = emojis.find((e) => e.fileName === selectedFileName);
      if (selectedEmoji) {
        const pixelString = selectedEmoji.pixels
          .flat()
          .map((hex) => hex.replace("#", ""))
          .join(",");
        const command = `DRAW_EMOJI:${pixelString}`;
        if (!window.WebSocketService.send(command)) {
          alert("Failed to send emoji. Connection may be lost.");
        }
      }
    },

    sendSelectedCommand: function () {
      const command = this.ui.commandSelector.value;
      if (!command) {
        alert("Please select a command first.");
        return;
      }
      if (!window.WebSocketService.send(command)) {
        alert("Failed to send command. Connection may be lost.");
      }
    },

    takePixelPhoto: function () {
      if (!window.WebSocketService.getConnectionStatus().isConnected) {
        alert("Device is not connected.");
        return;
      }
      const command = "TAKE_PIXEL_PHOTO:16"; // Hardcoded para 16x16 por agora
      alert(
        "Requesting photo from device... The editor will open automatically."
      );
      window.WebSocketService.send(command);
    },
  };
}

// Inicializa a página
window.EmojiDisplayPage.init();
