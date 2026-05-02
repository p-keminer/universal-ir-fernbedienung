# LCD1602 I2C Check

## Einordnung

LCD1602 ist nicht Teil von Dashboard-v1.
Dieses Dokument bleibt als future-Notiz erhalten, falls spaeter ein Standalone-Modus ohne Handy entstehen soll.

## Future-Anschluss

| Signal | GPIO | Notiz |
| --- | --- | --- |
| SDA | offen | erst bei LCD-Future-Pfad festlegen |
| SCL | offen | erst bei LCD-Future-Pfad festlegen |
| VCC | offen | Modulaufdruck und Pullups pruefen |
| GND | GND | gemeinsame Masse |

Typische I2C-Adressen sind `0x27` und `0x3F`.
Fuer Dashboard-v1 wird keine LCD-Library installiert und kein I2C initialisiert.

## Wichtige Pruefung

Viele LCD1602-I2C-Backpacks ziehen SDA/SCL ueber Pullups auf VCC.
Wenn das Backpack mit `5 V` versorgt wird, koennen SDA/SCL dadurch ebenfalls auf `5 V` liegen.
Vor direktem Anschluss an den ESP32-S3 muessen Pullup-Spannung oder Level-Shifting geprueft werden.

## Future-Testablauf

1. LCD separat vom IR-Sendepfad pruefen.
2. Versorgung messen.
3. SDA/SCL Pullup-Spannung messen.
4. I2C-Adresse scannen.
5. Erst danach Firmware um LCD-Anzeige erweitern.
