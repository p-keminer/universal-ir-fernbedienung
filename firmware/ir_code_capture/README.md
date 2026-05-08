# ir_code_capture

## Zweck

Dieser Sketch dient nur zum Einfangen echter IR-Codes von vorhandenen Fernbedienungen.
Er ersetzt fuer den Capture-Test den Web-Remote-Sketch.

## Hardware

IR-Empfaenger wie `KY-022`, `VS1838B` oder kompatibles 38-kHz-Receiver-Modul:

| Empfaenger | ESP32-S3 |
| --- | --- |
| `S` / `OUT` | `GPIO15` |
| `+` / `VCC` | `3V3` |
| `-` / `GND` | `GND` |

Den Empfaenger mit `3V3` betreiben, damit der Ausgang nicht auf 5 V gegen den ESP32-S3 zieht.

## Compile

```text
arduino-cli compile --fqbn esp32:esp32:robotic_arm_s3n16r8 firmware/ir_code_capture
```

## Upload

```text
arduino-cli upload -p COM7 --fqbn esp32:esp32:robotic_arm_s3n16r8 firmware/ir_code_capture
```

Der Port kann lokal abweichen.

## Ablauf

1. Capture-Sketch flashen.
2. Serial Monitor mit `115200` Baud oeffnen.
3. Originalfernbedienung 10-50 cm vor den Empfaenger halten.
4. Eine Taste kurz druecken, nicht halten.
5. `Protocol`, `Code`, `Bits` und `uint16_t rawData[]` in ein Testprotokoll uebernehmen.
6. Drei Captures pro Taste vergleichen.

Nur wiederholbare Captures werden in den aktiven Katalog uebernommen.

## Speicher und RGB

- Serial gibt jeden Capture sofort mit `Protocol`, `Code`, `Bits` und Source-Code aus.
- Zusaetzlich wird jeder Capture intern in der `FFat`-Partition unter `/ir_captures.log` gespeichert.
- Die interne RGB-LED auf `GPIO48` blinkt Cyan, wenn der Capture empfangen und gespeichert wurde.
- Rot bedeutet: Capture wurde zwar erkannt, konnte aber nicht gespeichert werden.

Zum Auslesen des internen Logs gibt es den Sketch `firmware/ir_capture_dump`.
