#include <Arduino.h>

#include "config/Config.h"
#include "ir_sender/IrSender.h"
#include "web_dashboard/WebDashboard.h"

status_led::StatusLed statusLed(config::kRgbLedPin);
ir_sender::UniversalIrSender irSender(&statusLed);
web_dashboard::RemoteWebDashboard dashboard(irSender);

void setup() {
  Serial.begin(config::kSerialBaud);
  delay(config::kBootDelayMs);

  statusLed.begin();
  statusLed.showBoot();
  irSender.begin();
  dashboard.begin();
}

void loop() {
  dashboard.update();
  statusLed.update();
  delay(1);
}
