// --- Padrão Singleton: Garante que o código de inicialização corre apenas uma vez ---

// Verifica se o nosso objeto de editor já existe na janela global.
if (!window.EmojiEditor) {

    // Se não existir, cria-o. Este bloco só vai correr UMA VEZ por sessão da página.
    window.EmojiEditor = {

        // 1. ESTADO (STATE): As variáveis que precisam de persistir.
        state: {
            currentColor: '#FF0000',
            matrix: Array(8).fill().map(() => Array(8).fill('#000000'))
        },

        // 2. MÉTODOS (FUNÇÕES): A lógica do nosso editor.

        /**
         * Carrega o estado a partir do sessionStorage. Corre apenas na inicialização.
         */
        loadState: function() {
            const savedMatrix = sessionStorage.getItem('editorMatrix');
            const savedColor = sessionStorage.getItem('editorCurrentColor');
            if (savedMatrix) {
                this.state.matrix = JSON.parse(savedMatrix);
            }
            if (savedColor) {
                this.state.currentColor = savedColor;
            }
        },

        /**
         * Guarda o estado atual no sessionStorage.
         */
        saveState: function() {
            sessionStorage.setItem('editorMatrix', JSON.stringify(this.state.matrix));
            sessionStorage.setItem('editorCurrentColor', this.state.currentColor);
        },

        /**
         * Inicializa a interface do editor. Esta função PODE e DEVE correr sempre que a página do editor é mostrada.
         */
        init: function() {
            const grid = document.getElementById('pixel-grid');
            const palette = document.getElementById('color-palette');
            if (!grid || !palette) return;

            grid.innerHTML = '';
            palette.innerHTML = '';

            // Cria a grelha de pixels usando o estado atual
            for (let y = 0; y < 8; y++) {
                for (let x = 0; x < 8; x++) {
                    const pixel = document.createElement('div');
                    pixel.className = 'pixel';
                    pixel.style.backgroundColor = this.state.matrix[y][x];
                    pixel.addEventListener('click', () => {
                        this.state.matrix[y][x] = this.state.currentColor;
                        pixel.style.backgroundColor = this.state.currentColor;
                        this.updateOutputs();
                        this.saveState(); // Salva o estado após cada clique
                    });
                    grid.appendChild(pixel);
                }
            }

            // Cria a paleta de cores
            this.colors.forEach(color => {
                const colorBtn = document.createElement('div');
                colorBtn.className = 'color-option';
                colorBtn.style.backgroundColor = color;
                if (color === this.state.currentColor) {
                    colorBtn.classList.add('selected');
                }
                colorBtn.addEventListener('click', () => {
                    this.state.currentColor = color;
                    document.querySelectorAll('.color-option').forEach(el => el.classList.remove('selected'));
                    colorBtn.classList.add('selected');
                    this.saveState(); // Salva o estado ao mudar de cor
                });
                palette.appendChild(colorBtn);
            });

            // Adiciona eventos aos botões
            document.getElementById('clear-btn').addEventListener('click', () => this.clearGrid());
            document.getElementById('copy-btn').addEventListener('click', () => this.copyCode());
            document.getElementById('save-btn').addEventListener('click', () => this.saveToFile());
            document.getElementById('load-btn').addEventListener('click', () => this.loadFromJson());

            this.updateOutputs();
        },

        clearGrid: function() {
            if (confirm('Tem a certeza que quer limpar a grelha?')) {
                this.state.matrix = Array(8).fill().map(() => Array(8).fill('#000000'));
                this.saveState();
                this.init(); // Redesenha a interface
            }
        },

        updateOutputs: function() { /* ... código ... */ },
        updateArduinoOutput: function() { /* ... código ... */ },
        updateJsonOutput: function() { /* ... código ... */ },
        hexToRgb: function(hex) { /* ... código ... */ },
        copyCode: function() { /* ... código ... */ },
        saveToFile: function() { /* ... código ... */ },
        loadFromJson: function() { /* ... código ... */ },

        // 3. DADOS ESTÁTICOS: A paleta de cores não muda.
        colors: ['#000000', '#FFFFFF', '#FF0000', '#00FF00', '#0000FF', '#FFFF00', '#FF00FF', '#00FFFF', '#FFA500', '#800080', '#FF4500', '#DC143C', '#B22222', '#8B0000', '#CD5C5C', '#32CD32', '#228B22', '#008000', '#006400', '#ADFF2F', '#1E90FF', '#4169E1', '#000080', '#00008B', '#4682B4', '#FFD700', '#FF8C00', '#FF6347', '#FF7F50', '#DAA520', '#9932CC', '#8A2BE2', '#4B0082', '#9400D3', '#BA55D3', '#FF69B4', '#FF1493', '#DB7093', '#C71585', '#FFC0CB', '#A52A2A', '#8B4513', '#D2691E', '#CD853F', '#F4A460', '#808080', '#696969', '#2F4F4F', '#708090', '#C0C0C0', '#7CFC00', '#7FFFD4', '#F0E68C', '#E6E6FA', '#FFFACD', '#FFE4E1', '#FFE4B5', '#FAFAD2', '#F5F5DC', '#F0FFF0']
    };

    // --- COPIAR E COLAR AS SUAS FUNÇÕES PARA DENTRO DO OBJETO ---
    // (Abaixo estão as funções adaptadas para usar 'this.state' e 'this')

    window.EmojiEditor.updateOutputs = function() { this.updateArduinoOutput(); this.updateJsonOutput(); };
    window.EmojiEditor.updateArduinoOutput = function() {
        let code = 'const CRGB emoji[8][8] = {\n';
        for (let y = 0; y < 8; y++) {
            code += '  { ';
            for (let x = 0; x < 8; x++) {
                const rgb = this.hexToRgb(this.state.matrix[y][x]);
                code += `CRGB(${rgb.r}, ${rgb.g}, ${rgb.b})`;
                if (x < 7) code += ', ';
            }
            code += ' }';
            if (y < 7) code += ',';
            code += '\n';
        }
        code += '};';
        document.getElementById('code-output').value = code;
    };
    window.EmojiEditor.updateJsonOutput = function() {
        document.getElementById('json-output').value = JSON.stringify({ width: 8, height: 8, pixels: this.state.matrix }, null, 2);
    };
    window.EmojiEditor.hexToRgb = function(hex) {
        return { r: parseInt(hex.substring(1, 3), 16), g: parseInt(hex.substring(3, 5), 16), b: parseInt(hex.substring(5, 7), 16) };
    };
    window.EmojiEditor.copyCode = function() {
        const code = document.getElementById('code-output');
        navigator.clipboard.writeText(code.value).then(() => alert('Código Arduino copiado!'));
    };
    window.EmojiEditor.saveToFile = function() {
        const dataStr = JSON.stringify({ width: 8, height: 8, pixels: this.state.matrix }, null, 2);
        const dataBlob = new Blob([dataStr], {type: "application/json"});
        const url = URL.createObjectURL(dataBlob);
        const linkElement = document.createElement('a');
        linkElement.setAttribute('href', url);
        linkElement.setAttribute('download', 'emoji_pattern.json');
        document.body.appendChild(linkElement);
        linkElement.click();
        document.body.removeChild(linkElement);
    };
    window.EmojiEditor.loadFromJson = function() {
        try {
            const jsonText = document.getElementById('json-output').value;
            if (!jsonText) throw new Error('A caixa de texto JSON está vazia.');
            const jsonData = JSON.parse(jsonText);
            if (jsonData.width !== 8 || jsonData.height !== 8 || !jsonData.pixels) throw new Error('Formato JSON inválido.');
            this.state.matrix = jsonData.pixels;
            this.saveState();
            this.init();
            alert('Padrão carregado com sucesso!');
        } catch (error) {
            alert('Erro ao carregar o padrão: ' + error.message);
        }
    };
    
    // FINALMENTE: Carrega o estado guardado, uma única vez.
    window.EmojiEditor.loadState();
}

// --- Ponto de Entrada ---
// Esta linha está FORA do 'if'. Ela corre TODAS as vezes que a página do editor é carregada.
// Ela simplesmente diz ao nosso objeto já existente para se redesenhar no ecrã.
window.EmojiEditor.init();