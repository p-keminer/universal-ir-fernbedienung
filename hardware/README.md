# hardware

## Zweck

Dieser Ordner enthaelt die Hardwaredokumentation ausserhalb der Firmware.

## Aktueller Stand

Minimal-Hardware fuer v1:

- ESP32-S3 Entwicklungsboard
- KY-005 IR Transmitter oder einzelne IR-LED
- P2N2222 oder S8050 als NPN-Schalter
- Basiswiderstand
- IR-LED-Vorwiderstand, falls noetig
- gemeinsame Masse

IR-Sendepfad bleibt Pflicht.
Ohne IR-LED/Treiber kann das Dashboard zwar laufen, aber nichts per Infrarot senden.

Nicht mehr Pflicht fuer v1:

- LCD1602 Modul
- Taster
- Joystick-Modul

Diese Teile bleiben als optionale Ideen im `future/`-Ordner und koennen fuer andere Projekte aufgehoben werden.

## Inhalt

- `bom/` fuer Bauteile und Beschaffungsstand
- `electronics/` fuer Treiber, Pinmapping und aktive Minimalverdrahtung
- `assembly/` fuer mechanischen Aufbau und Gehaeuseideen
- `wiring/` fuer reale Leitungsfuehrung
- `schematics/` fuer Schaltplaene und Skizzen

## Regeln

- Entwurf und bestaetigter Aufbau bleiben sichtbar getrennt.
- Pinouts werden nicht geraten, sondern vor dem Loeten am echten Modul geprueft.
- Aenderungen am IR-Strompfad gehoeren in `electronics/` und ein Wiring-Log.
- Fotos und Messungen werden unter `docs/` nachgewiesen.
