# 2026-05-08 Reichweite und LED-Strip-Test

## Aufbau

- Board: `IMU Robotic Arm - ESP32-S3-N16R8`
- FQBN: `esp32:esp32:robotic_arm_s3n16r8`
- IR-GPIO: `GPIO4`
- Transistor: `S8050`
- Basiswiderstand: `1 kOhm`
- IR-Vorwiderstand: `3x 220 Ohm parallel`, ca. `73 Ohm`
- Versorgung: `3V3`
- IR-Modul: KY-005

## Beobachtung

- Transistor wird beim Test nicht warm.
- KY-005 wird beim Test nicht warm.
- Philips TV Power funktioniert mit ca. `2,5 m` Abstand.
- Manche LED-Strips reagieren.
- LED-Strip `Flash` und `Strobe` reagieren.
- LED-Strip Power On reagiert bisher nicht.
- Farbwechsel reagieren teilweise.

## Bewertung

Der aktuelle IR-Sendepfad ist fuer erste Raumtests brauchbar.
Fuer verlaessliche Geraetecodes sollen Originalfernbedienungen mit einem IR-Empfaenger gecaptured werden.

## Naechste Schritte

1. IR-Receiver-Modul anschliessen.
2. Capture-Sketch flashen.
3. Philips-TV-Fernbedienung capturen: Power, Input, Menu, Up, Down, Left, Right, Vol +, Vol -, Ch +, Ch -.
4. LED-Strip-Fernbedienung capturen: Power, Flash, Strobe, Farben.
5. Stabile Captures in `IrCatalog.h` uebernehmen.
