// wifi_ap.cpp
#include "wifi_ap.h"
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "wifi_config.h"

// Definição das variáveis declaradas como extern em wifi_ap.h
WebServer server(80);
DNSServer dnsServer;

void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html lang="pt">
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>ThingyChat WiFi Setup</title>
      <style>
        body { font-family: Arial, sans-serif; background-color: #f4f4f4; margin: 40px; text-align: center; }
        div { background-color: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); display: inline-block; }
        h1 { color: #333; }
        input[type="text"], input[type="password"] { width: 90%; padding: 10px; margin: 10px 0; border: 1px solid #ddd; border-radius: 4px; }
        input[type="submit"] { background-color: #007bff; color: white; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer; font-size: 16px; }
        input[type="submit"]:hover { background-color: #0056b3; }
      </style>
    </head>
    <body>
      <div>
        <h1>Configurar Wi-Fi - ThingyChat</h1>
        <form action="/save" method="POST">
          <label for="ssid">Nome da Rede (SSID):</label><br>
          <input type="text" id="ssid" name="ssid" placeholder="Ex: MinhaCasa_WiFi" required><br>
          <label for="pass">Password:</label><br>
          <input type="password" id="pass" name="pass" placeholder="A sua password de Wi-Fi"><br><br>
          <input type="submit" value="Guardar e Reiniciar">
        </form>
      </div>
    </body>
    </html>
  )rawliteral";
  
  server.send(200, "text/html", html);
}

void handleSave() {
  Serial.println("[AP] Recebido pedido para guardar credenciais.");
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");
  saveWiFiCredentials(ssid, pass);
  String htmlResponse = R"rawliteral(
    <!DOCTYPE html><html><head><title>Guardado!</title></head><body>
    <h1>Credenciais guardadas com sucesso!</h1>
    <p>O dispositivo vai reiniciar e tentar conectar-se à rede ' )rawliteral" + ssid + R"rawliteral('. Por favor, volte a ligar-se à sua rede Wi-Fi normal.</p>
    </body></html>
  )rawliteral";
  server.send(200, "text/html", htmlResponse);
  delay(1000);
  ESP.restart();
}

void startAPMode() {
  Serial.println("[AP] A iniciar modo Access Point...");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  bool success = WiFi.softAP("ThingyChat-Setup", "password");
  if (!success) {
    Serial.println("[AP] Falha ao criar AP");
    return;
  }
  IPAddress apIP(192, 168, 4, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  dnsServer.start(53, "*", apIP);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.onNotFound([]() {
    server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString(), true);
    server.send(302, "text/plain", "");
  });
  server.begin();
  Serial.print("[AP] Ponto de acesso 'ThingyChat-Setup' criado. Conecte-se a esta rede.\n");
  Serial.print("[AP] Para configurar, acesse: http://");
  Serial.println(apIP);
}

void handleAPClient() {
  server.handleClient();
  dnsServer.processNextRequest();
}