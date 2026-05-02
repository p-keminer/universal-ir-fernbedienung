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
    html.reserve(42000);

    html += F("<!doctype html><html lang='de'><head><meta charset='utf-8'>");
    html += F("<meta name='viewport' content='width=device-width,initial-scale=1'>");
    html += F("<title>IR Remote</title><style>");
    html += F(":root{color-scheme:light dark;font-family:system-ui,-apple-system,Segoe UI,sans-serif;background:#f4f7f8;color:#142023}");
    html += F("*{box-sizing:border-box}body{margin:0;padding:16px;background:#f4f7f8;color:#142023}");
    html += F(".shell{max-width:1180px;margin:0 auto}.top{display:grid;grid-template-columns:1fr auto;gap:12px;align-items:end;margin-bottom:12px}");
    html += F("h1{font-size:28px;line-height:1.05;margin:0}.sub{margin:6px 0 0;color:#4e6066;font-size:14px}");
    html += F(".status{border:1px solid #b7c8cd;background:#fff;padding:10px 12px;border-radius:8px;min-width:240px}");
    html += F(".status strong,.stat strong{display:block;font-size:12px;text-transform:uppercase;letter-spacing:.04em;color:#5b6e75}.status p{margin:4px 0 0}");
    html += F(".stats{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:8px;margin:0 0 12px}.stat{background:#fff;border:1px solid #d6e1e4;border-radius:8px;padding:10px 12px}.stat span{display:block;font-size:22px;font-weight:750;margin-top:2px}");
    html += F(".controls{position:sticky;top:0;z-index:2;background:#f4f7f8;padding:8px 0 12px;border-bottom:1px solid #d5e0e4;margin-bottom:12px}");
    html += F(".search-row{display:grid;grid-template-columns:1fr auto;gap:8px}.search{width:100%;min-height:44px;border:1px solid #aebdc2;border-radius:8px;background:#fff;color:#142023;padding:0 12px;font:inherit}");
    html += F(".filters{display:flex;gap:8px;overflow:auto;padding-top:8px}.filter{width:auto;min-height:36px;white-space:nowrap;padding:0 12px}.filter.active{background:#1f6f78;color:#fff;border-color:#1f6f78}");
    html += F(".grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(270px,1fr));gap:12px}.profile-card{background:#fff;border:1px solid #d7e0e3;border-radius:8px;padding:12px}");
    html += F(".card-head{display:flex;align-items:flex-start;justify-content:space-between;gap:8px;margin-bottom:10px}h2{font-size:17px;margin:0}.meta{font-size:12px;margin:3px 0 0;color:#607177}.pill{border:1px solid #bdd0d5;border-radius:999px;padding:3px 8px;font-size:12px;color:#315158;background:#edf4f5}");
    html += F(".commands{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:8px}form{margin:0}.command-form[hidden],.profile-card[hidden],.empty[hidden]{display:none}");
    html += F("button{width:100%;min-height:42px;border:1px solid #aebdc2;border-radius:8px;background:#eef3f4;color:#142023;font:inherit;font-weight:650;cursor:pointer;text-align:center}button:hover{background:#dfe8ea}.tag{display:block;font-size:11px;font-weight:500;color:#5d6c72;margin-top:2px}");
    html += F(".empty{border:1px dashed #9fb2b8;border-radius:8px;padding:20px;text-align:center;color:#56686e;background:#fff}.sweep{margin-top:16px;max-width:360px}.sweep button{background:#fff3df;border-color:#d3a657}");
    html += F("@media (max-width:640px){body{padding:10px}.top{grid-template-columns:1fr}.stats{grid-template-columns:1fr}.commands{grid-template-columns:1fr}.status{min-width:0}.controls{top:0}}");
    html += F("@media (prefers-color-scheme:dark){:root,body{background:#101415;color:#eef4f5}.controls{background:#101415;border-color:#263237}.status,.stat,.profile-card,.empty{background:#182023;border-color:#334247}.search{background:#182023;color:#eef4f5;border-color:#46575d}.sub,.meta,.tag{color:#a9b8bc}button{background:#243034;color:#eef4f5;border-color:#46575d}button:hover{background:#314146}.pill{background:#26373b;color:#d7e8eb;border-color:#46575d}.sweep button{background:#3a2d18;border-color:#80652f}}");
    html += F("</style></head><body><div class='shell'><header class='top'><div><h1>IR Remote</h1><p class='sub'>");
    html += config::kWifiApSsid;
    html += F(" / http://");
    html += config::kDashboardIp;
    html += F("</p></div><div class='status'><strong>Status</strong><p>");
    appendEscaped(html, lastStatus_.c_str());
    html += F("</p></div></header><section class='stats'><div class='stat'><strong>Profile</strong><span>");
    html += String(ir_catalog::profileCount());
    html += F("</span></div><div class='stat'><strong>Befehle</strong><span>");
    html += String(ir_catalog::commandCount());
    html += F("</span></div><div class='stat'><strong>Status</strong><span>Test</span></div></section>");
    html += F("<section class='controls'><div class='search-row'><input id='search' class='search' type='search' placeholder='Profil oder Befehl suchen' oninput='applyFilters()'><button type='button' onclick='clearSearch()'>Reset</button></div><div class='filters'>");
    appendCategoryFilter(html, "all", F("Alle"), true);
    appendCategoryFilter(html, "tv", F("TV"), false);
    appendCategoryFilter(html, "media", F("Media"), false);
    appendCategoryFilter(html, "led", F("LED Strip"), false);
    appendCategoryFilter(html, "projector", F("Projektor"), false);
    appendCategoryFilter(html, "audio", F("Audio"), false);
    appendCategoryFilter(html, "test", F("Test"), false);
    html += F("</div></section><p id='emptyState' class='empty' hidden>Keine passenden Befehle gefunden.</p><main class='grid'>");

    for (uint8_t profileIndex = 0; profileIndex < ir_catalog::profileCount();
         ++profileIndex) {
      appendProfile(html, profileIndex);
    }

    html += F("</main><form class='sweep' method='post' action='/sweep'>");
    html += F("<button type='submit' onclick=\"return confirm('Diagnostic Sweep sendet mehrere Codes. Fortfahren?')\">Diagnostic Sweep</button></form>");
    appendDashboardScript(html);
    html += F("</div></body></html>");
    return html;
  }

  void appendProfile(String& html, const uint8_t profileIndex) const {
    const ir_catalog::IrProfile& profile = ir_catalog::profileAt(profileIndex);
    const uint8_t commandCount =
        ir_catalog::commandCountForProfile(profile.id);

    html += F("<section class='profile-card' data-profile='");
    appendEscaped(html, profile.name);
    html += F("' data-category='");
    html += categoryKey(profile.category);
    html += F("' data-search='");
    appendEscaped(html, profile.name);
    html += F(" ");
    html += categoryKey(profile.category);
    for (uint8_t commandIndex = 0; commandIndex < commandCount;
         ++commandIndex) {
      const ir_catalog::IrCommand* command =
          ir_catalog::commandAt(profile.id, commandIndex);
      if (command != nullptr) {
        html += F(" ");
        appendEscaped(html, command->name);
      }
    }
    html += F("'><div class='card-head'><div><h2>");
    appendEscaped(html, profile.name);
    html += F("</h2><p class='meta'>");
    html += categoryName(profile.category);
    html += F(" / ");
    html += String(commandCount);
    html += (commandCount == 1 ? F(" Befehl") : F(" Befehle"));
    html += F("</p></div><span class='pill'>");
    html += categoryName(profile.category);
    html += F("</span></div><div class='commands'>");

    for (uint8_t commandIndex = 0; commandIndex < commandCount;
         ++commandIndex) {
      const ir_catalog::IrCommand* command =
          ir_catalog::commandAt(profile.id, commandIndex);
      if (command == nullptr) {
        continue;
      }

      html += F("<form class='command-form' method='post' action='/send' data-command='");
      appendEscaped(html, command->name);
      html += F(" ");
      html += statusName(command->status);
      html += F(" ");
      html += ir_catalog::protocolName(command->protocol);
      html += F("'>");
      html += F("<input type='hidden' name='profile' value='");
      html += String(profileIndex);
      html += F("'><input type='hidden' name='command' value='");
      html += String(commandIndex);
      html += F("'><button class='command-button' type='submit'>");
      appendEscaped(html, command->name);
      html += F("<span class='tag'>");
      html += statusName(command->status);
      html += F(" / ");
      html += ir_catalog::protocolName(command->protocol);
      html += F("</span></button></form>");
    }

    html += F("</div></section>");
  }

  void appendCategoryFilter(String& html, const char* key,
                            const __FlashStringHelper* label,
                            const bool active) const {
    html += F("<button class='filter");
    html += active ? F(" active") : F("");
    html += F("' type='button' data-filter='");
    html += key;
    html += F("' onclick=\"filterCategory('");
    html += key;
    html += F("',this)\">");
    html += label;
    html += F("</button>");
  }

  void appendDashboardScript(String& html) const {
    html += F("<script>");
    html += F("let activeCategory='all';");
    html += F("function normalize(v){return (v||'').toLowerCase();}");
    html += F("function filterCategory(category,button){activeCategory=category;document.querySelectorAll('.filter').forEach(function(item){item.classList.remove('active')});button.classList.add('active');applyFilters();}");
    html += F("function clearSearch(){document.getElementById('search').value='';activeCategory='all';document.querySelectorAll('.filter').forEach(function(item){item.classList.toggle('active',item.dataset.filter==='all')});applyFilters();}");
    html += F("function applyFilters(){const query=normalize(document.getElementById('search').value);let visibleProfiles=0;document.querySelectorAll('.profile-card').forEach(function(card){const categoryOk=activeCategory==='all'||card.dataset.category===activeCategory;let visibleCommands=0;card.querySelectorAll('.command-form').forEach(function(form){const text=normalize(card.dataset.search+' '+form.dataset.command);const show=categoryOk&&(!query||text.indexOf(query)!==-1);form.hidden=!show;if(show){visibleCommands++;}});const profileMatch=!query||normalize(card.dataset.search).indexOf(query)!==-1;const showCard=categoryOk&&(visibleCommands>0||profileMatch);card.hidden=!showCard;if(showCard){visibleProfiles++;}});document.getElementById('emptyState').hidden=visibleProfiles!==0;}");
    html += F("document.addEventListener('DOMContentLoaded',applyFilters);");
    html += F("</script>");
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
        case '\'':
          html += F("&#39;");
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

  static const char* categoryKey(const ir_catalog::DeviceCategory category) {
    switch (category) {
      case ir_catalog::DeviceCategory::Tv:
        return "tv";
      case ir_catalog::DeviceCategory::Media:
        return "media";
      case ir_catalog::DeviceCategory::LedStrip:
        return "led";
      case ir_catalog::DeviceCategory::Projector:
        return "projector";
      case ir_catalog::DeviceCategory::Audio:
        return "audio";
      case ir_catalog::DeviceCategory::GenericTest:
      default:
        return "test";
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
