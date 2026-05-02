# Input Options

## Entscheidung

Die v1-Bedienung ist jetzt das Web-Dashboard.
Mehrere einzelne Taster oder ein Joystick-Modul werden nicht mehr fuer den ersten Aufbau gebraucht.

## Aktive Option: Web-Dashboard

Vorteile:

- keine zusaetzliche Bedienhardware
- viele Codes lassen sich schnell testen
- Handy oder Laptop liefert grosse, lesbare Oberflaeche
- Profile und Befehle koennen direkt aus dem IR-Katalog gerendert werden
- weniger GPIOs und weniger Loetaufwand

Nachteile:

- braucht ein WLAN-faehiges Bediengeraet
- ohne Captive Portal muss `http://192.168.4.1` manuell geoeffnet werden
- nicht so direkt wie echte Tasten in der Hand

## Future: Einzelne Taster

Taster bleiben sinnvoll, falls spaeter ein komplett eigenstaendiger Handheld entstehen soll.
Sie wuerden mit `INPUT_PULLUP` gegen GND verschaltet.

## Future: Joystick-Modul

Ein Joystick bleibt interessant fuer Menues, braucht aber ADC-/Entprelllogik und mechanisch mehr Tests.

## Empfehlung

V1 bleibt beim Web-Dashboard.
Taster und Joystick erst wieder anfassen, wenn Hotspot, Dashboard und IR-Senden stabil laufen.
