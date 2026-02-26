#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "esp_camera.h"

Adafruit_BMP085 bmp;

bool bmpReady = false;
bool camReady = false;

unsigned long lastCapture = 0;
const unsigned long captureInterval = 15000;

String riskLevel = "LOW";

// ===== CAMERA CONFIG AI Thinker =====
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

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

  esp_err_t err = esp_camera_init(&config);

  if (err == ESP_OK) {
    camReady = true;
  }
}

void bootIntro() {
  Serial.println();
  Serial.println("==================================");
  Serial.println("   WIT WIT SAWIT SPACE SYSTEM");
  Serial.println("==================================");
  delay(800);

  Serial.println("Booting Satellite Firmware...");
  delay(800);

  Serial.println("Initializing Modules...");
  delay(800);

  Serial.print("BMP180 Sensor: ");
  if (bmpReady) Serial.println("READY ✅");
  else Serial.println("NOT DETECTED ❌");
  delay(500);

  Serial.print("Camera Module: ");
  if (camReady) Serial.println("READY ✅");
  else Serial.println("FAILED ❌ (Simulator OK)");
  delay(500);

  Serial.println("----------------------------------");
  Serial.println("System Status: OPERATIONAL 🚀");
  Serial.println("----------------------------------");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(14, 15);

  if (bmp.begin()) {
    bmpReady = true;
  }

  setupCamera();
  bootIntro();
}

void loop() {

  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0;

  if (temperature > 70) riskLevel = "HIGH";
  else if (temperature > 40) riskLevel = "MEDIUM";
  else riskLevel = "LOW";

  Serial.println("---- TELEMETRY ----");
  Serial.print("Temp: ");
  Serial.println(temperature);
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.print("Risk: ");
  Serial.println(riskLevel);

  if (millis() - lastCapture > captureInterval) {
    lastCapture = millis();

    if (!camReady) {
      Serial.println("⚠ Camera Offline (Simulated)");
    } else {
      camera_fb_t *fb = esp_camera_fb_get();

      if (!fb) {
        Serial.println("⚠ Capture Failed (Simulator)");
      } else {
        Serial.println("📸 Forest Snapshot Captured!");
        Serial.print("Image Size: ");
        Serial.println(fb->len);
        esp_camera_fb_return(fb);
      }
    }
  }

  delay(3000);
}
