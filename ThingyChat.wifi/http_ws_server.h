// File: http_ws_server.h

#ifndef HTTP_WS_SERVER_H
#define HTTP_WS_SERVER_H

#include <Arduino.h>

// Initializes the combined HTTP and WebSocket server.
void initHttpAndWsServer(String deviceName);

// The loop function for the server (needed for WebSocketsServer, but not for Async).
void serverLoop();

#endif