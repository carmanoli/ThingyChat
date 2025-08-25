#include <ESPAsyncWebServer.h>
#include "Camera.h"
#include "esp_camera.h"
#include <Arduino.h>

// --- CONFIGURAÇÃO DOS PINOS DA CÂMARA PARA O XIAO ESP32-S3 SENSE ---
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 10
#define SIOD_GPIO_NUM 40
#define SIOC_GPIO_NUM 39
#define Y9_GPIO_NUM 48
#define Y8_GPIO_NUM 11
#define Y7_GPIO_NUM 12
#define Y6_GPIO_NUM 14
#define Y5_GPIO_NUM 16
#define Y4_GPIO_NUM 18
#define Y3_GPIO_NUM 17
#define Y2_GPIO_NUM 15
#define VSYNC_GPIO_NUM 38
#define HREF_GPIO_NUM 47
#define PCLK_GPIO_NUM 13

// Objeto para a configuração da câmara
static camera_config_t camera_config;

bool Camera::begin() {
  Serial.println("Iniciando câmera...");
  Serial.println("  > Configurando pinos da câmera...");
  camera_config.ledc_channel = LEDC_CHANNEL_0;
  camera_config.ledc_timer = LEDC_TIMER_0;
  camera_config.pin_d0 = Y2_GPIO_NUM;
  camera_config.pin_d1 = Y3_GPIO_NUM;
  camera_config.pin_d2 = Y4_GPIO_NUM;
  camera_config.pin_d3 = Y5_GPIO_NUM;
  camera_config.pin_d4 = Y6_GPIO_NUM;
  camera_config.pin_d5 = Y7_GPIO_NUM;
  camera_config.pin_d6 = Y8_GPIO_NUM;
  camera_config.pin_d7 = Y9_GPIO_NUM;
  camera_config.pin_xclk = XCLK_GPIO_NUM;
  camera_config.pin_pclk = PCLK_GPIO_NUM;
  camera_config.pin_vsync = VSYNC_GPIO_NUM;
  camera_config.pin_href = HREF_GPIO_NUM;
  camera_config.pin_sscb_sda = SIOD_GPIO_NUM;
  camera_config.pin_sscb_scl = SIOC_GPIO_NUM;
  camera_config.pin_pwdn = PWDN_GPIO_NUM;
  camera_config.pin_reset = RESET_GPIO_NUM;
  camera_config.xclk_freq_hz = 20000000;
  camera_config.pixel_format = PIXFORMAT_JPEG;
  camera_config.frame_size = FRAMESIZE_QVGA;
  camera_config.jpeg_quality = 12;
  camera_config.fb_count = 1;

  Serial.println("  > Verificando pinos de configuração...");
  Serial.printf("    > PWDN: %d, RESET: %d, XCLK: %d, SIOD: %d, SIOC: %d\n",
                PWDN_GPIO_NUM, RESET_GPIO_NUM, XCLK_GPIO_NUM, SIOD_GPIO_NUM, SIOC_GPIO_NUM);
  Serial.printf("    > Y2-Y9: %d, %d, %d, %d, %d, %d, %d, %d\n",
                Y2_GPIO_NUM, Y3_GPIO_NUM, Y4_GPIO_NUM, Y5_GPIO_NUM,
                Y6_GPIO_NUM, Y7_GPIO_NUM, Y8_GPIO_NUM, Y9_GPIO_NUM);
  Serial.printf("    > VSYNC: %d, HREF: %d, PCLK: %d\n",
                VSYNC_GPIO_NUM, HREF_GPIO_NUM, PCLK_GPIO_NUM);

  Serial.println("  > Chamando esp_camera_init...");
  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK) {
    Serial.printf("  > ERRO: Inicialização da câmera falhou com erro 0x%x\n", err);
    switch (err) {
      case ESP_ERR_NO_MEM:
        Serial.println("    > Causa: Memória insuficiente");
        break;
      case ESP_ERR_INVALID_ARG:
        Serial.println("    > Causa: Configuração inválida");
        break;
      case ESP_FAIL:
        Serial.println("    > Causa: Falha ao acessar o sensor (I2C ou hardware)");
        break;
      default:
        Serial.println("    > Causa: Erro desconhecido");
        break;
    }
    return false;
  }

  Serial.println("  > Verificando sensor após inicialização...");
  sensor_t *s = esp_camera_sensor_get();
  if (!s) {
    Serial.println("  > ERRO: Sensor da câmera não detectado após inicialização!");
    return false;
  }
  Serial.printf("  > Sensor detectado, ID: 0x%x\n", s->id.PID);
  Serial.println("Câmara iniciada com sucesso!");
  return true;
}

