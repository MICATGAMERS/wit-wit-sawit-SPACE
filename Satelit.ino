#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_BMP085.h>
#include "esp_camera.h"

// ===== BMP180 =====
Adafruit_BMP085 bmp;
bool bmpReady = false;

// ===== LoRa =====
#define SS 5
#define RST 14
#define DIO0 26

// ===== CAMERA CONFIG AI Thinker =====
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     27
#define SIOC_GPIO_NUM     25
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     26

bool camReady = false;
unsigned long lastCapture = 0;
unsigned long lastTransmit = 0;

const unsigned long captureInterval = 20000;   // Foto tiap 20 detik (simulasi)
const unsigned long transmitInterval = 8000;   // Kirim data tiap 8 detik

String riskLevel = "LOW";

void setupCamera() {
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (esp_camera_init(&config) == ESP_OK) {
    camReady = true;
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(14, 15);

  if (bmp.begin()) bmpReady = true;

  LoRa.setPins(SS, RST, DIO0);
  LoRa.begin(433E6);

  setupCamera();

  Serial.println("WIT WIT SAWIT SPACE SATELLITE READY 🚀");
}

void loop() {

  float temperature = bmpReady ? bmp.readTemperature() : 0;
  float pressure = bmpReady ? bmp.readPressure() / 100.0 : 0;

  // ===== RISK LOGIC =====
  if (temperature > 70) riskLevel = "HIGH";
  else if (temperature > 40) riskLevel = "MEDIUM";
  else riskLevel = "LOW";

  // ===== TRANSMIT TELEMETRY =====
  if (millis() - lastTransmit > transmitInterval) {
    lastTransmit = millis();

    LoRa.beginPacket();
    LoRa.print("Temp:");
    LoRa.print(temperature);
    LoRa.print(",Pressure:");
    LoRa.print(pressure);
    LoRa.print(",Risk:");
    LoRa.print(riskLevel);
    LoRa.endPacket();

    Serial.println("📡 Data Sent to Earth");
  }

  // ===== CAMERA SNAPSHOT =====
  if (millis() - lastCapture > captureInterval) {
    lastCapture = millis();

    if (camReady) {
      camera_fb_t *fb = esp_camera_fb_get();

      if (fb) {
        Serial.println("📸 Forest Image Captured");
        esp_camera_fb_return(fb);
      } else {
        Serial.println("⚠ Camera Capture Failed (Simulator)");
      }
    }
  }

  delay(2000);
}
