const bonjour = require('bonjour')();
const express = require('express');
const app = express();
const port = 3000;

// List to store discovered devices
let devices = [];

// Set CSP headers
app.use((req, res, next) => {
    res.setHeader('Content-Security-Policy', "default-src 'self'; connect-src 'self' ws://10.0.1.174:81 ws://*.local:81; style-src 'self' 'unsafe-inline'; script-src 'self'");
    next();
});

// Find _ws._tcp services
const browser = bonjour.find({ type: 'ws' }, (service) => {
    console.log('Device found:', JSON.stringify(service, null, 2));
    devices = devices.filter(d => d.host !== service.host);
    const ip = service.addresses.find(addr => addr.includes('.')) || 'Unknown IP';
    devices.push({
        name: service.name,
        host: service.host,
        ip: ip,
        port: service.port
    });
});

// Handle service removal
browser.on('down', (service) => {
    console.log('Device removed:', JSON.stringify(service, null, 2));
    devices = devices.filter(d => d.name !== service.name);
});

// Handle errors
browser.on('error', (error) => {
    console.error('Bonjour error:', error);
});

// Log when browser starts
console.log('Starting mDNS discovery for _ws._tcp services...');

// API to list devices
app.get('/discover', (req, res) => {
    console.log('Received /discover request. Current devices:', JSON.stringify(devices, null, 2));
    res.json(devices);
});

// API to resolve mDNS name
app.get('/resolve', (req, res) => {
    const host = req.query.host;
    console.log(`Received /resolve request for host: ${host}`);
    const device = devices.find(d => d.host === host);
    if (device) {
        res.json({ ip: device.ip });
    } else {
        res.json({ error: 'Device not found' });
    }
});

// Serve static files from ThingyChat.js
app.use(express.static('../ThingyChat.js'));

app.listen(port, '127.0.0.1', () => {
    console.log(`Proxy server running at http://localhost:${port}`);
});

// Periodically log devices for debugging
setInterval(() => {
    console.log('Current devices:', JSON.stringify(devices, null, 2));
}, 10000);

// Refresh mDNS discovery
setInterval(() => {
    console.log('Refreshing mDNS discovery...');
    browser.update();
}, 30000);