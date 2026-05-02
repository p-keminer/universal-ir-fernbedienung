# IR Remote v1 ASCII Schematic

Gesamtbild fuer die v1-Web-Fernbedienung.
Die Bedienung laeuft ueber WLAN und Browser, deshalb entfallen Taster und LCD im aktiven Aufbau.

## V1-Gesamtskizze

```text
 Handy/Laptop
 Browser: http://192.168.4.1
        |
        | WLAN: IR-Remote
        v

                         ESP32-S3 Dev Module / DevKitC-1
                      +-------------------------------------+
                      |                                     |
USB-C 5V  ----------> | USB / Board-Regler                  |
                      |                                     |
              3V3  o--+----+-------------------------------+---- R_IR 120 Ohm ---- KY-005 Anode/S
                      |    |
              GND  o--+----+-------------------------------+---------------------- GND-Schiene
                      |    |                                                       |
            GPIO4 o---+----+---- R_B 1 kOhm ----+                                  |
                      |                         |                                  |
                      +-------------------------+----------------------------------+
                                                |
                                                +---- R_PD 100 kOhm ---- GND-Schiene
                                                |
                                             Base
                                          +--------+
KY-005 Kathode/GND ---- Collector --------+        |
                                          |  Q1    |
                                          |  NPN   |
GND-Schiene ---------- Emitter -----------+        |
                                          +--------+
```

## V1-Pinbelegung

| Signal | ESP32-S3 Pin | Ziel |
| --- | --- | --- |
| IR_OUT | GPIO4 | `R_B 1 kOhm` -> Q1 Base |
| 3V3 | 3V3 | IR-LED/KY-005 Testversorgung |
| GND | GND | gemeinsame Masse |

## Reihenfolge beim Aufbau

1. ESP32-S3 flashen und Dashboard im Browser pruefen.
2. Nur `IR_OUT`, Q1, KY-005/IR-LED und GND aufbauen.
3. Einen einzelnen Code aus dem Dashboard senden.
4. Reichweite, Winkel und Strompfad dokumentieren.

## Offene Schaltplanpunkte

- KY-005 Vorwiderstand real pruefen.
- Transistor-Pinout am echten Bauteil bestaetigen.
- Versorgung des IR-Sendepfads spaeter eventuell von 3,3 V auf 5 V mit passendem Widerstand umstellen.
