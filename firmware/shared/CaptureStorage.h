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

  bool append(const uint32_t timestampMs, const String& label,
              const String& humanReadable, const String& sourceCode) {
    if (!mounted_) {
      return false;
    }

    File file = FFat.open(kCaptureLogPath, FILE_APPEND);
    if (!file) {
      return false;
    }

    file.println(F("----- IR Capture -----"));
    file.print(F("Titel: "));
    file.println(label);
    file.print(F("Zeit ms: "));
    file.println(timestampMs);
    file.print(humanReadable);
    file.println(sourceCode);
    file.println(F("----------------------"));
    file.println();
    return true;
  }

  bool append(const uint32_t timestampMs, const String& humanReadable,
              const String& sourceCode) {
    const String fallbackLabel = F("Ohne Titel");
    return append(timestampMs, fallbackLabel, humanReadable, sourceCode);
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

  template <typename ServerT>
  bool appendDownloadTo(ServerT& server) {
    server.sendHeader("Content-Disposition",
                      "attachment; filename=\"ir_captures.log\"");

    if (!mounted_) {
      server.send(500, "text/plain", "FFat nicht bereit.\n");
      return false;
    }

    if (!FFat.exists(kCaptureLogPath)) {
      server.send(200, "text/plain", "Noch keine Captures gespeichert.\n");
      return true;
    }

    File file = FFat.open(kCaptureLogPath, FILE_READ);
    if (!file) {
      server.send(500, "text/plain",
                  "Capture-Log konnte nicht geoeffnet werden.\n");
      return false;
    }

    server.streamFile(file, "text/plain");
    return true;
  }

  bool clear() {
    if (!mounted_) {
      return false;
    }

    if (!FFat.exists(kCaptureLogPath)) {
      return true;
    }

    return FFat.remove(kCaptureLogPath);
  }

 private:
  bool mounted_ = false;
};

}  // namespace capture_storage
