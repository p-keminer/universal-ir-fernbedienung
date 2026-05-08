# Interne RGB-Status-LED

## Default

| Signal | Wert |
| --- | --- |
| RGB-Datenpin | `GPIO48` |
| LED-Typ | interne adressierbare RGB-LED / WS2812-kompatibel |
| Ansteuerung | `rgbLedWrite()` aus Arduino-ESP32 |

## Farbcode

| Farbe | RGB | Bedeutung |
| --- | --- | --- |
| Blau | `0, 0, 48` | Boot/Sketch gestartet |
| Orange | `96, 32, 0` | IR-Senden aktiv |
| Gruen | `0, 64, 0` | Senden abgeschlossen |
| Cyan | `0, 48, 64` | Empfangen erfolgreich und gespeichert |
| Rot | `96, 0, 0` | Fehler beim Senden oder Speichern |
| Aus | `0, 0, 0` | Idle |

## Hinweise

- GPIO48 wurde am Board als interne RGB-LED gesetzt.
- Die LED ist nur Statusanzeige; sie ist kein Teil des IR-Sendepfads.
- Empfangen erfolgreich bedeutet beim Capture-Sketch: IR-Code wurde decodiert, seriell ausgegeben und in `/ir_captures.log` geschrieben.
