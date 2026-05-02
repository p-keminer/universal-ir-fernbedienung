# IR Driver Concept

## Ziel

Der ESP32-S3 soll das KY-005 IR-Sendemodul nicht direkt mit hohem LED-Strom treiben.
Stattdessen schaltet ein NPN-Transistor den IR-Strompfad.

## Vorgesehener Aufbau

```text
ESP32-S3 GPIO
    -> Basiswiderstand
    -> Basis NPN

NPN Emitter
    -> GND

NPN Collector
    -> Minus-/Kathodenseite des IR-Sendepfads

IR-Sendemodul Plus
    -> geeignete Versorgung nach Modulcheck

ESP32-S3 GND
    -> gemeinsame Masse mit IR-Sendepfad
```

## Transistoren

Vorhandene Optionen:

- P2N2222
- S8050

Beide koennen als Low-Side-Schalter funktionieren.
Das konkrete Pinout muss vor dem Einloeten am Bauteil und Datenblatt geprueft werden, weil Gehaeuse und Hersteller abweichen koennen.

## Basiswiderstand

Ein Basiswiderstand ist Pflicht.
Als erster Arbeitsbereich ist ein Wert im Bereich `1 kOhm` bis `2.2 kOhm` sinnvoll.
Der finale Wert haengt vom gewuenschten LED-Strom, Transistor und Modulaufbau ab.

## Offene Pruefpunkte

- Hat das KY-005 Modul bereits einen Vorwiderstand?
- Wird das Modul mit 3,3 V oder 5 V betrieben?
- Welche Reichweite ist mit P2N2222 gegenueber S8050 sichtbar besser?
- Wird der Transistor warm?
- Bleibt der ESP32-S3 GPIO elektrisch sauber?

## Bench-Test

Der erste Test sollte dokumentieren:

- Versorgungsspannung
- Basiswiderstand
- verwendeter Transistor
- GPIO
- Abstand zum Zielgeraet
- Reaktion des Zielgeraets
- Auffaelligkeiten am seriellen Monitor
