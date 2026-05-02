#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

#include "../config/Config.h"
#include "../ir_catalog/IrCatalog.h"
#include "../ir_sender/IrSender.h"

namespace web_dashboard {

class RemoteWebDashboard {
 public:
  explicit RemoteWebDashboard(ir_sender::UniversalIrSender& sender)
      : sender_(sender), server_(config::kWebServerPort) {}

  void begin() {
    WiFi.mode(WIFI_AP);

    const IPAddress localIp(192, 168, 4, 1);
    const IPAddress gateway(192, 168, 4, 1);
    const IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(localIp, gateway, subnet);

    const bool apStarted = WiFi.softAP(config::kWifiApSsid,
                                       config::kWifiApPassword,
                                       config::kWifiApChannel,
                                       false,
                                       config::kWifiApMaxConnections);

    Serial.print(F("WLAN AP: "));
    Serial.println(apStarted ? F("bereit") : F("Fehler"));
    Serial.print(F("SSID: "));
    Serial.println(config::kWifiApSsid);
    Serial.print(F("Dashboard: http://"));
    Serial.println(config::kDashboardIp);

    server_.on("/", HTTP_GET, [this]() { handleRoot(); });
    server_.on("/send", HTTP_ANY, [this]() { handleSend(); });
    server_.on("/sweep", HTTP_ANY, [this]() { handleSweep(); });
    server_.onNotFound([this]() { redirectHome(); });
    server_.begin();

    lastStatus_ = F("Bereit");
  }

  void update() {
    server_.handleClient();
  }

 private:
  void handleRoot() {
    server_.send(200, "text/html", renderDashboard());
  }

  void handleSend() {
    uint8_t profileIndex = 0;
    uint8_t commandIndex = 0;

    if (!parseUint8Arg("profile", profileIndex) ||
        !parseUint8Arg("command", commandIndex) ||
        profileIndex >= ir_catalog::profileCount()) {
      lastStatus_ = F("Ungueltige Auswahl");
      redirectHome();
      return;
    }

    const ir_catalog::IrProfile& profile =
        ir_catalog::profileAt(profileIndex);
    const ir_catalog::IrCommand* command =
        ir_catalog::commandAt(profile.id, commandIndex);

    if (command == nullptr) {
      lastStatus_ = F("Befehl nicht gefunden");
      redirectHome();
      return;
    }

    lastStatus_ = profile.name;
    lastStatus_ += F(" / ");
    lastStatus_ += command->name;
    lastStatus_ += sender_.send(*command) ? F(" gesendet") : F(" Fehler");
    redirectHome();
  }

  void handleSweep() {
    lastStatus_ = sender_.sendDiagnosticSweep()
                      ? F("Diagnostic Sweep gesendet")
                      : F("Keine Diagnostic Codes");
    redirectHome();
  }

  bool parseUint8Arg(const char* name, uint8_t& value) {
    if (!server_.hasArg(name)) {
      return false;
    }

    const String raw = server_.arg(name);
    if (raw.length() == 0 || raw.length() > 3) {
      return false;
    }

    uint16_t parsed = 0;
    for (uint8_t i = 0; i < raw.length(); ++i) {
      const char c = raw.charAt(i);
      if (c < '0' || c > '9') {
        return false;
      }

      parsed = static_cast<uint16_t>((parsed * 10) + (c - '0'));
      if (parsed > 255) {
        return false;
      }
    }

    value = static_cast<uint8_t>(parsed);
    return true;
  }

