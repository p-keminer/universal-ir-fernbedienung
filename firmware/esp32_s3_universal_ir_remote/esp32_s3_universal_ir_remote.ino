#include <Arduino.h>

#include "config/Config.h"
#include "ir_receiver/IrReceiver.h"
#include "ir_sender/IrSender.h"
#include "web_dashboard/WebDashboard.h"

status_led::StatusLed statusLed(config::kRgbLedPin);
ir_sender::UniversalIrSender irSender(&statusLed);
ir_receiver::IntegratedIrReceiver captureReceiver(&statusLed);
web_dashboard::RemoteWebDashboard dashboard(irSender, captureReceiver);

void setup() {
  Serial.begin(config::kSerialBaud);
  delay(config::kBootDelayMs);

  statusLed.begin();
  statusLed.showBoot();
  irSender.begin();
  captureReceiver.begin();
  dashboard.begin();
}

void loop() {
  dashboard.update();
  captureReceiver.update();
  statusLed.update();
  delay(1);
}
