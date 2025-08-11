#ifndef WS_SERVER_H
#define WS_SERVER_H

#include <WebSocketsServer.h>

extern WebSocketsServer webSocket;

void initWebSocket();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

#endif