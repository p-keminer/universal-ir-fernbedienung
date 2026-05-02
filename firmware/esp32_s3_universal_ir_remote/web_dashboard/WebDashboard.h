#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <string.h>

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

  enum class CommandGroup : uint8_t {
    Power,
    VolumeChannel,
    Navigation,
    SourceMedia,
    Lighting,
    Other,
  };

  String renderDashboard() const {
    String html;
    html.reserve(64000);

    html += F("<!doctype html><html lang='de'><head><meta charset='utf-8'>");
    html += F("<meta name='viewport' content='width=device-width,initial-scale=1'>");
    html += F("<title>IR Remote</title><style>");
    html += F(":root{color-scheme:light dark;font-family:system-ui,-apple-system,Segoe UI,sans-serif;background:#e8edf0;color:#081116;scrollbar-width:thin;scrollbar-color:#536a75 #d3dde1}");
    html += F("*{box-sizing:border-box}html,body{min-height:100%;overflow-x:hidden}body{margin:0;padding:14px;background:#e8edf0;color:#081116}");
    html += F("::-webkit-scrollbar{width:10px;height:10px}::-webkit-scrollbar-track{background:#d3dde1}::-webkit-scrollbar-thumb{background:#536a75;border:2px solid #d3dde1;border-radius:999px}::-webkit-scrollbar-thumb:hover{background:#30454e}");
    html += F(".shell{max-width:980px;margin:0 auto;padding-bottom:128px}.top{display:grid;grid-template-columns:1fr minmax(190px,280px);gap:12px;align-items:center;margin-bottom:10px}");
    html += F("h1{font-size:25px;line-height:1.05;margin:0}.status{border:2px solid #8a9da5;background:#fff;padding:9px 11px;border-radius:8px;min-width:0}");
    html += F(".status strong{display:block;font-size:12px;text-transform:uppercase;letter-spacing:.04em;color:#32434b}.status p{margin:4px 0 0;min-height:20px}");
    html += F(".controls{position:sticky;top:0;z-index:3;background:#e8edf0;padding:8px 0 12px;border-bottom:2px solid #bac8ce;margin-bottom:14px}");
    html += F(".search-row{display:grid;grid-template-columns:1fr 86px;gap:10px}.search{width:100%;min-height:48px;border:2px solid #8a9da5;border-radius:8px;background:#fff;color:#081116;padding:0 12px;font:inherit}");
    html += F(".filters{display:grid;grid-template-columns:repeat(auto-fit,minmax(112px,1fr));gap:10px;padding:10px 0 0}.filter{width:100%;min-height:44px;white-space:nowrap;padding:0 12px}.filter.active{background:#006d77;color:#fff;border-color:#00545c}");
    html += F(".profile-picker{margin:0 0 20px}.stage-title{font-size:13px;font-weight:800;text-transform:uppercase;letter-spacing:.04em;color:#2f4048;margin:0 0 8px}.brand-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(150px,1fr));gap:10px}");
    html += F(".brand-card{min-height:68px;text-align:left;padding:10px 11px;background:#fff}.brand-card.active{background:#d7f0f2;border-color:#00545c;color:#081116}.brand-card span{display:block;font-size:12px;font-weight:650;color:#33464f;margin-top:3px}");
    html += F(".remote-stage{display:block;padding-bottom:40px}.remote-card{max-width:720px;margin:0 auto 28px;background:#fff;border:2px solid #bac8ce;border-radius:8px;padding:14px;scroll-margin-top:138px}.remote-head{display:flex;justify-content:space-between;gap:10px;align-items:flex-start;margin-bottom:12px}");
    html += F("h2{font-size:20px;margin:0}.meta{font-size:12px;margin:4px 0 0;color:#33464f}.pill{border:2px solid #9eb2ba;border-radius:999px;padding:3px 8px;font-size:12px;color:#193940;background:#e3f4f6;white-space:nowrap}");
    html += F(".remote-group{margin-top:15px}.remote-group-title{font-size:12px;font-weight:800;text-transform:uppercase;letter-spacing:.04em;color:#657078;margin:0 0 8px}.remote-grid{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:10px}");
    html += F("form{margin:0}.command-form[hidden],.brand-card[hidden],.remote-card[hidden],.profile-picker[hidden],.empty[hidden]{display:none}");
    html += F("button{width:100%;min-height:48px;border:2px solid #8799a3;border-radius:8px;background:#f8fbfc;color:#081116;font:inherit;font-weight:800;cursor:pointer;text-align:center}button:hover{background:#dce8ec}");
    html += F(".remote-command button{min-height:60px;padding:8px 6px}.remote-command.power button{background:#fff0e8;border-color:#b84f2c;color:#64210f}.remote-command.nav button{background:#e8f0ff;border-color:#8aa4d6}.remote-command.light button{background:#fff8d9;border-color:#b79c25}");
    html += F(".command-label{display:block}.tag{display:block;font-size:11px;font-weight:650;color:#30444d;margin-top:3px}.empty{border:2px dashed #7f939c;border-radius:8px;padding:18px;text-align:center;color:#33464f;background:#fff}.sweep{margin:22px auto 0;max-width:360px}.sweep button{background:#fff0d4;border-color:#b47811}");
    html += F("@media (max-width:680px){body{padding:10px}.top{grid-template-columns:1fr}.search-row{grid-template-columns:1fr 78px}.filters{grid-template-columns:repeat(2,minmax(0,1fr))}.remote-card{padding:12px;scroll-margin-top:156px}.remote-grid{grid-template-columns:repeat(2,minmax(0,1fr))}.remote-group.nav .remote-grid{grid-template-columns:repeat(3,minmax(0,1fr))}.status{min-width:0}}");
    html += F("@media (prefers-color-scheme:dark){:root{scrollbar-color:#87a5af #182225}:root,body{background:#0c1113;color:#f3f8f9}.controls{background:#0c1113;border-color:#34454c}.status,.brand-card,.remote-card,.empty{background:#172022;border-color:#52656d}.search{background:#172022;color:#f3f8f9;border-color:#61757e}.meta,.tag,.stage-title,.remote-group-title{color:#c4d4d9}button{background:#263438;color:#f3f8f9;border-color:#61757e}button:hover{background:#34464c}.filter.active,.brand-card.active{background:#0f6e78;border-color:#75ccd4;color:#fff}.pill{background:#24393e;color:#def8fb;border-color:#61757e}.remote-command.power button{background:#422418;border-color:#c87556;color:#ffd8ca}.remote-command.nav button{background:#1c2b42}.remote-command.light button{background:#3c3412}.sweep button{background:#3d2c10;border-color:#a27a25}}");
    html += F("</style></head><body><div class='shell'><header class='top'><h1>IR Remote</h1><div class='status'><strong>Status</strong><p id='statusText'>");
    appendEscaped(html, lastStatus_.c_str());
    html += F("</p></div></header><section class='controls'><div class='search-row'><input id='search' class='search' type='search' placeholder='Profil oder Befehl suchen' oninput='applyFilters()'><button type='button' onclick='clearSearch()'>Reset</button></div><div class='filters'>");
    appendCategoryFilter(html, "tv", F("TV"));
    appendCategoryFilter(html, "media", F("Media"));
    appendCategoryFilter(html, "led", F("LED Strip"));
    appendCategoryFilter(html, "projector", F("Projektor"));
    appendCategoryFilter(html, "audio", F("Audio"));
    appendCategoryFilter(html, "test", F("Test"));
    html += F("</div></section><section id='profilePicker' class='profile-picker' hidden><p class='stage-title'>Marke auswaehlen</p><div class='brand-grid'>");

    for (uint8_t profileIndex = 0; profileIndex < ir_catalog::profileCount();
         ++profileIndex) {
      appendProfileOption(html, profileIndex);
    }

    html += F("</div></section><p id='emptyState' class='empty' hidden>Keine passenden Profile gefunden.</p><main class='remote-stage'>");

    for (uint8_t profileIndex = 0; profileIndex < ir_catalog::profileCount();
         ++profileIndex) {
      appendRemoteCard(html, profileIndex);
    }

    html += F("</main><form class='sweep' method='post' action='/sweep'>");
    html += F("<button type='submit' onclick=\"return confirm('Diagnostic Sweep sendet mehrere Codes. Fortfahren?')\">Diagnostic Sweep</button></form>");
    appendDashboardScript(html);
    html += F("</div></body></html>");
    return html;
  }

  void appendProfileOption(String& html, const uint8_t profileIndex) const {
    const ir_catalog::IrProfile& profile = ir_catalog::profileAt(profileIndex);
    const uint8_t commandCount =
        ir_catalog::commandCountForProfile(profile.id);

    html += F("<button class='brand-card' hidden type='button' data-profile='");
    appendEscaped(html, profile.name);
    html += F("' data-profile-index='");
    html += String(profileIndex);
    html += F("' data-category='");
    html += categoryKey(profile.category);
    html += F("' data-search='");
    appendProfileSearchText(html, profile, commandCount);
    html += F("' onclick=\"selectProfile('");
    html += String(profileIndex);
    html += F("',this)\">");
    appendEscaped(html, profile.name);
    html += F("<span>");
    html += categoryName(profile.category);
    html += F(" / ");
    html += String(commandCount);
    html += (commandCount == 1 ? F(" Befehl") : F(" Befehle"));
    html += F("</span></button>");
  }

  void appendRemoteCard(String& html, const uint8_t profileIndex) const {
    const ir_catalog::IrProfile& profile = ir_catalog::profileAt(profileIndex);
    const uint8_t commandCount =
        ir_catalog::commandCountForProfile(profile.id);

    html += F("<section class='remote-card' hidden data-profile='");
    appendEscaped(html, profile.name);
    html += F("' data-profile-index='");
    html += String(profileIndex);
    html += F("' data-category='");
    html += categoryKey(profile.category);
    html += F("' data-search='");
    appendProfileSearchText(html, profile, commandCount);
    html += F("'><div class='remote-head'><div><h2>");
    appendEscaped(html, profile.name);
    html += F("</h2><p class='meta'>");
    html += categoryName(profile.category);
    html += F(" / ");
    html += String(commandCount);
    html += (commandCount == 1 ? F(" Befehl") : F(" Befehle"));
    html += F("</p></div><span class='pill'>");
    html += categoryName(profile.category);
    html += F("</span></div>");

    appendCommandGroup(html, profileIndex, CommandGroup::Power, F("Power"),
                       "power");
    appendCommandGroup(html, profileIndex, CommandGroup::VolumeChannel,
                       F("Lautstaerke / Sender"), "volume");
    appendCommandGroup(html, profileIndex, CommandGroup::Navigation,
                       F("Navigation"), "nav");
    appendCommandGroup(html, profileIndex, CommandGroup::SourceMedia,
                       F("Eingang / Menue / Media"), "media");
    appendCommandGroup(html, profileIndex, CommandGroup::Lighting,
                       F("Licht / Farbe"), "light");
    appendCommandGroup(html, profileIndex, CommandGroup::Other, F("Weitere"),
                       "other");

    html += F("</section>");
  }

  void appendProfileSearchText(String& html,
                               const ir_catalog::IrProfile& profile,
                               const uint8_t commandCount) const {
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
  }

  void appendCommandGroup(String& html, const uint8_t profileIndex,
                          const CommandGroup group,
                          const __FlashStringHelper* label,
                          const char* cssClass) const {
    const ir_catalog::IrProfile& profile = ir_catalog::profileAt(profileIndex);
    const uint8_t commandCount =
        ir_catalog::commandCountForProfile(profile.id);
    uint8_t visibleCommands = 0;

    for (uint8_t commandIndex = 0; commandIndex < commandCount;
         ++commandIndex) {
      const ir_catalog::IrCommand* command =
          ir_catalog::commandAt(profile.id, commandIndex);
      if (command != nullptr && classifyCommand(command->name) == group) {
        ++visibleCommands;
      }
    }

    if (visibleCommands == 0) {
      return;
    }

    html += F("<section class='remote-group ");
    html += cssClass;
    html += F("'><p class='remote-group-title'>");
    html += label;
    html += F("</p><div class='remote-grid'>");

    for (uint8_t commandIndex = 0; commandIndex < commandCount;
         ++commandIndex) {
      const ir_catalog::IrCommand* command =
          ir_catalog::commandAt(profile.id, commandIndex);
      if (command == nullptr || classifyCommand(command->name) != group) {
        continue;
      }

      appendCommandForm(html, profileIndex, commandIndex, profile, *command,
                        cssClass);
    }

    html += F("</div></section>");
  }

  void appendCommandForm(String& html, const uint8_t profileIndex,
                         const uint8_t commandIndex,
                         const ir_catalog::IrProfile& profile,
                         const ir_catalog::IrCommand& command,
                         const char* cssClass) const {
    html += F("<form class='command-form remote-command ");
    html += cssClass;
    html += F("' method='post' action='/send' onsubmit='return sendCommand(this)' data-command='");
    appendEscaped(html, command.name);
    html += F(" ");
    html += statusName(command.status);
    html += F(" ");
    html += ir_catalog::protocolName(command.protocol);
    html += F("' data-command-name='");
    appendEscaped(html, command.name);
    html += F("' data-profile-name='");
    appendEscaped(html, profile.name);
    html += F("'>");
    html += F("<input type='hidden' name='profile' value='");
    html += String(profileIndex);
    html += F("'><input type='hidden' name='command' value='");
    html += String(commandIndex);
    html += F("'><button class='command-button' type='submit'><span class='command-label'>");
    appendEscaped(html, command.name);
    html += F("</span><span class='tag'>");
    html += statusName(command.status);
    html += F(" / ");
    html += ir_catalog::protocolName(command.protocol);
    html += F("</span></button></form>");
  }

  void appendCategoryFilter(String& html, const char* key,
                            const __FlashStringHelper* label) const {
    html += F("<button class='filter' type='button' data-filter='");
    html += key;
    html += F("' onclick=\"filterCategory('");
    html += key;
    html += F("',this)\">");
    html += label;
    html += F("</button>");
  }

  void appendDashboardScript(String& html) const {
    html += F("<script>");
    html += F("let activeCategory='';let activeProfile='';");
    html += F("function normalize(v){return (v||'').toLowerCase();}");
    html += F("function setStatus(text){const status=document.getElementById('statusText');if(status){status.textContent=text;}}");
    html += F("function filterCategory(category,button){activeCategory=category;activeProfile='';document.querySelectorAll('.filter').forEach(function(item){item.classList.remove('active')});button.classList.add('active');document.querySelectorAll('.brand-card').forEach(function(item){item.classList.remove('active')});applyFilters();}");
    html += F("function selectProfile(profileIndex,button){activeProfile=String(profileIndex);document.querySelectorAll('.brand-card').forEach(function(item){item.classList.remove('active')});button.classList.add('active');applyFilters();const card=document.querySelector(\".remote-card[data-profile-index='\"+activeProfile+\"']\");if(card){card.scrollIntoView({block:'start',behavior:'smooth'});}}");
    html += F("function clearSearch(){document.getElementById('search').value='';activeCategory='';activeProfile='';document.querySelectorAll('.filter,.brand-card').forEach(function(item){item.classList.remove('active')});applyFilters();}");
    html += F("function applyFilters(){const query=normalize(document.getElementById('search').value);const hasCategory=activeCategory!=='';const picker=document.getElementById('profilePicker');let visibleProfiles=0;document.querySelectorAll('.brand-card').forEach(function(card){const categoryOk=hasCategory&&card.dataset.category===activeCategory;const text=normalize(card.dataset.search);const showCard=categoryOk&&(!query||text.indexOf(query)!==-1);card.hidden=!showCard;if(!showCard){card.classList.remove('active')}if(showCard){visibleProfiles++;}});picker.hidden=!hasCategory;if(activeProfile!==''){const activeButton=document.querySelector(\".brand-card[data-profile-index='\"+activeProfile+\"']\");if(!activeButton||activeButton.hidden||activeButton.dataset.category!==activeCategory){activeProfile='';}}document.querySelectorAll('.remote-card').forEach(function(card){card.hidden=!(hasCategory&&activeProfile!==''&&card.dataset.profileIndex===activeProfile);});document.getElementById('emptyState').hidden=!hasCategory||visibleProfiles!==0;}");
    html += F("function confirmCommand(form){const name=normalize(form.dataset.commandName);if(name.indexOf('power')!==-1||name==='off'||name.indexOf(' off')!==-1){return confirm('Power/Off wirklich senden?')}return true;}");
    html += F("function sendCommand(form){if(!confirmCommand(form)){return false;}const data=new URLSearchParams(new FormData(form));const profile=form.dataset.profileName||'Profil';const command=form.dataset.commandName||'Befehl';setStatus(profile+' / '+command+' sendet...');fetch(form.action,{method:'POST',body:data}).then(function(response){setStatus(response.ok?profile+' / '+command+' gesendet':'Sendefehler');}).catch(function(){setStatus('Sendefehler');});return false;}");
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

  static bool containsText(const char* text, const char* needle) {
    return strstr(text, needle) != nullptr;
  }

  static CommandGroup classifyCommand(const char* name) {
    if (containsText(name, "Power") || containsText(name, "Off")) {
      return CommandGroup::Power;
    }

    if (containsText(name, "Bright") || containsText(name, "Red") ||
        containsText(name, "Green") || containsText(name, "Blue") ||
        containsText(name, "White") || containsText(name, "Flash") ||
        containsText(name, "Strobe") || containsText(name, "Fade")) {
      return CommandGroup::Lighting;
    }

    if (containsText(name, "Vol") || containsText(name, "Mute") ||
        containsText(name, "Ch ")) {
      return CommandGroup::VolumeChannel;
    }

    if (containsText(name, "Up") || containsText(name, "Down") ||
        containsText(name, "Left") || containsText(name, "Right") ||
        containsText(name, "OK") || containsText(name, "Enter")) {
      return CommandGroup::Navigation;
    }

    if (containsText(name, "Source") || containsText(name, "Input") ||
        containsText(name, "HDMI") || containsText(name, "Menu") ||
        containsText(name, "Exit") || containsText(name, "Return") ||
        containsText(name, "Info") || containsText(name, "Smart") ||
        containsText(name, "Play") || containsText(name, "Pause") ||
        containsText(name, "CD") || containsText(name, "Tuner") ||
        containsText(name, "DVD")) {
      return CommandGroup::SourceMedia;
    }

    return CommandGroup::Other;
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
