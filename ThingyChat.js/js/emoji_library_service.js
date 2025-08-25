// Ficheiro: js/emoji_library_service.js

if (!window.EmojiLibraryService) {
  window.EmojiLibraryService = {
    directoryHandle: null, // Onde guardamos a referência à pasta selecionada
    emojis: [], // A lista de emojis carregados em memória

    /**
     * Pede ao utilizador para selecionar uma pasta. Guarda a referência e carrega os emojis.
     */
    selectFolder: async function () {
      try {
        // Abre o seletor de pastas do browser
        this.directoryHandle = await window.showDirectoryPicker();
        await this.loadEmojisFromHandle();
        alert(
          `Pasta "${this.directoryHandle.name}" selecionada e emojis carregados!`
        );
        return true;
      } catch (err) {
        // O utilizador pode ter cancelado o seletor, o que é normal.
        if (err.name !== "AbortError") {
          console.error("Erro ao selecionar a pasta:", err);
          alert("Não foi possível aceder à pasta selecionada.");
        }
        return false;
      }
    },

    /**
     * Lê todos os ficheiros suportados (.json, .png, .gif) da pasta e carrega-os.
     */
    loadLibraryContents: async function () {
      if (!this.directoryHandle) return;

      this.libraryItems = [];
      console.log("Loading library contents...");

      for await (const entry of this.directoryHandle.values()) {
        if (entry.kind !== "file") continue;

        const fileName = entry.name;
        const baseName = fileName.substring(0, fileName.lastIndexOf("."));

        try {
          if (fileName.endsWith(".json")) {
            const file = await entry.getFile();
            const content = await file.text();
            const data = JSON.parse(content);
            const type =
              data.frames && data.frames.length > 1 ? "animation" : "static";
            this.libraryItems.push({ type, data, fileName: baseName });
          } else if (fileName.endsWith(".png") || fileName.endsWith(".gif")) {
            const type = fileName.endsWith(".png") ? "png" : "gif";
            const file = await entry.getFile();
            const url = URL.createObjectURL(file);
            this.libraryItems.push({ type, url, fileName: baseName });
          }
        } catch (err) {
          console.warn(`Could not load file ${fileName}:`, err);
        }
      }
      console.log(`Loaded ${this.libraryItems.length} library items.`);
      window.dispatchEvent(new CustomEvent("libraryupdated"));
    },

    /**
     * Lê todos os ficheiros .json da pasta selecionada e carrega-os para a memória.
     */
    loadEmojisFromHandle: async function () {
      if (!this.directoryHandle) return;

      this.emojis = []; // Limpa a lista antiga
      console.log("A carregar emojis da pasta...");

      // Itera sobre todos os ficheiros/pastas dentro da pasta selecionada
      for await (const entry of this.directoryHandle.values()) {
        if (entry.kind === "file" && entry.name.endsWith(".json")) {
          try {
            const file = await entry.getFile();
            const content = await file.text();
            const emojiData = JSON.parse(content);

            // Adiciona o nome do ficheiro (sem .json) aos dados do emoji
            emojiData.fileName = entry.name.replace(".json", "");

            this.emojis.push(emojiData);
          } catch (err) {
            console.warn(
              `Não foi possível ler ou parsear o ficheiro ${entry.name}:`,
              err
            );
          }
        }
      }
      console.log(`Carregados ${this.emojis.length} emojis.`);

      // Dispara um evento para notificar outras partes da app que a biblioteca foi atualizada
      window.dispatchEvent(new CustomEvent("emojilibraryupdated"));
    },

    /**
     * Salva um novo emoji (ou sobrescreve um existente) na pasta.
     * @param {object} emojiData - O objeto com os dados do emoji (width, height, pixels).
     * @param {string} fileName - O nome do ficheiro (sem .json).
     */
    saveEmoji: async function (emojiData, fileName) {
      if (!this.directoryHandle) {
        alert(
          "Por favor, selecione primeiro uma pasta para a biblioteca de emojis."
        );
        return false;
      }
      if (!fileName || fileName.trim() === "") {
        alert("O nome do ficheiro não pode estar vazio.");
        return false;
      }

      try {
        // Pede um "handle" para o ficheiro (cria-o se não existir)
        const fileHandle = await this.directoryHandle.getFileHandle(
          `${fileName}.json`,
          { create: true }
        );
        // Pede permissão para escrever no ficheiro
        const writable = await fileHandle.createWritable();
        // Converte o objeto de dados para uma string JSON formatada
        const jsonString = JSON.stringify(emojiData, null, 2);
        // Escreve o conteúdo
        await writable.write(jsonString);
        // Fecha o ficheiro, guardando as alterações
        await writable.close();

        alert(`Emoji "${fileName}" guardado com sucesso!`);

        // Recarrega a biblioteca em memória para incluir a nova adição
        await this.loadEmojisFromHandle();
        return true;
      } catch (err) {
        console.error(`Erro ao guardar o ficheiro ${fileName}.json:`, err);
        alert("Ocorreu um erro ao guardar o emoji.");
        return false;
      }
    },

    /**
     * Devolve a lista de emojis atualmente em memória.
     */
    getEmojis: function () {
      return this.emojis;
    },

    /**
     * Guarda um ficheiro binário (Blob) na pasta da biblioteca.
     * @param {Blob} blob - Os dados do ficheiro a guardar.
     * @param {string} fileName - O nome completo do ficheiro (ex: 'meu_gif.gif').
     */
    saveBinaryFile: async function (blob, fileName) {
      if (!this.directoryHandle) {
        alert("Por favor, selecione primeiro uma pasta para a biblioteca.");
        return false;
      }
      if (!fileName) {
        alert("O nome do ficheiro é inválido.");
        return false;
      }

      try {
        const fileHandle = await this.directoryHandle.getFileHandle(fileName, {
          create: true,
        });
        const writable = await fileHandle.createWritable();
        await writable.write(blob); // Escreve o Blob diretamente
        await writable.close();

        alert(`Ficheiro "${fileName}" guardado com sucesso na sua biblioteca!`);
        return true;
      } catch (err) {
        console.error(`Erro ao guardar o ficheiro ${fileName}:`, err);
        alert("Ocorreu um erro ao guardar o ficheiro na biblioteca.");
        return false;
      }
    },

    getLibraryItems: function () {
      return this.libraryItems;
    },
  };

  // Para limpar os URLs de Blob quando a página fecha (boa prática)
  window.addEventListener("beforeunload", () => {
    if (window.EmojiLibraryService) {
      window.EmojiLibraryService.getLibraryItems().forEach((item) => {
        if (item.url) {
          URL.revokeObjectURL(item.url);
        }
      });
    }
  });
}
