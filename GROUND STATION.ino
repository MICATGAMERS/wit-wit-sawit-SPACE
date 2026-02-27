#include <SPI.h>
#include <LoRa.h>

#define SS 5
#define RST 14
#define DIO0 26

#define LED_GREEN 2
#define LED_RED 4
#define BUZZER 15

void setup() {
  Serial.begin(115200);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  LoRa.setPins(SS, RST, DIO0);
  LoRa.begin(433E6);

  Serial.println("WIT WIT SAWIT SPACE GROUND CONTROL 🌍");
}

void loop() {

  int packetSize = LoRa.parsePacket();

  if (packetSize) {

    String incoming = "";

    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    Serial.println("📡 Data Received:");
    Serial.println(incoming);

    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER, LOW);

    if (incoming.indexOf("HIGH") >= 0) {

      Serial.println("🔥 FIRE ALERT 🔥");
      digitalWrite(LED_RED, HIGH);
      digitalWrite(BUZZER, HIGH);

    } else if (incoming.indexOf("MEDIUM") >= 0) {

      Serial.println("⚠ MEDIUM RISK ⚠");
      digitalWrite(LED_RED, HIGH);

    } else {

      Serial.println("✅ SAFE");
      digitalWrite(LED_GREEN, HIGH);
    }
  }
}