  String renderDashboard() const {
    String html;
    html.reserve(26000);

    html += F("<!doctype html><html lang='de'><head><meta charset='utf-8'>");
    html += F("<meta name='viewport' content='width=device-width,initial-scale=1'>");
    html += F("<title>IR Remote</title><style>");
    html += F(":root{color-scheme:light dark;font-family:system-ui,-apple-system,Segoe UI,sans-serif;background:#f5f7f8;color:#162023}");
    html += F("body{margin:0;padding:18px;background:#f5f7f8;color:#162023}");
    html += F("header{display:flex;gap:12px;align-items:flex-end;justify-content:space-between;flex-wrap:wrap;margin:0 0 16px}");
    html += F("h1{font-size:26px;line-height:1.1;margin:0}p{margin:4px 0;color:#4f5e63}");
    html += F(".status{border:1px solid #b8c6ca;background:#fff;padding:10px 12px;border-radius:8px;min-width:220px}");
    html += F(".grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(250px,1fr));gap:12px}");
    html += F("section{background:#fff;border:1px solid #d7e0e3;border-radius:8px;padding:12px}");
    html += F("h2{font-size:17px;margin:0 0 2px}.meta{font-size:12px;margin:0 0 10px;color:#607177}");
    html += F(".commands{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:8px}");
    html += F("form{margin:0}button{width:100%;min-height:42px;border:1px solid #aebdc2;border-radius:8px;background:#eef3f4;color:#162023;font:inherit;font-weight:650;cursor:pointer}");
    html += F("button:hover{background:#dfe8ea}.tag{display:block;font-size:11px;font-weight:500;color:#5d6c72;margin-top:2px}");
    html += F(".sweep{margin-top:16px;max-width:360px}.sweep button{background:#fff3df;border-color:#d3a657}");
    html += F("@media (prefers-color-scheme:dark){:root,body{background:#101415;color:#eef4f5}.status,section{background:#182023;border-color:#334247}p,.meta,.tag{color:#a9b8bc}button{background:#243034;color:#eef4f5;border-color:#46575d}button:hover{background:#314146}.sweep button{background:#3a2d18;border-color:#80652f}}");
    html += F("</style></head><body><header><div><h1>IR Remote</h1><p>");
    html += config::kWifiApSsid;
    html += F(" / http://");
    html += config::kDashboardIp;
    html += F("</p></div><div class='status'><strong>Status</strong><p>");
    appendEscaped(html, lastStatus_.c_str());
    html += F("</p></div></header><main class='grid'>");

    for (uint8_t profileIndex = 0; profileIndex < ir_catalog::profileCount();
         ++profileIndex) {
      appendProfile(html, profileIndex);
    }

    html += F("</main><form class='sweep' method='post' action='/sweep'>");
    html += F("<button type='submit'>Diagnostic Sweep</button></form>");
    html += F("</body></html>");
    return html;
  }

  void appendProfile(String& html, const uint8_t profileIndex) const {
    const ir_catalog::IrProfile& profile = ir_catalog::profileAt(profileIndex);
    const uint8_t commandCount =
        ir_catalog::commandCountForProfile(profile.id);

    html += F("<section><h2>");
    appendEscaped(html, profile.name);
    html += F("</h2><p class='meta'>");
    html += categoryName(profile.category);
    html += F(" / ");
    html += String(commandCount);
    html += commandCount == 1 ? F(" Befehl") : F(" Befehle");
    html += F("</p><div class='commands'>");

    for (uint8_t commandIndex = 0; commandIndex < commandCount;
         ++commandIndex) {
      const ir_catalog::IrCommand* command =
          ir_catalog::commandAt(profile.id, commandIndex);
      if (command == nullptr) {
        continue;
      }

      html += F("<form method='post' action='/send'>");
      html += F("<input type='hidden' name='profile' value='");
      html += String(profileIndex);
      html += F("'><input type='hidden' name='command' value='");
      html += String(commandIndex);
      html += F("'><button type='submit'>");
      appendEscaped(html, command->name);
      html += F("<span class='tag'>");
      html += statusName(command->status);
      html += F(" / ");
      html += ir_catalog::protocolName(command->protocol);
      html += F("</span></button></form>");
    }

    html += F("</div></section>");
  }

  void redirectHome() {
    server_.sendHeader("Location", "/");
    server_.send(303, "text/plain", "");
  }

  static void appendEscaped(String& html, const char* text) {
    for (const char* c = text; *c != '\0'; ++c) {
      switch (*c) {
        case '&':
          html += F("&amp;");
          break;
        case '<':
          html += F("&lt;");
          break;
        case '>':
          html += F("&gt;");
          break;
        case '"':
          html += F("&quot;");
          break;
        default:
          html += *c;
          break;
      }
    }
  }

  static const __FlashStringHelper* categoryName(
      const ir_catalog::DeviceCategory category) {
    switch (category) {
      case ir_catalog::DeviceCategory::Tv:
        return F("TV");
      case ir_catalog::DeviceCategory::Media:
        return F("Media");
      case ir_catalog::DeviceCategory::LedStrip:
        return F("LED Strip");
      case ir_catalog::DeviceCategory::Projector:
        return F("Projektor");
      case ir_catalog::DeviceCategory::Audio:
        return F("Audio");
      case ir_catalog::DeviceCategory::GenericTest:
      default:
        return F("Test");
    }
  }

  static const __FlashStringHelper* statusName(
      const ir_catalog::CodeStatus status) {
    switch (status) {
      case ir_catalog::CodeStatus::Confirmed:
        return F("confirmed");
      case ir_catalog::CodeStatus::Rejected:
        return F("rejected");
      case ir_catalog::CodeStatus::Untested:
      default:
        return F("untested");
    }
  }

  ir_sender::UniversalIrSender& sender_;
  WebServer server_;
  String lastStatus_;
};

}  // namespace web_dashboard
