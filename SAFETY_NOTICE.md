# Safety Notice

Dieses Projekt sendet Infrarotbefehle, die echte Geraete steuern koennen.
Auch wenn der Aufbau klein wirkt, koennen TV, Beamer, LED-Controller, Audioanlagen oder Media-Player unbeabsichtigt reagieren.

## Grundregeln

- IR-Tests nur an eigenen oder freigegebenen Geraeten durchfuehren.
- Sender beim Entwickeln nicht unkontrolliert in den Raum richten.
- Automatische Testschleifen nur kurz und bewusst starten.
- Keine Dauersendung in oeffentlichen oder fremden Umgebungen.
- Vor Messungen am Transistorpfad Stromversorgung trennen.
- Pinout von P2N2222/S8050 vor dem Einloeten am konkreten Bauteil pruefen.
- Das Dashboard-WLAN nur in kontrollierter Umgebung betreiben und Passwort nicht unnoetig teilen.

## Elektrische Hinweise

- ESP32-S3 GPIOs arbeiten mit 3,3 V Logik.
- Gemeinsame Masse zwischen ESP32-S3 und IR-Treiber ist Pflicht.
- LCD1602 ist fuer v1 nicht angeschlossen; falls spaeter genutzt, I2C-Pullups vor Anschluss pruefen.
- IR-LED-Strom nicht direkt ueber einen GPIO treiben, wenn mehr Reichweite gewuenscht ist.

## Dokumentationspflicht

Sicherheitsrelevante Aenderungen an Strompfad, Pinmapping oder Testverhalten gehoeren in:

- `hardware/electronics/PIN_MAPPING_DRAFT.md`
- `hardware/electronics/IR_DRIVER_CONCEPT.md`
- ein dated Wiring- oder Bench-Test-Protokoll unter `docs/`
