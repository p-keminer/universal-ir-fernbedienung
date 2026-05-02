#include <Arduino.h>

#include "config/Config.h"
#include "ir_sender/IrSender.h"
#include "web_dashboard/WebDashboard.h"

ir_sender::UniversalIrSender irSender;
web_dashboard::RemoteWebDashboard dashboard(irSender);

void setup() {
  Serial.begin(config::kSerialBaud);
  delay(config::kBootDelayMs);

  irSender.begin();
  dashboard.begin();
}

void loop() {
  dashboard.update();
  delay(1);
}
