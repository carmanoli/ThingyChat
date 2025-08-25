// File: js/websocket_service.js (CLEANED AND SIMPLIFIED)
if (!window.WebSocketService) {
    window.WebSocketService = {
        ws: null,
        ip: 'emoji16_display.local', // Default device
        status: 'Disconnected',
        listeners: [],

        addListener: function(callback) {
            if (!this.listeners.includes(callback)) {
                this.listeners.push(callback);
            }
        },

        _notifyListeners: function() {
            this.listeners.forEach(callback => callback(this.status, this.ws?.readyState === WebSocket.OPEN));
        },

        _updateStatus: function(newStatus) {
            this.status = newStatus;
            this._notifyListeners();
        },

        connect: function(host) {
            if (host) {
                this.ip = host;
            }

            if (this.ws) {
                this.ws.close();
            }

            this._updateStatus('Connecting...');
            const wsUrl = `ws://${this.ip}/ws`;
            console.log(`Attempting to connect to: ${wsUrl}`);

            this.ws = new WebSocket(wsUrl);

            this.ws.onopen = () => {
                this._updateStatus('Connected');
            };

            this.ws.onclose = () => {
                this._updateStatus('Disconnected');
                this.ws = null;
            };

            this.ws.onerror = (error) => {
                console.error('WebSocket Error:', error);
                this._updateStatus('Connection Error');
                this.ws = null;
            };

// Ficheiro: js/websocket_service.js (Substituir esta função)

            this.ws.onmessage = (event) => {
                const message = event.data;
                
                // Primeiro, tenta analisar a mensagem como JSON
                let data;
                try {
                    data = JSON.parse(message);
                } catch (e) {
                    // Se não for JSON, trata como texto simples (para o DEVICE:)
                    if (message.startsWith('DEVICE:')) {
                        const deviceName = message.split(':')[1].trim();
                        
                        // Dispara o evento de navegação para a página do dispositivo
                        const navigateEvent = new CustomEvent('navigate', {
                            detail: { url: `pages/${deviceName}.html`, deviceName: deviceName }
                        });
                        window.dispatchEvent(navigateEvent);
                    } else {
                        console.log("Received plain text message:", message);
                    }
                    return; // Ignora outras mensagens que não sejam JSON ou DEVICE:
                }

                // Se for um JSON válido, verifica o tipo
                if (data.type === 'pixel_photo') {
                    console.log('Pixel photo received and stored temporarily.', data);
                    
                    // --- A LÓGICA CORRETA ESTÁ AQUI ---

                    // 1. Guarda os dados na variável temporária
                    this.pendingPhotoData = data;

                    // 2. DISPARA O EVENTO 'photoready' para notificar a UI
                    window.dispatchEvent(new CustomEvent('photoready'));

                    // 3. Notifica o utilizador
                    alert('Photo received from device!\nNavigate to the "Animation Editor" and click the "Paste Photo" button.');
                }
            };
        },

        send: function(command) {
            if (this.ws && this.ws.readyState === WebSocket.OPEN) {
                this.ws.send(command);
                return true;
            } else {
                this._updateStatus('Not Connected');
                return false;
            }
        },

        getConnectionStatus: function() {
            return { status: this.status, isConnected: this.ws?.readyState === WebSocket.OPEN };
        }
    };
}