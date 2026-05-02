# bom

## Zweck

Dieser Ordner sammelt den Materialstand fuer die Universal-IR-Web-Fernbedienung.

## V1-Pflichtteile

| Bauteil | Status | Notiz |
| --- | --- | --- |
| ESP32-S3 Entwicklungsboard | vorhanden | genauer Boardtyp noch dokumentieren |
| KY-005 IR Transmitter oder IR-LED | vorhanden | Modulaufbau pruefen |
| P2N2222 | vorhanden | Pinout am Bauteil pruefen |
| S8050 | vorhanden | Alternative zum P2N2222 |
| Basiswiderstand | benoetigt | Startbereich 1 kOhm bis 2.2 kOhm |
| IR-LED-Vorwiderstand | benoetigt/pruefen | abhaengig vom KY-005-Modul |
| Steckbrett / Lochraster | benoetigt | erst Breadboard, danach Loetstand |

## Vorhanden, aber fuer v1 nicht noetig

| Bauteil | Status | Notiz |
| --- | --- | --- |
| LCD1602 Modul | vorhanden | fuer future Standalone-Modus aufheben |
| Taster | vorhanden | fuer andere Projekte oder spaeteren Tastermodus aufheben |
| Joystick-Modul | vorhanden | future Bedienvariante |

## Pflege

Bei realem Aufbau ergaenzen:

- exakte Modulbezeichnung
- Foto
- gemessene Versorgung
- Besonderheiten beim Pinout
