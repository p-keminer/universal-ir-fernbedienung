#pragma once

#include <Arduino.h>
#include <FFat.h>
#include <FS.h>

namespace capture_storage {

static constexpr const char* kCaptureLogPath = "/ir_captures.log";
static constexpr const char* kCaptureTmpPath = "/ir_captures.tmp";

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
    file.print(F("Index-Quelle: "));
    file.println(timestampMs);
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

  template <typename ServerT>
  bool sendListJsonTo(ServerT& server) {
    String payload;
    payload.reserve(8192);
    payload += F("{\"records\":[");

    if (!mounted_) {
      payload += F("],\"storage\":false}");
      server.send(200, "application/json", payload);
      return false;
    }

    if (!FFat.exists(kCaptureLogPath)) {
      payload += F("],\"storage\":true}");
      server.send(200, "application/json", payload);
      return true;
    }

    File file = FFat.open(kCaptureLogPath, FILE_READ);
    if (!file) {
      payload += F("],\"storage\":false}");
      server.send(200, "application/json", payload);
      return false;
    }

    bool first = true;
    bool inRecord = false;
    uint16_t recordIndex = 0;
    String label;
    String timestamp;
    String block;

    while (file.available()) {
      String line = file.readStringUntil('\n');
      trimLineEnd(line);

      if (line.startsWith(F("----- IR Capture -----"))) {
        inRecord = true;
        label = F("Ohne Titel");
        timestamp = F("");
        block = line;
        block += F("\n");
        continue;
      }

      if (!inRecord) {
        continue;
      }

      block += line;
      block += F("\n");

      if (line.startsWith(F("Titel: "))) {
        label = line.substring(7);
      } else if (line.startsWith(F("Zeit ms: "))) {
        timestamp = line.substring(9);
      }

      if (line.startsWith(F("----------------------"))) {
        appendRecordJson(payload, first, recordIndex, label, timestamp, block);
        first = false;
        ++recordIndex;
        inRecord = false;
      }
    }

    if (inRecord) {
      appendRecordJson(payload, first, recordIndex, label, timestamp, block);
    }

    payload += F("],\"storage\":true}");
    server.send(200, "application/json", payload);
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

  bool deleteRecord(const uint16_t targetIndex) {
    if (!mounted_ || !FFat.exists(kCaptureLogPath)) {
      return false;
    }

    File source = FFat.open(kCaptureLogPath, FILE_READ);
    if (!source) {
      return false;
    }

    FFat.remove(kCaptureTmpPath);
    File target = FFat.open(kCaptureTmpPath, FILE_WRITE);
    if (!target) {
      return false;
    }

    bool inRecord = false;
    bool skipRecord = false;
    bool deleted = false;
    uint16_t recordIndex = 0;

    while (source.available()) {
      String line = source.readStringUntil('\n');
      trimLineEnd(line);

      if (line.startsWith(F("----- IR Capture -----"))) {
        inRecord = true;
        skipRecord = recordIndex == targetIndex;
        deleted = deleted || skipRecord;
      }

      if (!skipRecord) {
        target.println(line);
      }

      if (inRecord && line.startsWith(F("----------------------"))) {
        inRecord = false;
        skipRecord = false;
        ++recordIndex;
      }
    }

    source.close();
    target.close();

    if (!deleted) {
      FFat.remove(kCaptureTmpPath);
      return false;
    }

    FFat.remove(kCaptureLogPath);
    return FFat.rename(kCaptureTmpPath, kCaptureLogPath);
  }

 private:
  static void trimLineEnd(String& line) {
    while (line.endsWith("\r") || line.endsWith("\n")) {
      line.remove(line.length() - 1);
    }
  }

  static void appendJsonEscaped(String& json, const String& text) {
    for (uint16_t i = 0; i < text.length(); ++i) {
      const char c = text.charAt(i);
      switch (c) {
        case '\\':
          json += F("\\\\");
          break;
        case '"':
          json += F("\\\"");
          break;
        case '\n':
          json += F("\\n");
          break;
        case '\r':
          break;
        case '\t':
          json += F("\\t");
          break;
        default:
          json += c;
          break;
      }
    }
  }

  static void appendRecordJson(String& payload, const bool first,
                               const uint16_t index, const String& label,
                               const String& timestamp, const String& block) {
    if (!first) {
      payload += F(",");
    }

    payload += F("{\"index\":");
    payload += String(index);
    payload += F(",\"label\":\"");
    appendJsonEscaped(payload, label);
    payload += F("\",\"time\":\"");
    appendJsonEscaped(payload, timestamp);
    payload += F("\",\"text\":\"");
    appendJsonEscaped(payload, block);
    payload += F("\"}");
  }

  bool mounted_ = false;
};

}  // namespace capture_storage
