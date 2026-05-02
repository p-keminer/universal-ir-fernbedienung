# ESP32-S3 Universal IR Web Remote

Dieses Repository dokumentiert eine universelle IR-Fernbedienung, bei der der ESP32-S3 einen eigenen WLAN-Hotspot startet und die Bedienung ueber ein Web-Dashboard im Browser laeuft.
Der ESP32-S3 bleibt der IR-Sender: Er erzeugt die IR-Signale auf `GPIO4`, ein NPN-Transistor treibt das KY-005/IR-LED-Modul.

## Projektziel

Gebaut wird eine testfreundliche IR-Fernbedienung fuer TV, LED-Strip, Media-Player, Beamer und Audio-Geraete.
Das Handy oder der Laptop ist nur die Bedienoberflaeche: verbinden mit dem ESP-Hotspot, `http://192.168.4.1` oeffnen, Profil und Befehl antippen.

Taster und LCD sind optional und gehoeren nicht mehr zum v1-Default.

## Aktueller Stand

- Arduino-Sketch startet einen ESP32-S3 Access Point
- Web-Dashboard ist die Standardbedienung mit Suche, Kategorienfilter und Statusanzeige
- IR-Katalog enthaelt 16 Profile und 104 vorbereitete `Untested`-Befehle
- IR-Sender nutzt weiter `IRremoteESP8266`
- Minimal-Hardware ist nur ESP32-S3 plus IR-Sendepfad
- Taster, Joystick und LCD1602 liegen als future/optional zur Seite

## Minimal-Hardware

- ESP32-S3 Entwicklungsboard
- KY-005 IR Transmitter oder einzelne IR-LED
- NPN-Transistor: P2N2222 oder S8050
- Basiswiderstand fuer den Transistor
- passender IR-LED-Vorwiderstand, falls das Modul keinen geeigneten besitzt
- gemeinsame Masse zwischen ESP32-S3 und IR-Treiber

Keine v1-Bedienhardware:

- kein LCD1602
- keine Taster
- kein Joystick

## Dashboard-Start

Nach dem Flashen:

1. ESP32-S3 per USB versorgen.
2. Mit dem WLAN `IR-Remote` verbinden.
3. Passwort `irremote` verwenden.
4. Im Browser `http://192.168.4.1` oeffnen.
5. Kategorie waehlen oder per Suche filtern.
6. Profil und Befehl im Web-Dashboard senden.
7. Diagnostic Sweep nur bewusst ueber die bestaetigte Sweep-Schaltflaeche starten.

## Repository-Struktur

```text
.
|-- firmware/                      Arduino-Sketches und Firmware-Notizen
|-- hardware/                      Minimaler IR-Sendepfad und optionale Hardwareideen
|-- docs/                          Arbeitsnachweise, Fotos, Messungen, Vorlagen
|-- future/                        Taster, LCD, Joystick und weitere Ideen
|-- ARCHITECTURE.md                Systemaufbau und Datenfluss
|-- PROJEKT_ABLAUFPLAN.md          Arbeitsreihenfolge
|-- PROJEKT_FORTSCHRITT.md         Aktueller Managementstand
|-- ROADMAP.md                     Naechste Meilensteine
|-- SAFETY_NOTICE.md               Sicherheits- und Einsatzhinweise
|-- LICENSE.md                     Apache-2.0-Lizenz fuer eigene Inhalte
```

## Wichtigste Dokumente

- `firmware/esp32_s3_universal_ir_remote/README.md` fuer Sketch, Dashboard und Bibliotheken
- `firmware/esp32_s3_universal_ir_remote/web_dashboard/WebDashboard.h` fuer Webserver und Bedienoberflaeche
- `docs/IR_CODE_RESEARCH.md` fuer IR-Code-Quellen, Lizenzhinweise und Sortierregeln
- `hardware/electronics/SCHEMATIC_CURRENT.md` fuer den aktiven ASCII-Schaltplan
- `hardware/electronics/IR_DRIVER_CONCEPT.md` fuer den Transistor-Treiber
- `hardware/electronics/PIN_MAPPING_DRAFT.md` fuer den aktuellen Pin-Entwurf
- `future/README.md` fuer Taster, LCD1602, Joystick und weitere Ausbaustufen

## Firmware

Der aktive Sketch liegt unter:

```text
firmware/esp32_s3_universal_ir_remote/esp32_s3_universal_ir_remote.ino
```

Defaultwerte:

| Funktion | Wert |
| --- | --- |
| Board/FQBN | `esp32:esp32:esp32s3` |
| IR-Ausgang | `GPIO4` |
| WLAN-SSID | `IR-Remote` |
| WLAN-Passwort | `irremote` |
| Dashboard | `http://192.168.4.1` |
| Webserver-Port | `80` |

Compile:

```text
arduino-cli compile --fqbn esp32:esp32:esp32s3 firmware/esp32_s3_universal_ir_remote
```

## Naechste Schritte

1. ESP32-S3 flashen und Dashboard im Handy-Browser oeffnen.
2. IR-Treiber auf Breadboard testen und Reichweite dokumentieren.
3. Einzelne Codes am echten Zielgeraet testen.
4. Ergebnisse von `Untested` auf `Confirmed` oder `Rejected` setzen.
5. Optional spaeter: Favoriten, Captive Portal, gespeicherte Testprotokolle.
