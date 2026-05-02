#pragma once

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "../config/Config.h"
#include "../ir_catalog/IrCatalog.h"

namespace ir_sender {

class UniversalIrSender {
 public:
  UniversalIrSender() : irsend_(config::kIrLedPin) {}

  void begin() {
    irsend_.begin();
    Serial.print(F("IR-Sender bereit auf GPIO"));
    Serial.println(config::kIrLedPin);
  }

  bool send(const ir_catalog::IrCommand& command) {
    Serial.print(F("IR send: "));
    Serial.print(command.name);
    Serial.print(F(" / "));
    Serial.print(ir_catalog::protocolName(command.protocol));
    Serial.print(F(" data=0x"));
    Serial.println(static_cast<unsigned long>(command.data), HEX);

    switch (command.protocol) {
      case ir_catalog::IrProtocol::Samsung:
        irsend_.sendSAMSUNG(command.data, command.bits);
        return true;
      case ir_catalog::IrProtocol::Nec:
        irsend_.sendNEC(command.data, command.bits);
        return true;
      case ir_catalog::IrProtocol::Sony:
        irsend_.sendSony(command.data, command.bits);
        return true;
      case ir_catalog::IrProtocol::Panasonic:
        irsend_.sendPanasonic(command.address,
                              static_cast<uint32_t>(command.data));
        return true;
      case ir_catalog::IrProtocol::Rc5:
        irsend_.sendRC5(command.data, command.bits);
        return true;
      case ir_catalog::IrProtocol::Jvc:
        irsend_.sendJVC(command.data, command.bits);
        return true;
      case ir_catalog::IrProtocol::SharpRaw:
        irsend_.sendSharpRaw(command.data, command.bits);
        return true;
      case ir_catalog::IrProtocol::Epson:
        irsend_.sendEpson(command.data, command.bits);
        return true;
      default:
        return false;
    }
  }

  bool sendDiagnosticSweep() {
    bool sentAny = false;

    Serial.println(F("Starte manuelle Diagnostic-Sweep-Sequenz."));
    for (uint8_t i = 0; i < ir_catalog::commandCount(); ++i) {
      const ir_catalog::IrCommand& command = ir_catalog::kCommands[i];

      if (!command.diagnostic) {
        continue;
      }

      sentAny = send(command) || sentAny;
      delay(config::kCommandGapMs);
    }

    Serial.println(F("Diagnostic-Sweep beendet."));
    return sentAny;
  }

 private:
  IRsend irsend_;
};

}  // namespace ir_sender
