# Architektur

## Systemidee

Die v1-Fernbedienung besteht aus drei aktiven Teilen:

- Bedienung: Handy oder Laptop oeffnet das Web-Dashboard des ESP32-S3
- Steuerung: ESP32-S3 stellt WLAN-Hotspot, Webserver, IR-Codeauswahl und Timing bereit
- IR-Ausgang: NPN-Transistor treibt das KY-005/IR-LED-Modul

Taster, Joystick und LCD1602 sind keine v1-Pflicht mehr.
Sie bleiben als spaetere Offline-/Standalone-Option im `future/`-Bereich.

## Datenfluss

```text
Handy/Laptop Browser
    -> WLAN-Hotspot IR-Remote
    -> ESP32-S3 WebServer Route /send
    -> IR-Katalog
    -> IRremoteESP8266 Sendefunktion
    -> GPIO4
    -> Basiswiderstand
    -> NPN-Transistor
    -> KY-005 oder IR-LED
    -> Zielgeraet
```

Der Diagnostic Sweep bleibt manuell erreichbar, aber nicht als Dauerschleife.

## Verantwortlichkeiten

- `firmware/` enthaelt den aktiven ESP32-S3 Sketch.
- `web_dashboard/` erzeugt das HTML-Dashboard und verarbeitet Sendekommandos.
- `ir_catalog/` enthaelt Profile, Befehle, Protokolle und Teststatus.
- `ir_sender/` kapselt `IRremoteESP8266`.
- `hardware/` enthaelt nur den real noetigen IR-Sendepfad plus optionale Notizen.
- `docs/` enthaelt Bench-Tests, Fotos, Messungen und Arbeitsprotokolle.
- `future/` sammelt Bedienhardware und Komfortfunktionen, die v1 nicht blockieren.

## Designregeln

- Dashboard-Bedienung ist Default.
- IR-Senden passiert nur nach bewusstem Web-Klick.
- Der ESP32-S3 Access Point funktioniert ohne externen Router.
- IR-Codes aus Datenbanken bleiben `Untested`, bis ein realer Test dokumentiert ist.
- Der IR-Sendepfad bleibt elektrisch einfach und nachvollziehbar.
