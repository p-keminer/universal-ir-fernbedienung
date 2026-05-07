# 2026-05-08 Philips TV erster IR-Test

## Aufbau

- Board: `IMU Robotic Arm - ESP32-S3-N16R8`
- FQBN: `esp32:esp32:robotic_arm_s3n16r8`
- Firmwarestand: `716e226` plus lokaler Philips-Teststatus
- IR-GPIO: `GPIO4`
- Transistor: `S8050`
- Basiswiderstand: `1 kOhm`
- KY-005 Versorgung: `3V3 -> 220 Ohm -> KY-005 S`
- Abstand: ca. `20 cm`
- Zielgeraet: Philips TV

## Ergebnis

| Profil | Befehl | Protokoll | Erwartung | Ergebnis | Status |
| --- | --- | --- | --- | --- | --- |
| Philips TV | Power | RC5 | TV Power Toggle | funktioniert | `confirmed` |
| Philips TV | Input | RC5 | Eingang/Quelle | gleiche TV-Reaktion wie `Menu` | erneut testen |
| Philips TV | Menu | RC5 | Menue | gleiche TV-Reaktion wie `Input` | erneut testen |
| Philips TV | Up | RC5 | Navigation hoch | keine sichtbare Reaktion | erneut testen |
| Philips TV | Down | RC5 | Navigation runter | keine sichtbare Reaktion | erneut testen |
| Philips TV | Left | RC5 | Navigation links | keine sichtbare Reaktion | erneut testen |
| Philips TV | Right | RC5 | Navigation rechts | keine sichtbare Reaktion | erneut testen |
| Philips TV | Vol + | RC5 | Lauter | noch nicht bewertet | offen |
| Philips TV | Vol - | RC5 | Leiser | noch nicht bewertet | offen |
| Philips TV | Ch + | RC5 | Sender hoch | noch nicht bewertet | offen |
| Philips TV | Ch - | RC5 | Sender runter | noch nicht bewertet | offen |

## Bewertung

Nur `Power` wird im aktiven Katalog auf `confirmed` gesetzt.
Die anderen Befehle bleiben vorerst `untested`, weil der erste Aufbau mit `220 Ohm` und ca. `20 cm` Abstand noch kein belastbarer Reichweitentest ist.

## Naechste Tests

1. `Vol +` und `Vol -` bei gleicher Ausrichtung testen.
2. `Ch +` und `Ch -` nur testen, wenn Senderwechsel am TV ungefaehrlich ist.
3. `Input` und `Menu` nochmal mit sichtbarem TV-Menuezustand beschreiben.
4. Navigation nochmal testen, wenn ein Menue wirklich offen ist.
5. IR-Strompfad spaeter mit kleinerem passenden Vorwiderstand oder direkter KY-005-Bewertung optimieren.
