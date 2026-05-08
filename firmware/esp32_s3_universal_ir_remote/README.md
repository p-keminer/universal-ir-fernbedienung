# esp32_s3_universal_ir_remote

## Zweck

Arduino-Arbeitsstand fuer die ESP32-S3 Universal-IR-Web-Fernbedienung.
Der ESP32-S3 startet einen eigenen WLAN-Hotspot und liefert ein Web-Dashboard aus.

## Bibliotheken

- `IRremoteESP8266`
- `WiFi` aus dem ESP32-Arduino-Core
- `WebServer` aus dem ESP32-Arduino-Core

## Aktuelles Verhalten

Der Sketch:

1. startet Serial mit `115200`
2. initialisiert den IR-Sender auf `GPIO4`
3. initialisiert den IR-Empfaenger auf `GPIO15`
4. startet den Access Point `IR-Remote`
5. stellt das Dashboard unter `http://192.168.4.1` bereit
6. wechselt im Dashboard zwischen `Senden` und `Einlesen`
7. zeigt nach Kategorieauswahl erst passende Marken/Profile
8. oeffnet danach eine fernbedienungsartige Tastenansicht pro Profil
9. sendet den gewaehlten IR-Befehl ueber `/send`, ohne die Auswahlansicht zu verlassen
10. schreibt empfangene IR-Codes seriell und in `/ir_captures.log`
11. bietet den Diagnostic Sweep nur manuell und mit Browser-Bestaetigung ueber `/sweep` an

Die alte automatische Dauerschleife ist entfernt.
Taster, Joystick und LCD1602 sind nicht Teil des v1-Defaults.

## Dashboard

| Wert | Default |
| --- | --- |
| SSID | `IR-Remote` |
| Passwort | `irremote` |
| IP | `192.168.4.1` |
| Port | `80` |

## UI-Funktionen

- Auswahlstufe 1: Kategorie, z.B. TV, Media oder LED Strip
- Auswahlstufe 2: Marke/Profil innerhalb der Kategorie
- Fernbedienungsansicht mit gruppierten Befehlen fuer Power, Lautstaerke/Sender, Navigation, Eingang/Media, Licht/Farbe und Weitere
- Suche ueber Profil- und Befehlsnamen
- Kategorienfilter fuer TV, Media, LED Strip, Projektor, Audio und Test
- Status-/Protokoll-Tags pro Befehl
- Power-/Off-Befehle mit Bestaetigungsdialog gegen versehentliches Senden
- Diagnostic Sweep mit Bestaetigungsdialog

## Routen

| Route | Zweck |
| --- | --- |
| `/` | Dashboard mit Profilen und Befehlen |
| `/send?profile=N&command=M` | einzelnen Befehl senden |
| `/sweep` | manuelle Diagnostic-Sweep-Sequenz senden |
| `/mode?mode=send` | State Machine auf Senden stellen |
| `/mode?mode=capture` | State Machine auf Einlesen stellen |
| `/capture/status` | aktueller Modus und letzter Capture als JSON |
| `/captures/download` | gespeicherte Captures als Textdatei herunterladen |
| `/captures/clear` | gespeicherte Captures loeschen |

## Senden und Einlesen

Der Hauptsketch kann beide Betriebsarten ohne erneutes Flashen:

- `Senden`: IR-Buttons und Diagnostic Sweep sind aktiv.
- `Einlesen`: IR-Empfaenger ist aktiv, Senden wird blockiert.
- Der Capture-Titel wird im Dashboard frei eingetragen und mit `Titel setzen` aktiviert.
- Jeder danach empfangene Code wird mit diesem Titel protokolliert.
- Der letzte empfangene Code erscheint im Dashboard im Bereich `Letzter Empfang`.
- Alle Captures werden in der internen `FFat`-Datei `/ir_captures.log` gesammelt.
- Die Datei kann ueber `Download` im Dashboard heruntergeladen werden.
- Die Datei kann ueber `Loeschen` im Dashboard nach Bestaetigung entfernt werden.

## Naechste Firmware-Schritte

- Dashboard auf echtem ESP32-S3 im Handy-Browser testen
- Fehlermeldungen im Dashboard weiter verbessern
- optional Captive Portal ergaenzen
- IR-Codes am Zielgeraet bestaetigen
