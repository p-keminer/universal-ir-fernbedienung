# ir_capture_dump

## Zweck

Dieser Sketch liest das interne Capture-Log aus der FFat-Partition des ESP32-S3
und gibt es ueber Serial aus.

## Compile

```text
arduino-cli compile --fqbn esp32:esp32:robotic_arm_s3n16r8 firmware/ir_capture_dump
```

## Upload

```text
arduino-cli upload -p COM7 --fqbn esp32:esp32:robotic_arm_s3n16r8 firmware/ir_capture_dump
```

## Ablauf

1. Sketch flashen.
2. Serial Monitor mit `115200` Baud oeffnen.
3. Logausgabe kopieren und in ein Capture-Protokoll uebernehmen.

Das Log liegt intern unter `/ir_captures.log`.
