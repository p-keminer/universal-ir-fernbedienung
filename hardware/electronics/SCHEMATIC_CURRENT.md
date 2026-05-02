# Schematic Current

Aktueller Elektronikstand der Universal-IR-Web-Fernbedienung, Stand 2026-05-02.

## Einordnung

Dieses Dokument ist die aktive Projektreferenz fuer Verdrahtung und Pinbelegung.
Der erste reale Aufbau bleibt bewusst klein:

- ESP32-S3 per USB
- IR-Sendepfad ueber NPN-Low-Side-Schalter
- keine Taster
- kein LCD
- keine Joystick-Leitungen

Bedienung erfolgt ueber das Web-Dashboard des ESP32-S3.

## ESP32-S3 Signale

| Signal | Pin | Aktuelle Funktion |
| --- | --- | --- |
| IR_OUT | GPIO4 | IR-Sendesignal zur NPN-Basis ueber `R_B` |
| 3V3 | 3V3 | erste IR-LED-Testversorgung |
| GND | GND | gemeinsame Masse fuer ESP32-S3, Transistor und KY-005/IR-LED |

Bewusst frei bleiben im ersten Aufbau:

- GPIO0, GPIO3, GPIO45, GPIO46 wegen Strapping-/Boot-Funktion
- GPIO19, GPIO20 wegen USB D-/D+
- GPIO35, GPIO36, GPIO37 wegen moeglicher interner Flash/PSRAM-Nutzung
- GPIO38/GPIO48 wegen RGB-LED-Boardrevisionen
- GPIO43/GPIO44 wegen UART0

## Aktiver ASCII-Schaltplan

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

## Netzliste

| Netz | Verbindung |
| --- | --- |
| `3V3_IR` | ESP32-S3 `3V3` -> `R_IR 120 Ohm` -> KY-005 Anode/Signal |
| `IR_RETURN` | KY-005 Kathode/GND -> NPN Collector |
| `IR_DRIVE` | ESP32-S3 `GPIO4` -> `R_B 1 kOhm` -> NPN Base |
| `IR_BASE_PD` | NPN Base -> `R_PD 100 kOhm` -> GND |
| `GND` | ESP32-S3 GND -> NPN Emitter -> KY-005-GND-Bezug |

## Bauteilwerte

| Bauteil | Wert | Zweck |
| --- | --- | --- |
| `R_IR` | `120 Ohm` | erster sicherer 3,3-V-Vorwiderstand fuer KY-005/IR-LED |
| `R_B` | `1 kOhm` | begrenzt Basisstrom vom ESP32-S3 GPIO |
| `R_PD` | `100 kOhm` | haelt NPN-Basis beim Booten definiert aus |
| `Q1` | P2N2222A oder S8050 | NPN-Low-Side-Schalter |

## Transistor-Regel

Im Schaltplan zaehlen die Netznamen, nicht die Beinchenposition aus dem Kopf:

- Collector an `IR_RETURN`
- Base an `IR_DRIVE`
- Emitter an `GND`

P2N2222A und S8050 koennen je nach Hersteller unterschiedlich angeordnet sein.
Vor dem Stecken am echten Bauteil Pinout pruefen.

## Schnittstellen/Abhaengigkeiten

- `PIN_MAPPING_DRAFT.md`
- `../schematics/IR_TRANSMITTER_BENCH_STEP_01.md`
- `../schematics/IR_REMOTE_V1_ASCII.md`
