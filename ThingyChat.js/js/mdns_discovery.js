if (!window.MdnsDiscoveryPage) {
    window.MdnsDiscoveryPage = {
        init: function() {
            const statusEl = document.getElementById('status');
            const deviceListEl = document.getElementById('deviceList');
            const refreshBtn = document.getElementById('refreshBtn');
            const manualBtn = document.getElementById('manualBtn');
            const manualIpBtn = document.getElementById('manualIpBtn');
            const manualIpInput = document.getElementById('manualIpInput');

            const updateDeviceList = (devices) => {
                deviceListEl.innerHTML = '';
                if (devices.length === 0) {
                    statusEl.textContent = 'Nenhum dispositivo encontrado.';
                    statusEl.style.color = 'red';
                    return;
                }
                statusEl.textContent = 'Dispositivos encontrados:';
                statusEl.style.color = 'green';
                devices.forEach(device => {
                    const deviceItem = document.createElement('div');
                    deviceItem.className = 'device-item';
                    deviceItem.textContent = `${device.name} (${device.host})`;
                    deviceItem.addEventListener('click', () => {
                        window.location.href = `/pages/websocket.html?host=${encodeURIComponent(device.host)}`;
                    });
                    deviceListEl.appendChild(deviceItem);
                });
            };

            const discoverDevices = async () => {
                statusEl.textContent = 'Procurando dispositivos...';
                statusEl.style.color = 'black';
                try {
                    const controller = new AbortController();
                    const timeoutId = setTimeout(() => controller.abort(), 5000);
                    const response = await fetch('http://localhost:3000/discover', { signal: controller.signal });
                    clearTimeout(timeoutId);
                    const devices = await response.json();
                    updateDeviceList(devices);
                } catch (error) {
                    console.error('Erro ao buscar dispositivos:', error);
                    statusEl.textContent = 'Erro ao procurar dispositivos.';
                    statusEl.style.color = 'red';
                    updateDeviceList([]);
                }
            };

            refreshBtn.addEventListener('click', discoverDevices);
            manualBtn.addEventListener('click', () => {
                window.location.href = '/pages/websocket.html';
            });
            manualIpBtn.addEventListener('click', () => {
                const ip = manualIpInput.value.trim();
                if (ip) {
                    window.location.href = `/pages/websocket.html?host=${encodeURIComponent(ip)}`;
                } else {
                    alert('Por favor, insira um IP válido.');
                }
            });

            discoverDevices();
        }
    };
}

window.MdnsDiscoveryPage.init();