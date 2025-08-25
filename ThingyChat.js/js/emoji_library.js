// Ficheiro: js/emoji_library.js (Versão Final e Completa)

if (!window.LibraryPage) {
    window.LibraryPage = {
        
        drawFrameToCanvas: function(canvas, frameData, gridSize) {
            const ctx = canvas.getContext('2d');
            const pixelSize = canvas.width / gridSize;
            ctx.clearRect(0, 0, canvas.width, canvas.height);

            frameData.pixels.forEach((row, y) => {
                row.forEach((color, x) => {
                    if (color) {
                        ctx.fillStyle = color;
                        ctx.fillRect(x * pixelSize, y * pixelSize, pixelSize, pixelSize);
                    }
                });
            });
        },

        renderLibraryItem: function(item, container) {
            const card = document.createElement('div');
            card.className = 'library-card';
            const name = document.createElement('h5');
            name.textContent = item.fileName;
            card.appendChild(name);
            const previewContainer = document.createElement('div');
            previewContainer.className = 'library-card-preview';

            switch (item.type) {
                case 'animation':
                case 'static':
                    const canvas = document.createElement('canvas');
                    const resolution = item.data.width > 8 ? 160 : 80; // Resolução maior para 16x16
                    canvas.width = resolution;
                    canvas.height = resolution;
                    previewContainer.appendChild(canvas);
                    const firstFrame = item.data.frames ? item.data.frames[0] : { pixels: item.data.pixels };
                    if (firstFrame) { this.drawFrameToCanvas(canvas, firstFrame, item.data.width); }
                    if (item.type === 'animation') {
                        canvas.animationData = { frames: item.data.frames, gridSize: item.data.width, currentFrame: 0, lastTime: 0, animationId: null };
                        card.addEventListener('mouseenter', () => this.startAnimation(canvas));
                        card.addEventListener('mouseleave', () => this.stopAnimation(canvas));
                    }
                    break;
                case 'png':
                case 'gif':
                    const img = document.createElement('img');
                    img.src = item.url;
                    img.alt = item.fileName;
                    previewContainer.appendChild(img);
                    break;
            }
            card.appendChild(previewContainer);
            container.appendChild(card);
        },

        startAnimation: function(canvas) {
            const anim = canvas.animationData;
            if (anim.animationId) return;
            const animate = (timestamp) => {
                if (!anim.lastTime) anim.lastTime = timestamp;
                const elapsed = timestamp - anim.lastTime;
                const frameDuration = anim.frames[anim.currentFrame].duration;
                if (elapsed > frameDuration) {
                    anim.currentFrame = (anim.currentFrame + 1) % anim.frames.length;
                    this.drawFrameToCanvas(canvas, anim.frames[anim.currentFrame], anim.gridSize);
                    anim.lastTime = timestamp;
                }
                anim.animationId = requestAnimationFrame(animate);
            };
            anim.animationId = requestAnimationFrame(animate);
        },

        stopAnimation: function(canvas) {
            const anim = canvas.animationData;
            if (anim.animationId) {
                cancelAnimationFrame(anim.animationId);
                anim.animationId = null;
                anim.currentFrame = 0;
                anim.lastTime = 0;
                this.drawFrameToCanvas(canvas, anim.frames[0], anim.gridSize);
            }
        },

 displayLibrary: function() {
            const gridContainer = document.getElementById('library-grid');
            if (!gridContainer) return; // Se o elemento não existir, sai
            gridContainer.innerHTML = ''; 
            
            // --- A CORREÇÃO ESTÁ AQUI ---
            
            // 1. Verifica se o serviço e a função existem antes de os chamar
            if (!window.EmojiLibraryService || typeof window.EmojiLibraryService.getLibraryItems !== 'function') {
                console.error("EmojiLibraryService não está disponível.");
                gridContainer.innerHTML = '<p class="text-center text-danger">Error: Library Service failed to load.</p>';
                return;
            }

            const items = window.EmojiLibraryService.getLibraryItems();

            // 2. Verifica se o resultado é um array válido antes de tentar ler o '.length'
            // A verificação `!items` apanha o caso de 'undefined' ou 'null'.
            if (!items || items.length === 0) {
                gridContainer.innerHTML = '<p class="text-center">Your library is empty or no folder has been selected. Use "Library Settings" to select a folder.</p>';
                return;
            }

            // Se tudo estiver bem, continua a renderizar os itens
            items.forEach(item => {
                this.renderLibraryItem(item, gridContainer);
            });
        },
    };
}

// Padrão Singleton para os event listeners
if (!window.LibraryPageInitialized) {
    const refreshBtn = document.getElementById('refreshBtn');
    if (refreshBtn) {
        refreshBtn.addEventListener('click', () => {
            if (window.EmojiLibraryService && window.EmojiLibraryService.directoryHandle) {
                window.EmojiLibraryService.loadLibraryContents();
            } else {
                alert("Please select a library folder first in 'Library Settings'.");
            }
        });
    }
    
    window.addEventListener('libraryupdated', () => window.LibraryPage.displayLibrary());
    
    window.LibraryPageInitialized = true;
}

// Corre sempre que a página é carregada
window.LibraryPage.displayLibrary();