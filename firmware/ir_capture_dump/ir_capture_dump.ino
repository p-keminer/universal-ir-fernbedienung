#include <Arduino.h>

#include "../shared/CaptureStorage.h"
#include "../shared/StatusLed.h"

constexpr uint32_t kBaudRate = 115200;
constexpr uint16_t kBootDelayMs = 500;

status_led::StatusLed statusLed(status_led::kRgbLedPin);
capture_storage::CaptureStorage captureStorage;

void setup() {
  Serial.begin(kBaudRate);
  delay(kBootDelayMs);

  statusLed.begin();
  statusLed.showBoot();

  Serial.println();
  Serial.println(F("IR-Capture-Speicher auslesen"));
  Serial.print(F("Log-Datei: "));
  Serial.println(capture_storage::kCaptureLogPath);

  if (!captureStorage.begin(false)) {
    Serial.println(F("FFat konnte nicht geoeffnet werden."));
    statusLed.showStorageError();
    return;
  }

  if (captureStorage.printTo(Serial)) {
    statusLed.showReceiveOk();
  } else {
    statusLed.showStorageError();
  }

  Serial.println();
  Serial.println(F("Auslesen beendet. Reset druecken zum erneuten Ausgeben."));
}

void loop() {
  statusLed.update();
  delay(1);
}
