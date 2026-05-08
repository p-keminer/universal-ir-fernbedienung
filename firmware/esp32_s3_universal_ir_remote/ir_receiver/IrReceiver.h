#pragma once

#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include "../config/Config.h"
#include "../shared/CaptureStorage.h"
#include "../shared/StatusLed.h"

namespace ir_receiver {

enum class OperationMode : uint8_t {
  Send,
  Capture,
};

class IntegratedIrReceiver {
 public:
  explicit IntegratedIrReceiver(status_led::StatusLed* statusLed = nullptr)
      : irrecv_(config::kIrRecvPin,
                config::kCaptureBufferSize,
                config::kCaptureTimeoutMs,
                true),
        statusLed_(statusLed) {}

  bool begin() {
    storageReady_ = captureStorage_.begin(true);

#if DECODE_HASH
    irrecv_.setUnknownThreshold(config::kMinUnknownSize);
#endif

    irrecv_.disableIRIn();

    Serial.print(F("IR-Empfaenger bereit auf GPIO"));
    Serial.println(config::kIrRecvPin);
    Serial.print(F("Capture-Log: "));
    Serial.println(capture_storage::kCaptureLogPath);
    Serial.println(storageReady_ ? F("FFat Capture-Speicher bereit.")
                                 : F("WARNUNG: FFat nicht bereit."));

    return storageReady_;
  }

  void update() {
    if (mode_ != OperationMode::Capture) {
      return;
    }

    if (!irrecv_.decode(&results_)) {
      return;
    }

    const uint32_t now = millis();
    const String label = captureLabel_;
    const String humanReadable = resultToHumanReadableBasic(&results_);
    const String sourceCode = resultToSourceCode(&results_);
    const bool stored =
        captureStorage_.append(now, label, humanReadable, sourceCode);

    printCapture(now, label, humanReadable, sourceCode, stored);
    updateLatestCapture(now, label, humanReadable, sourceCode, stored);

    if (statusLed_ != nullptr) {
      if (stored) {
        statusLed_->showReceiveOk();
      } else {
        statusLed_->showStorageError();
      }
    }

    irrecv_.resume();
    yield();
  }

  void setMode(const OperationMode mode) {
    if (mode_ == mode) {
      return;
    }

    mode_ = mode;

    if (mode_ == OperationMode::Capture) {
      irrecv_.enableIRIn();
      Serial.println(F("Modus: Einlesen"));
    } else {
      irrecv_.disableIRIn();
      Serial.println(F("Modus: Senden"));
    }
  }

  OperationMode mode() const {
    return mode_;
  }

  const char* modeName() const {
    return mode_ == OperationMode::Capture ? "capture" : "send";
  }

  bool isCaptureMode() const {
    return mode_ == OperationMode::Capture;
  }

  void setCaptureLabel(const String& label) {
    captureLabel_ = label;
    captureLabel_.trim();

    if (captureLabel_.length() == 0) {
      captureLabel_ = F("Ohne Titel");
    }

    if (captureLabel_.length() > config::kCaptureLabelMaxLength) {
      captureLabel_.remove(config::kCaptureLabelMaxLength);
    }

    Serial.print(F("Capture-Titel: "));
    Serial.println(captureLabel_);
  }

  const String& captureLabel() const {
    return captureLabel_;
  }

  const String& latestCaptureSummary() const {
    return latestCaptureSummary_;
  }

  bool storageReady() const {
    return storageReady_;
  }

  template <typename ServerT>
  bool appendCaptureDownloadTo(ServerT& server) {
    return captureStorage_.appendDownloadTo(server);
  }

  template <typename ServerT>
  bool appendCaptureListTo(ServerT& server) {
    return captureStorage_.sendListJsonTo(server);
  }

  bool deleteCaptureRecord(const uint16_t index) {
    const bool deleted = captureStorage_.deleteRecord(index);
    if (deleted) {
      latestCaptureSummary_ = F("Capture geloescht.");
    }
    return deleted;
  }

  bool clearCaptures() {
    const bool cleared = captureStorage_.clear();
    if (cleared) {
      latestCaptureSummary_ = F("Capture-Log geloescht.");
    }
    return cleared;
  }

 private:
  void printCapture(const uint32_t now, const String& label,
                    const String& humanReadable, const String& sourceCode,
                    const bool stored) const {
    Serial.println(F("----- IR Capture -----"));
    Serial.print(F("Titel: "));
    Serial.println(label);
    Serial.print(F("Zeit ms: "));
    Serial.println(now);

    if (results_.overflow) {
      Serial.print(F("WARNUNG: Capture-Puffer voll: "));
      Serial.println(config::kCaptureBufferSize);
    }

    Serial.print(humanReadable);
    Serial.println(sourceCode);
    Serial.println(stored ? F("Capture gespeichert.")
                          : F("WARNUNG: Capture nicht gespeichert."));
    Serial.println(F("----------------------"));
    Serial.println();
  }

  void updateLatestCapture(const uint32_t now, const String& label,
                           const String& humanReadable,
                           const String& sourceCode, const bool stored) {
    latestCaptureSummary_ = F("Titel: ");
    latestCaptureSummary_ += label;
    latestCaptureSummary_ += F("\nZeit ms: ");
    latestCaptureSummary_ += String(now);
    latestCaptureSummary_ += F("\n");
    latestCaptureSummary_ += stored ? F("Gespeichert: ja\n")
                                    : F("Gespeichert: nein\n");
    latestCaptureSummary_ += humanReadable;
    latestCaptureSummary_ += sourceCode;
  }

  IRrecv irrecv_;
  decode_results results_;
  capture_storage::CaptureStorage captureStorage_;
  status_led::StatusLed* statusLed_;
  OperationMode mode_ = OperationMode::Send;
  bool storageReady_ = false;
  String captureLabel_ = F("Ohne Titel");
  String latestCaptureSummary_ = F("Noch kein Capture empfangen.");
};

}  // namespace ir_receiver
