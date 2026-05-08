#pragma once

#include <Arduino.h>

#include "../shared/StatusLed.h"

namespace config {

constexpr uint32_t kSerialBaud = 115200;
constexpr uint16_t kBootDelayMs = 200;

constexpr uint16_t kIrLedPin = 4;
constexpr uint16_t kIrRecvPin = 15;
constexpr uint16_t kRgbLedPin = status_led::kRgbLedPin;
constexpr uint16_t kCommandGapMs = 200;
constexpr uint16_t kPostSendStatusMs = 700;
constexpr uint16_t kCaptureBufferSize = 1024;
constexpr uint8_t kCaptureTimeoutMs = 15;
constexpr uint16_t kMinUnknownSize = 12;
constexpr uint8_t kCaptureLabelMaxLength = 64;

constexpr const char* kWifiApSsid = "IR-Remote";
constexpr const char* kWifiApPassword = "irremote";
constexpr const char* kDashboardIp = "192.168.4.1";
constexpr uint8_t kWifiApChannel = 6;
constexpr uint8_t kWifiApMaxConnections = 4;
constexpr uint16_t kWebServerPort = 80;

}  // namespace config
