#pragma once

#include <Arduino.h>
#include <FFat.h>
#include <FS.h>

namespace capture_storage {

static constexpr const char* kCaptureLogPath = "/ir_captures.log";

class CaptureStorage {
 public:
  bool begin(const bool formatOnFail = true) {
    mounted_ = FFat.begin(formatOnFail);
    return mounted_;
  }

  bool append(const uint32_t timestampMs, const String& humanReadable,
              const String& sourceCode) {
    if (!mounted_) {
      return false;
    }

    File file = FFat.open(kCaptureLogPath, FILE_APPEND);
    if (!file) {
      return false;
    }

    file.println(F("----- IR Capture -----"));
    file.print(F("Zeit ms: "));
    file.println(timestampMs);
    file.print(humanReadable);
    file.println(sourceCode);
    file.println(F("----------------------"));
    file.println();
    return true;
  }

  bool printTo(Stream& output) {
    if (!mounted_) {
      output.println(F("FFat nicht bereit."));
      return false;
    }

    if (!FFat.exists(kCaptureLogPath)) {
      output.println(F("Noch keine Captures gespeichert."));
      return true;
    }

    File file = FFat.open(kCaptureLogPath, FILE_READ);
    if (!file) {
      output.println(F("Capture-Log konnte nicht geoeffnet werden."));
      return false;
    }

    while (file.available()) {
      output.write(file.read());
    }

    return true;
  }

 private:
  bool mounted_ = false;
};

}  // namespace capture_storage
