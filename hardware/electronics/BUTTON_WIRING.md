# Button Wiring

## Einordnung

Button-Wiring ist nicht Teil von Dashboard-v1.
Dieses Dokument bleibt als future-Notiz erhalten, falls spaeter ein Standalone-Handheld ohne Handy entstehen soll.

## Future-Prinzip

Ein einfacher Tasterpfad waere:

```text
ESP32-S3 GPIO ---- Taster ---- GND
```

Die Firmware koennte dann `INPUT_PULLUP` nutzen:

- ungedrueckt: `HIGH`
- gedrueckt: `LOW`

## Future-Belegung

Noch keine aktive Belegung.
Falls der Tastermodus zurueckkommt, sollten Pins neu gegen das konkrete ESP32-S3 Board geprueft werden.

| Taste | GPIO | Status |
| --- | --- | --- |
| Up | offen | future |
| Down | offen | future |
| Left | offen | future |
| Right | offen | future |
| OK | offen | future |
| Back | offen | future |

## Regel

Erst Dashboard-v1 testen.
Taster werden nicht angeschlossen, solange der Web-Bedienpfad aktiv entwickelt wird.