void Camera::captureAndSendPixelated(int gridSize, AsyncWebSocketClient *client) {
  Serial.println("\n--- Início do Processo de Captura ---");
  Serial.printf("  > Client ID: %u, Grid Size: %d\n", client->id(), gridSize);
  Serial.printf("  > Memória livre inicial: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

  // Verificar estado do cliente WebSocket
  if (!client || client->status() != WS_CONNECTED) {
    Serial.println("!!! ERRO: Cliente WebSocket não está conectado!");
    return;
  }

  // 1. Tira a foto
  Serial.println("Passo 1: A tentar capturar o frame buffer (FB)...");
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("!!! ERRO: Falha ao capturar a imagem (esp_camera_fb_get retornou nulo).");
    Serial.println("  > Verificando estado da câmera...");
    sensor_t *s = esp_camera_sensor_get();
    if (s) {
      Serial.printf("  > Sensor ativo, ID: 0x%x\n", s->id.PID);
    } else {
      Serial.println("  > Sensor da câmera não acessível!");
    }
    client->text("ERROR:Capture failed on device.");
    return;
  }
  Serial.printf("  > Sucesso! Imagem JPEG capturada: %d bytes, Resolução: %dx%d\n", fb->len, fb->width, fb->height);

  // 2. Aloca memória para a imagem de destino (bitmap RGB565)
  size_t rgb_buf_len = fb->width * fb->height * sizeof(uint16_t);
  Serial.printf("Passo 2: A tentar alocar %d bytes de memória para o bitmap RGB...\n", rgb_buf_len);
  uint16_t *rgb565_image = (uint16_t *)ps_malloc(rgb_buf_len);
  if (!rgb565_image) {
      Serial.println("!!! ERRO: Falha ao alocar memória para a imagem RGB (ps_malloc retornou nulo).");
      esp_camera_fb_return(fb);
      client->text("ERROR:Device out of memory.");
      return;
  }
  Serial.println("  > Sucesso! Memória alocada.");

  // 3. Descodifica o JPEG para o formato RGB565
  Serial.println("Passo 3: A descodificar JPEG para RGB565...");
  bool jpeg_converted = jpg2rgb565(fb->buf, fb->len, (uint8_t*)rgb565_image, JPG_SCALE_NONE);
  
  // 4. Liberta a memória do buffer JPEG original
  esp_camera_fb_return(fb);
  Serial.println("  > Buffer JPEG original libertado.");

  if (!jpeg_converted) {
      Serial.println("!!! ERRO: Falha na conversão do JPEG.");
      free(rgb565_image);
      client->text("ERROR:JPEG conversion failed on device.");
      return;
  }
  Serial.println("  > Sucesso! JPEG descodificado.");

  // 5. Processa a imagem (Algoritmo de Pixelização)
  Serial.printf("Passo 4: A pixelizar a imagem para %dx%d...\n", gridSize, gridSize);
  String pixelsJson = "[";
  int blockWidth = fb->width / gridSize;
  int blockHeight = fb->height / gridSize;

  for (int y_block = 0; y_block < gridSize; y_block++) {
    pixelsJson += "[";
    for (int x_block = 0; x_block < gridSize; x_block++) {
      unsigned long sumR = 0, sumG = 0, sumB = 0;
      int startX = x_block * blockWidth;
      int startY = y_block * blockHeight;

      for (int y = 0; y < blockHeight; y++) {
        for (int x = 0; x < blockWidth; x++) {
          uint16_t pixel = rgb565_image[(startY + y) * fb->width + (startX + x)];
          sumR += (pixel >> 11) & 0x1F;
          sumG += (pixel >> 5) & 0x3F;
          sumB += pixel & 0x1F;
        }
      }
      
      int numPixelsInBlock = blockWidth * blockHeight;
      uint8_t avgR = (sumR / numPixelsInBlock) << 3;
      uint8_t avgG = (sumG / numPixelsInBlock) << 2;
      uint8_t avgB = (sumB / numPixelsInBlock) << 3;

      char hexColor[8];
      sprintf(hexColor, "\"#%02X%02X%02X\"", avgR, avgG, avgB);
      pixelsJson += hexColor;

      if (x_block < gridSize - 1) pixelsJson += ",";
    }
    pixelsJson += "]";
    if (y_block < gridSize - 1) pixelsJson += ",";
  }
  pixelsJson += "]";
  Serial.println("  > Sucesso! Pixelização concluída.");

  // 6. Liberta a memória do bitmap RGB
  free(rgb565_image);
  Serial.println("  > Memória do bitmap RGB libertada.");

  // 7. Envia o resultado para a aplicação web
  Serial.println("Passo 5: A construir e a enviar a resposta JSON...");
  String response = "{\"type\":\"pixel_photo\",\"width\":" + String(gridSize) + ",\"height\":" + String(gridSize) + ",\"pixels\":" + pixelsJson + "}";
  client->text(response);
  Serial.println("  > Sucesso! Resposta enviada para a app web.");
  Serial.println("--- Fim do Processo de Captura ---\n");
}