# boards

## Zweck

Dieser Ordner enthaelt das Arduino-Boardprofil, das fuer den aktuellen ESP32-S3 genutzt wird.

## Aktives Profil

| Feld | Wert |
| --- | --- |
| Boardname | `IMU Robotic Arm - ESP32-S3-N16R8` |
| FQBN | `esp32:esp32:robotic_arm_s3n16r8` |
| Flash | `16MB` |
| PSRAM | `8MB OPI` |
| Partition | `app3M_fat9M_16MB` |
| USB CDC on Boot | aktiv |

## Nutzung lokal

Die Datei `boards.local.txt` muss zur passenden Arduino-ESP32-Core-Version kopiert werden:

```text
Arduino15/packages/esp32/hardware/esp32/3.3.7/boards.local.txt
```

Danach steht das Profil in `arduino-cli board listall` als
`IMU Robotic Arm - ESP32-S3-N16R8` zur Verfuegung.

## Compile/Upload

```text
arduino-cli compile --fqbn esp32:esp32:robotic_arm_s3n16r8 firmware/esp32_s3_universal_ir_remote
arduino-cli upload -p COM7 --fqbn esp32:esp32:robotic_arm_s3n16r8 firmware/esp32_s3_universal_ir_remote
```

Der Port kann lokal abweichen.
