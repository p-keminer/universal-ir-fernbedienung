# Projekt Ablaufplan

Dieses Dokument beschreibt den roten Faden fuer die Universal-IR-Web-Fernbedienung.

## Grundsatz

V1 wird keine voll verdrahtete Handfernbedienung.
Der ESP32-S3 stellt einen Hotspot und ein Web-Dashboard bereit; das Handy uebernimmt die Bedienung.
Hardware wird nur noch fuer den IR-Sendepfad benoetigt.

## Reihenfolge der aktiven Projektarbeit

### 1. Dashboard-Default festziehen

- `Config.h` auf WLAN-AP und Webserver-Defaults ausrichten
- Sketch auf `web_dashboard` plus `ir_sender` reduzieren
- Taster, Joystick und LCD1602 aus dem aktiven Pfad entfernen
- Repo-Tests fuer diese Richtung pflegen

### 2. Web-Dashboard testen

- ESP32-S3 flashen
- mit WLAN `IR-Remote` verbinden
- `http://192.168.4.1` im Browser oeffnen
- einzelne Befehle aus dem Dashboard ausloesen
- Serial Monitor als technisches Log nutzen

### 3. IR-Sendepfad aufbauen

- Transistor als Low-Side-Schalter auf Breadboard testen
- Basiswiderstand einsetzen
- gemeinsame Masse sicherstellen
- KY-005/IR-LED-Strompfad pruefen
- Reichweite und Zielwinkel dokumentieren

### 4. IR-Code-Katalog pflegen

- neue Codes nur als `Untested` aufnehmen
- reale Tests per Vorlage dokumentieren
- erst nach Test auf `Confirmed` oder `Rejected` setzen
- Profile im Dashboard so sortieren, dass Tests schnell gehen

### 5. Praesentationsstand herstellen

- Dashboard-Screenshots oder Fotos vom Testaufbau ergaenzen
- Wiring-Log und Bench-Test-Protokoll schreiben
- README mit getesteten Geraeten aktualisieren
- erste stabile Firmware-Version taggen, wenn Senden reproduzierbar funktioniert

### 6. Future-Pfade erst danach

- Captive Portal fuer automatische Browser-Oeffnung
- Favoriten oder Schnellzugriffe
- optionaler Taster-/LCD-Standalone-Modus
- IR-Lernmodus mit IR-Empfaenger
