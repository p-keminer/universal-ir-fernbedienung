# Wiring Log: YYYY-MM-DD Thema

## Ziel

Kurz beschreiben, welche Verdrahtung aufgebaut oder geaendert wurde.

## Aufbau

| Verbindung | Von | Nach | Notiz |
| --- | --- | --- | --- |
| IR GPIO | ESP32-S3 GPIO4 | Basiswiderstand | Wert eintragen |
| NPN Base | Basiswiderstand | Transistor Base | Pinout geprueft? |
| NPN Emitter | Transistor | GND | gemeinsame Masse |
| NPN Collector | Transistor | KY-005 Minus/Kathode | Modulcheck eintragen |
| KY-005 Plus | Versorgung | KY-005 VCC | Spannung und Vorwiderstand eintragen |

## Verwendete Teile

- ESP32-S3 Board:
- IR-Modul:
- Transistor:
- Basiswiderstand:
- IR-Vorwiderstand:
- Versorgung:

## Pruefung

- [ ] Kurzschlusspruefung
- [ ] gemeinsame Masse bestaetigt
- [ ] Versorgung gemessen
- [ ] GPIO im Sketch passt zum Wiring
- [ ] Dashboard unter `http://192.168.4.1` erreichbar
- [ ] Foto abgelegt

## Ergebnis

Beschreiben, ob der Aufbau funktioniert hat und was als Naechstes geaendert werden muss.
