// Ficheiro: js/websocket_service.js

// Padrão Singleton para o nosso serviço global de WebSocket
if (!window.WebSocketService) {
    window.WebSocketService = {
        ws: null,
        ip: '10.0.1.142', // IP por defeito ou o último usado
        status: 'Desconectado',
        listeners: [], // Lista de funções (callbacks) para notificar sobre mudanças de estado

        /**
         * Adiciona uma função à lista de "ouvintes".
         * Sempre que o estado da conexão mudar, todas as funções nesta lista serão chamadas.
         */
        addListener: function(callback) {
            this.listeners.push(callback);
        },

        /**
         * Notifica todos os "ouvintes" sobre o estado atual.
         */
        _notifyListeners: function() {
            this.listeners.forEach(callback => callback(this.status, this.ws?.readyState === WebSocket.OPEN));
        },
        
        /**
         * Atualiza o estado e notifica toda a gente.
         */
        _updateStatus: function(newStatus) {
            this.status = newStatus;
            this._notifyListeners();
        },

        /**
         * Inicia a conexão WebSocket.
         */
        connect: function(newIp) {
            if (newIp) {
                this.ip = newIp;
            }
            
            if (this.ws) {
                this.ws.close();
            }

            this._updateStatus('Conectando...');
            const wsUrl = `ws://${this.ip}:81/`;
            this.ws = new WebSocket(wsUrl);

            this.ws.onopen = () => {
                this._updateStatus('Conectado');
            };

            this.ws.onclose = () => {
                this._updateStatus('Desconectado');
                this.ws = null;
            };

            this.ws.onerror = () => {
                this._updateStatus('Erro na conexão');
                this.ws = null;
            };

            // NOVO: Adiciona o handler para mensagens recebidas
            this.ws.onmessage = (event) => {
                const message = event.data;
                console.log('Mensagem recebida do dispositivo:', message);

                // Verifica se é uma mensagem de identificação de dispositivo
                if (message.startsWith('DEVICE:')) {
                    const deviceName = message.split(':')[1].trim();
                    const pageUrl = `pages/${deviceName}.html`;

                    // Dispara um evento global para notificar a aplicação para navegar
                    const navigateEvent = new CustomEvent('navigate', {
                        detail: {
                            url: pageUrl,
                            deviceName: deviceName
                        }
                    });
                    window.dispatchEvent(navigateEvent);
                }
                
                // Pode adicionar mais lógica aqui para outras mensagens
            };
        },

        /**
         * Envia um comando através do WebSocket.
         */
        send: function(command) {
            if (this.ws && this.ws.readyState === WebSocket.OPEN) {
                this.ws.send(command);
                return true; // Sucesso
            } else {
                this._updateStatus('Não conectado');
                return false; // Falha
            }
        },
        
        /**
         * Retorna o estado atual da conexão
         */
        getConnectionStatus: function() {
             return { status: this.status, isConnected: this.ws?.readyState === WebSocket.OPEN };
        }
    };
}