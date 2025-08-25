// File: js/websocket.js (CLEANED)

if (!window.RemoteControlPage) {
    window.RemoteControlPage = {
        init: function() {
            const statusEl = document.getElementById('status');
            const ipInput = document.getElementById('ipInput');

            // Prefill with current WebSocket IP (readonly for display)
            ipInput.value = window.WebSocketService.ip;

            const updateUI = (statusMessage, isConnected) => {
                statusEl.textContent = statusMessage;
                statusEl.style.color = isConnected ? 'green' : 'red';
                ipInput.value = window.WebSocketService.ip;
            };

            window.WebSocketService.addListener(updateUI);

            document.getElementById('connectBtn').addEventListener('click', () => {
                window.WebSocketService.connect(ipInput.value.trim());
            });
            document.getElementById('heartBtn')?.addEventListener('click', () => this.sendCommand('HEART'));
            document.getElementById('crossBtn')?.addEventListener('click', () => this.sendCommand('CROSS'));
            document.getElementById('smileBtn')?.addEventListener('click', () => this.sendCommand('SMILE'));
            document.getElementById('offBtn')?.addEventListener('click', () => this.sendCommand('OFF'));

            const initialState = window.WebSocketService.getConnectionStatus();
            updateUI(initialState.status, initialState.isConnected);

            if (!initialState.isConnected) {
                window.WebSocketService.connect(ipInput.value.trim());
            }
        },

        sendCommand: function(cmd) {
            if (!window.WebSocketService.send(cmd)) {
                alert('Failed to send command. Please check the connection.');
            }
        }
    };
}

window.RemoteControlPage.init();