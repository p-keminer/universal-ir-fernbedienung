# electronics

## Zweck

Dieser Ordner beschreibt den elektronischen Aufbau der IR-Web-Fernbedienung.

## Dokumente

- `SCHEMATIC_CURRENT.md` beschreibt den aktiven Minimalstand im ASCII-Stil.
- `IR_DRIVER_CONCEPT.md` beschreibt den NPN-Treiber fuer das KY-005/IR-LED-Modul.
- `PIN_MAPPING_DRAFT.md` sammelt GPIOs und offene Pinentscheidungen.
- `DATASHEET_NOTES.md` sammelt Quellen und Bauteilhinweise.
- `INPUT_OPTIONS.md`, `BUTTON_WIRING.md` und `LCD1602_I2C_CHECK.md` bleiben als optionale/future Notizen erhalten.

## Aktueller Schwerpunkt

Die Firmware nutzt fuer v1 nur `GPIO4` als IR-Ausgang.
Bedienung und Anzeige laufen ueber das Web-Dashboard unter `http://192.168.4.1`.
Taster, Joystick und LCD1602 werden fuer den aktuellen Aufbau nicht verdrahtet.
