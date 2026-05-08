#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include "../shared/CaptureStorage.h"
#include "../shared/StatusLed.h"

constexpr uint16_t kRecvPin = 15;
constexpr uint32_t kBaudRate = 115200;
constexpr uint16_t kCaptureBufferSize = 1024;
constexpr uint8_t kCaptureTimeoutMs = 15;
constexpr uint16_t kMinUnknownSize = 12;

status_led::StatusLed statusLed(status_led::kRgbLedPin);
capture_storage::CaptureStorage captureStorage;
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kCaptureTimeoutMs, true);
decode_results results;

void setup() {
  Serial.begin(kBaudRate);
  delay(500);

  statusLed.begin();
  statusLed.showBoot();

  Serial.println();
  Serial.println(F("IR-Code-Capture bereit"));
  Serial.print(F("Empfaenger: GPIO"));
  Serial.println(kRecvPin);
  Serial.println(F("Taste auf Originalfernbedienung kurz druecken."));

  Serial.print(F("Interne RGB-LED: GPIO"));
  Serial.println(status_led::kRgbLedPin);
  Serial.print(F("Capture-Log: "));
  Serial.println(capture_storage::kCaptureLogPath);

  if (captureStorage.begin()) {
    Serial.println(F("FFat Capture-Speicher bereit."));
  } else {
    Serial.println(F("WARNUNG: FFat Capture-Speicher nicht bereit."));
    statusLed.showStorageError();
  }

#if DECODE_HASH
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif

  irrecv.enableIRIn();
}

void loop() {
  statusLed.update();

  if (!irrecv.decode(&results)) {
    return;
  }

  const uint32_t now = millis();
  const String humanReadable = resultToHumanReadableBasic(&results);
  const String sourceCode = resultToSourceCode(&results);

  Serial.println(F("----- IR Capture -----"));
  Serial.print(F("Zeit ms: "));
  Serial.println(now);

  if (results.overflow) {
    Serial.print(F("WARNUNG: Capture-Puffer voll: "));
    Serial.println(kCaptureBufferSize);
  }

  Serial.print(humanReadable);
  Serial.println(sourceCode);

  if (captureStorage.append(now, humanReadable, sourceCode)) {
    Serial.print(F("Capture gespeichert: "));
    Serial.println(capture_storage::kCaptureLogPath);
    statusLed.showReceiveOk();
  } else {
    Serial.println(F("WARNUNG: Capture konnte nicht gespeichert werden."));
    statusLed.showStorageError();
  }

  Serial.println(F("----------------------"));
  Serial.println();

  irrecv.resume();
  yield();
}
