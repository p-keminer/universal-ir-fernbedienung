#pragma once

#include <Arduino.h>

namespace status_led {

struct RgbColor {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

static constexpr uint8_t kRgbLedPin = 48;

static constexpr RgbColor kOffColor = {0, 0, 0};
static constexpr RgbColor kBootColor = {0, 0, 48};
static constexpr RgbColor kSendColor = {96, 32, 0};
static constexpr RgbColor kSendOkColor = {0, 64, 0};
static constexpr RgbColor kReceiveOkColor = {0, 48, 64};
static constexpr RgbColor kStorageErrorColor = {96, 0, 0};

static constexpr uint16_t kBootPulseMs = 220;
static constexpr uint16_t kSendPulseMs = 120;
static constexpr uint16_t kSendOkPulseMs = 180;
static constexpr uint16_t kReceiveOkPulseMs = 260;
static constexpr uint16_t kErrorPulseMs = 700;

class StatusLed {
 public:
  explicit StatusLed(const uint8_t pin) : pin_(pin) {}

  void begin() {
    off();
  }

  void showBoot() {
    show(kBootColor, kBootPulseMs);
  }

  void showSending() {
    show(kSendColor, kSendPulseMs);
  }

  void showSendOk() {
    show(kSendOkColor, kSendOkPulseMs);
  }

  void showReceiveOk() {
    show(kReceiveOkColor, kReceiveOkPulseMs);
  }

  void showStorageError() {
    show(kStorageErrorColor, kErrorPulseMs);
  }

  void update() {
    if (!active_) {
      return;
    }

    if (static_cast<int32_t>(millis() - offAtMs_) >= 0) {
      off();
    }
  }

  void off() {
    write(kOffColor);
    active_ = false;
  }

 private:
  void show(const RgbColor color, const uint16_t durationMs) {
    write(color);
    active_ = durationMs > 0;
    offAtMs_ = millis() + durationMs;
  }

  void write(const RgbColor color) {
    rgbLedWrite(pin_, color.red, color.green, color.blue);
  }

  uint8_t pin_;
  bool active_ = false;
  uint32_t offAtMs_ = 0;
};

}  // namespace status_led
