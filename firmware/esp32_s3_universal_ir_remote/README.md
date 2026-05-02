# esp32_s3_universal_ir_remote

## Zweck

Arduino-Arbeitsstand fuer die ESP32-S3 Universal-IR-Web-Fernbedienung.
Der ESP32-S3 startet einen eigenen WLAN-Hotspot und liefert ein Web-Dashboard aus.

## Bibliotheken

- `IRremoteESP8266`
- `WiFi` aus dem ESP32-Arduino-Core
- `WebServer` aus dem ESP32-Arduino-Core

## Aktuelles Verhalten

Der Sketch:

1. startet Serial mit `115200`
2. initialisiert den IR-Sender auf `GPIO4`
3. startet den Access Point `IR-Remote`
4. stellt das Dashboard unter `http://192.168.4.1` bereit
5. sendet den gewaehlten IR-Befehl ueber `/send`
6. bietet den Diagnostic Sweep nur manuell ueber `/sweep` an

Die alte automatische Dauerschleife ist entfernt.
Taster, Joystick und LCD1602 sind nicht Teil des v1-Defaults.

## Dashboard

| Wert | Default |
| --- | --- |
| SSID | `IR-Remote` |
| Passwort | `irremote` |
| IP | `192.168.4.1` |
| Port | `80` |

## Routen

| Route | Zweck |
| --- | --- |
| `/` | Dashboard mit Profilen und Befehlen |
| `/send?profile=N&command=M` | einzelnen Befehl senden |
| `/sweep` | manuelle Diagnostic-Sweep-Sequenz senden |

## Naechste Firmware-Schritte

- Dashboard auf echtem ESP32-S3 im Handy-Browser testen
- Fehlermeldungen im Dashboard weiter verbessern
- optional Captive Portal ergaenzen
- IR-Codes am Zielgeraet bestaetigen
