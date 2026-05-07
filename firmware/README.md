# firmware

## Zweck

Dieser Ordner enthaelt die Firmware-Arbeitsstaende fuer die ESP32-S3 Universal-IR-Web-Fernbedienung.

## Aktueller Stand

Der aktive Sketch liegt in:

```text
esp32_s3_universal_ir_remote/
```

Der Capture-Sketch zum Einfangen echter Fernbedienungscodes liegt in:

```text
ir_code_capture/
```

Er nutzt:

- `config/` fuer IR-, WLAN- und Webserver-Defaults
- `ir_catalog/` fuer Profile, Codes und Teststatus
- `ir_sender/` fuer `IRremoteESP8266`-Sendekapselung
- `web_dashboard/` fuer ESP32-Hotspot, HTTP-Routen und HTML-Dashboard

Der Capture-Sketch nutzt `GPIO15` als IR-Empfaenger-Eingang fuer `KY-022`, `VS1838B` oder kompatible 38-kHz-Receiver.

Nicht mehr im v1-Default:

- physische Taster
- Joystick
- LCD1602
- lokale State Machine fuer Hardware-Buttons

## Regeln

- Web-Dashboard ist die Standardbedienung.
- IR-Codes mit Quelle oder Testnachweis dokumentieren.
- Testsequenzen klar von normaler Bedienung trennen.
- GPIOs nur dann als final bezeichnen, wenn sie am echten Board getestet wurden.
- Keine lokalen Boardpfade oder privaten Geraetedaten einchecken.

## Abhaengigkeiten

- Arduino IDE oder kompatible Arduino-CLI Umgebung
- ESP32 Board Support Package
- Bibliothek `IRremoteESP8266`
- ESP32-Arduino Standardbibliotheken `WiFi` und `WebServer`
