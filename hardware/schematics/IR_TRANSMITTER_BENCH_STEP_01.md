# IR Transmitter Bench Step 01

Erster Breadboard-Aufbau fuer die Dashboard-v1-Firmware.

## Ziel

- nur den IR-Sendepfad anschliessen
- keinen OK-Taster anschliessen
- kein LCD anschliessen
- keine weiteren Eingabemodule anschliessen
- Dashboard unter `http://192.168.4.1` als Ausloeser verwenden

## Signale

| Funktion | ESP32-S3 Pin | Notiz |
| --- | --- | --- |
| IR_OUT | GPIO4 | normaler GPIO, steuert NPN-Basis ueber Widerstand |
| 3V3 | 3V3 | erste sichere Testversorgung fuer IR-LED/KY-005 |
| GND | GND | gemeinsame Masse |

## ASCII-Skizze

```text
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

## Aufbaufolge

| Schritt | Aktion |
| --- | --- |
| 1 | ESP32-S3 nur per USB anschliessen |
| 2 | Firmware flashen |
| 3 | WLAN `IR-Remote` verbinden |
| 4 | `http://192.168.4.1` oeffnen |
| 5 | ESP32-S3 GND auf Breadboard-GND-Schiene |
| 6 | NPN-Emitter auf GND-Schiene |
| 7 | NPN-Base ueber `R_B 1 kOhm` an `GPIO4` |
| 8 | `R_PD 100 kOhm` von Base nach GND |
| 9 | KY-005/IR-LED Rueckleitung an NPN-Collector |
| 10 | KY-005/IR-LED Versorgung ueber passenden Vorwiderstand an 3V3 |
| 11 | einzelnen Dashboard-Befehl senden und Zielgeraet beobachten |

## Erwartung

| Zustand | Erwartung |
| --- | --- |
| ESP startet | Serial zeigt AP und Dashboard-IP |
| Handy verbunden | Browser erreicht Dashboard |
| Befehl gesendet | Serial loggt Profil, Protokoll und Daten |
| IR-Test | Zielgeraet reagiert nur bei passendem Code |

## Nicht anschliessen in Step 01

- LCD1602
- Taster
- Joystick
- 5-V-IR-Versorgung ohne neuen Widerstandscheck
