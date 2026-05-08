# firmware

## Zweck

Dieser Ordner enthaelt die Firmware-Arbeitsstaende fuer die ESP32-S3 Universal-IR-Web-Fernbedienung.

## Aktueller Stand

Der aktive Sketch liegt in:

```text
esp32_s3_universal_ir_remote/
```

Der aktive Web-Sketch kann im Dashboard zwischen Senden und Einlesen wechseln.
Der separate Capture-Sketch zum Einfangen echter Fernbedienungscodes bleibt als Diagnosewerkzeug im Repo:

```text
ir_code_capture/
```

Der Dump-Sketch zum seriellen Auslesen gespeicherter Captures liegt in:

```text
ir_capture_dump/
```

Er nutzt:

- `config/` fuer IR-, WLAN- und Webserver-Defaults
- `ir_catalog/` fuer Profile, Codes und Teststatus
- `ir_receiver/` fuer Einlese-Modus, FFat-Logging und letzten Capture
- `ir_sender/` fuer `IRremoteESP8266`-Sendekapselung
- `web_dashboard/` fuer ESP32-Hotspot, HTTP-Routen und HTML-Dashboard
- `../shared/` fuer interne RGB-Status-LED und Capture-Speicher

Der Web-Sketch und der Capture-Sketch nutzen `GPIO15` als IR-Empfaenger-Eingang fuer `KY-022`, `VS1838B` oder kompatible 38-kHz-Receiver.
Captures werden ueber Serial ausgegeben, im Dashboard angezeigt und in der internen `FFat`-Partition unter `/ir_captures.log` gespeichert.
Im Web-Sketch kann jeder Capture vorher mit einem frei eingetragenen Titel beschriftet werden.
Das Capture-Log kann im Dashboard heruntergeladen oder nach Bestaetigung geloescht werden.

## RGB-Status

Die interne RGB-LED sitzt auf `GPIO48`.

| Farbe | Bedeutung |
| --- | --- |
| Blau | Boot/Sketch gestartet |
| Orange | IR-Befehl wird gesendet |
| Gruen | IR-Senden abgeschlossen |
| Cyan | Empfangen erfolgreich und Capture gespeichert |
| Rot | Speicher-/Sendefehler |

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
