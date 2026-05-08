# 2026-05-08 RGB-Status und Capture-Speicher

## Ziel

Der ESP32-S3 soll beim Senden und Empfangen eine sichtbare Rueckmeldung ueber
die interne RGB-LED geben. Empfangene IR-Codes sollen zusaetzlich seriell
ausgegeben und intern gespeichert werden.

## Farbcode

| Farbe | Bedeutung |
| --- | --- |
| Blau | Sketch gestartet |
| Orange | IR-Befehl wird gesendet |
| Gruen | IR-Senden abgeschlossen |
| Cyan | IR-Code empfangen und gespeichert |
| Rot | Fehler beim Senden oder Speichern |

## Umsetzung

- RGB-LED: `GPIO48`
- Capture-Eingang: `GPIO15`
- Interner Speicher: `FFat`
- Log-Datei: `/ir_captures.log`
- Capture-Sketch: `firmware/ir_code_capture`
- Dump-Sketch: `firmware/ir_capture_dump`

## Flash-Stand

Am 2026-05-08 wurde `firmware/ir_code_capture` auf `COM7` geflasht.
Serial-Startausgabe bestaetigt:

```text
IR-Code-Capture bereit
Empfaenger: GPIO15
Interne RGB-LED: GPIO48
Capture-Log: /ir_captures.log
FFat Capture-Speicher bereit.
```

## Naechster Test

Originalfernbedienung auf den KY-022 richten und je Taste drei kurze Captures
vergleichen. Wiederholbare Codes werden danach in den aktiven IR-Katalog
uebernommen.
