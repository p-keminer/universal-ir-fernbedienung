#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

constexpr uint16_t kRecvPin = 15;
constexpr uint32_t kBaudRate = 115200;
constexpr uint16_t kCaptureBufferSize = 1024;
constexpr uint8_t kCaptureTimeoutMs = 15;
constexpr uint16_t kMinUnknownSize = 12;

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kCaptureTimeoutMs, true);
decode_results results;

void setup() {
  Serial.begin(kBaudRate);
  delay(500);

  Serial.println();
  Serial.println(F("IR-Code-Capture bereit"));
  Serial.print(F("Empfaenger: GPIO"));
  Serial.println(kRecvPin);
  Serial.println(F("Taste auf Originalfernbedienung kurz druecken."));

#if DECODE_HASH
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif

  irrecv.enableIRIn();
}

void loop() {
  if (!irrecv.decode(&results)) {
    return;
  }

  const uint32_t now = millis();
  Serial.println(F("----- IR Capture -----"));
  Serial.print(F("Zeit ms: "));
  Serial.println(now);

  if (results.overflow) {
    Serial.print(F("WARNUNG: Capture-Puffer voll: "));
    Serial.println(kCaptureBufferSize);
  }

  Serial.print(resultToHumanReadableBasic(&results));
  Serial.println(resultToSourceCode(&results));
  Serial.println(F("----------------------"));
  Serial.println();

  irrecv.resume();
  yield();
}
