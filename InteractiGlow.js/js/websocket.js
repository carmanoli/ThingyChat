// Ficheiro: js/websocket.js (versão refatorada)

if (!window.RemoteControlPage) {
    window.RemoteControlPage = {
        init: function() {
            // Elementos da UI desta página
            const statusEl = document.getElementById('status');
            const ipInput = document.getElementById('ipInput');
            
            // Função que atualiza a UI desta página com base no estado do serviço
            const updateUI = (statusMessage, isConnected) => {
                statusEl.textContent = statusMessage;
                statusEl.style.color = isConnected ? 'green' : 'red';
                ipInput.value = window.WebSocketService.ip;
            };

            // 1. "Inscreve-se" para receber atualizações do serviço central
            window.WebSocketService.addListener(updateUI);

            // 2. Adiciona eventos aos botões que chamam o serviço central
            document.getElementById('connectBtn').addEventListener('click', () => {
                window.WebSocketService.connect(ipInput.value.trim());
            });
            document.getElementById('heartBtn').addEventListener('click', () => this.sendCommand('HEART'));
            document.getElementById('crossBtn').addEventListener('click', () => this.sendCommand('CROSS'));
            document.getElementById('smileBtn').addEventListener('click', () => this.sendCommand('SMILE'));
            document.getElementById('offBtn').addEventListener('click', () => this.sendCommand('OFF'));
            
            // 3. Pede ao serviço o estado atual para sincronizar a UI imediatamente
            const initialState = window.WebSocketService.getConnectionStatus();
            updateUI(initialState.status, initialState.isConnected);
        },

        sendCommand: function(cmd) {
            if (!window.WebSocketService.send(cmd)) {
                alert('Falha ao enviar comando. Verifique a conexão.');
            }
        }
    };
}

window.RemoteControlPage.init();