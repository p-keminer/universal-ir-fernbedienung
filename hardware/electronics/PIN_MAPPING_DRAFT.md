# Pin Mapping Draft

Dieses Dokument ist ein Entwurf.
Pins werden erst nach realem Boardcheck als bestaetigt markiert.

## Default-Pinmapping v1

| Funktion | Pin | Status | Notiz |
| --- | --- | --- | --- |
| IR-Ausgang | GPIO4 | Default | treibt NPN-Basis ueber Widerstand |
| USB | Board-USB | Default | Stromversorgung, Flashen, Serial Monitor |
| WLAN AP | intern | Default | SSID `IR-Remote`, Dashboard `192.168.4.1` |

## Nicht belegt in v1

| Funktion | Status | Notiz |
| --- | --- | --- |
| LCD1602 SDA/SCL | future | nicht fuer Dashboard-v1 anschliessen |
| Taster | future | Bedienung laeuft ueber Browser |
| Joystick | future | spaetere Handheld-Option |

## Firmware-Konfig

Die Defaultwerte stehen in:

```text
firmware/esp32_s3_universal_ir_remote/config/Config.h
```

Aktuelle Werte:

- IR-Ausgang: `GPIO4`
- WLAN-SSID: `IR-Remote`
- WLAN-Passwort: `irremote`
- Dashboard: `http://192.168.4.1`
- IR command gap: `200 ms`

## Regeln

- Bootstrapping- und Spezialpins des konkreten ESP32-S3 Boards vor Nutzung pruefen.
- Fuer ESP32-S3 insbesondere GPIO0, GPIO3, GPIO45 und GPIO46 nicht fuer den ersten Aufbau verwenden.
- GPIO19/GPIO20 fuer USB und GPIO43/GPIO44 fuer UART0 frei lassen.
- GPIO35/GPIO36/GPIO37 bei Octal-Flash/PSRAM-Varianten meiden.
- Keine Taster oder LCDs anschliessen, solange Dashboard-v1 getestet wird.

## Aenderungslog

| Datum | Aenderung |
| --- | --- |
| 2026-05-01 | erster Pin-Entwurf aus Sketch und Hardwareidee angelegt |
| 2026-05-01 | Zwischenstand mit LCD-/Taster-Pins dokumentiert |
| 2026-05-02 | v1 auf Web-Dashboard umgestellt; nur IR-Ausgang bleibt aktiv |
