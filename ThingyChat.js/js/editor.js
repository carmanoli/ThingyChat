// File: js/editor.js (Updated for dynamic grid size)

if (!window.EmojiEditor) {

    window.EmojiEditor = {
        // 1. STATE: Now includes gridSize
        state: {
            gridSize: 8,
            currentColor: '#FF0000',
            matrix: [] // Matrix will be created based on gridSize
        },

        // Helper function to create an empty matrix of a given size
        _createEmptyMatrix: function(size) {
            return Array(size).fill().map(() => Array(size).fill('#000000'));
        },
        
        loadState: function() {
            const savedState = sessionStorage.getItem('editorState');
            if (savedState) {
                // Merge saved state with default state to prevent errors if we add new properties later
                Object.assign(this.state, JSON.parse(savedState));
            } else {
                // If no state is saved, create a default matrix
                this.state.matrix = this._createEmptyMatrix(this.state.gridSize);
            }
        },

        saveState: function() {
            sessionStorage.setItem('editorState', JSON.stringify(this.state));
        },

        // This is the main function to draw/redraw the entire editor UI
        init: function() {
            const grid = document.getElementById('pixel-grid');
            const palette = document.getElementById('color-palette');
            if (!grid || !palette) return;

            // Update the UI controls to match the current state
            document.querySelector(`input[name="gridSize"][value="${this.state.gridSize}"]`).checked = true;

            // Set CSS Variables for the grid
            const pixelSize = this.state.gridSize === 16 ? '25px' : '50px'; // smaller pixels for 16x16
            grid.style.setProperty('--grid-size', this.state.gridSize);
            grid.style.setProperty('--pixel-size', pixelSize);

            // Clear previous content
            grid.innerHTML = '';
            palette.innerHTML = '';
            
            // Rebuild the pixel grid
            for (let y = 0; y < this.state.gridSize; y++) {
                for (let x = 0; x < this.state.gridSize; x++) {
                    const pixel = document.createElement('div');
                    pixel.className = 'pixel';
                    pixel.style.backgroundColor = this.state.matrix[y][x];
                    pixel.addEventListener('click', () => {
                        this.state.matrix[y][x] = this.state.currentColor;
                        pixel.style.backgroundColor = this.state.currentColor;
                        this.updateOutputs();
                        this.saveState();
                    });
                    grid.appendChild(pixel);
                }
            }

            // Rebuild the color palette
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
                    this.saveState();
                });
                palette.appendChild(colorBtn);
            });

            // Re-attach event listeners
            document.getElementById('clear-btn').addEventListener('click', () => this.clearGrid());
            document.getElementById('copy-btn').addEventListener('click', () => this.copyCode());
            document.getElementById('save-btn').addEventListener('click', () => this.saveToFile());
            document.getElementById('load-btn').addEventListener('click', () => this.loadFromJson());
            document.querySelectorAll('input[name="gridSize"]').forEach(radio => {
                radio.addEventListener('change', (e) => this.changeGridSize(parseInt(e.target.value)));
            });

            this.updateOutputs();
        },

        changeGridSize: function(newSize) {
            if (newSize === this.state.gridSize) return;

            if (confirm('Changing the grid size will clear your current drawing. Are you sure?')) {
                this.state.gridSize = newSize;
                this.state.matrix = this._createEmptyMatrix(newSize);
                this.saveState();
                this.init(); // Redraw everything with the new size
            } else {
                // If user cancels, revert the radio button to the current state
                document.querySelector(`input[name="gridSize"][value="${this.state.gridSize}"]`).checked = true;
            }
        },

        clearGrid: function() {
            if (confirm('Are you sure you want to clear the grid?')) {
                this.state.matrix = this._createEmptyMatrix(this.state.gridSize);
                this.saveState();
                this.init();
            }
        },

        updateOutputs: function() {
            this.updateArduinoOutput();
            this.updateJsonOutput();
        },

        updateArduinoOutput: function() {
            const size = this.state.gridSize;
            let code = `const CRGB emoji[${size}][${size}] = {\n`;
            for (let y = 0; y < size; y++) {
                code += '  { ';
                for (let x = 0; x < size; x++) {
                    const rgb = this.hexToRgb(this.state.matrix[y][x]);
                    code += `CRGB(${rgb.r}, ${rgb.g}, ${rgb.b})`;
                    if (x < size - 1) code += ', ';
                }
                code += ' }';
                if (y < size - 1) code += ',';
                code += '\n';
            }
            code += '};';
            document.getElementById('code-output').value = code;
        },

        updateJsonOutput: function() {
            const jsonData = {
                width: this.state.gridSize,
                height: this.state.gridSize,
                pixels: this.state.matrix
            };
            document.getElementById('json-output').value = JSON.stringify(jsonData, null, 2);
        },

        loadFromJson: function() {
            try {
                const jsonText = document.getElementById('json-output').value;
                if (!jsonText) throw new Error('JSON text box is empty.');
                
                const jsonData = JSON.parse(jsonText);
                if (!jsonData.width || jsonData.width !== jsonData.height || !jsonData.pixels) {
                    throw new Error('Invalid JSON format. Must contain width, height, and pixels array.');
                }
                
                // Update state from the loaded JSON
                this.state.gridSize = jsonData.width;
                this.state.matrix = jsonData.pixels;
                
                this.saveState();
                this.init(); // Redraw the entire UI with the loaded data and size
                alert(`Pattern loaded successfully with a ${jsonData.width}x${jsonData.width} grid!`);

            } catch (error) {
                alert('Error loading pattern: ' + error.message);
            }
        },
        
        // Static data & helper functions (mostly unchanged)
        colors: ['#000000', '#FFFFFF', '#FF0000', '#00FF00', '#0000FF', '#FFFF00', '#FF00FF', '#00FFFF', '#FFA500', '#800080', '#FF4500', '#DC143C', '#B22222', '#8B0000', '#CD5C5C', '#32CD32', '#228B22', '#000000', '#006400', '#ADFF2F', '#1E90FF', '#4169E1', '#000080', '#00008B', '#4682B4', '#FFD700', '#FF8C00', '#FF6347', '#FF7F50', '#DAA520', '#9932CC', '#8A2BE2', '#4B0082', '#9400D3', '#BA55D3', '#FF69B4', '#FF1493', '#DB7093', '#C71585', '#FFC0CB', '#A52A2A', '#8B4513', '#D2691E', '#CD853F', '#F4A460', '#808080', '#696969', '#2F4F4F', '#708090', '#C0C0C0', '#7CFC00', '#7FFFD4', '#F0E68C', '#E6E6FA', '#FFFACD', '#FFE4E1', '#FFE4B5', '#FAFAD2', '#F5F5DC', '#F0FFF0'],
        hexToRgb: function(hex) { return { r: parseInt(hex.substring(1, 3), 16), g: parseInt(hex.substring(3, 5), 16), b: parseInt(hex.substring(5, 7), 16) }; },
        copyCode: function() { navigator.clipboard.writeText(document.getElementById('code-output').value).then(() => alert('Arduino code copied!')); },
        saveToFile: function() {
            const dataStr = document.getElementById('json-output').value;
            const dataBlob = new Blob([dataStr], {type: "application/json"});
            const url = URL.createObjectURL(dataBlob);
            const linkElement = document.createElement('a');
            linkElement.setAttribute('href', url);
            linkElement.setAttribute('download', `pixel_art_${this.state.gridSize}x${this.state.gridSize}.json`);
            document.body.appendChild(linkElement);
            linkElement.click();
            document.body.removeChild(linkElement);
        }
    };
    
    // Load state ONCE when the object is first created
    window.EmojiEditor.loadState();
}

// Entry Point: This runs every time the editor page is loaded
window.EmojiEditor.init();