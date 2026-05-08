# 2026-05-08 Integriertes Senden und Einlesen

## Ziel

Der Hauptsketch soll ohne erneutes Flashen zwischen Senden und Einlesen wechseln.
Der separate Capture-Sketch bleibt als Diagnosewerkzeug im Repo.

## Umsetzung

- Hauptsketch: `firmware/esp32_s3_universal_ir_remote`
- IR-Senden: `GPIO4`
- IR-Empfangen: `GPIO15`
- RGB-LED: `GPIO48`
- Capture-Speicher: `FFat`, Datei `/ir_captures.log`

## Dashboard

Neue Bedienung im Dashboard:

- Button `Senden`
- Button `Einlesen`
- Freies Textfeld `Capture-Titel`
- Button `Titel setzen`
- Bereich `Letzter Empfang`
- Download-Link fuer `/captures/download`
- Loesch-Button fuer `/captures/clear`

Neue Routen:

| Route | Zweck |
| --- | --- |
| `/mode?mode=send` | Senden aktivieren |
| `/mode?mode=capture` | Einlesen aktivieren |
| `/capture/status` | Modus und letzter Capture als JSON |
| `/capture/label` | frei eingetragenen Capture-Titel setzen |
| `/captures/download` | Capture-Log herunterladen |
| `/captures/clear` | Capture-Log nach Bestaetigung loeschen |

Der aktive Titel wird mit jedem neu empfangenen Code als `Titel:` in das Log geschrieben.

## Flash-Stand

Am 2026-05-08 wurde der Hauptsketch mit dem Custom-FQBN geflasht:

```text
arduino-cli upload -p COM7 --fqbn esp32:esp32:robotic_arm_s3n16r8 firmware/esp32_s3_universal_ir_remote
```

Der Upload wurde von `esptool` abgeschlossen und der Hash wurde verifiziert.
