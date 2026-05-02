# Projekt Fortschritt

Stand: 2026-05-02

## Gesamtstatus

- Projektmodus: frueher Firmware-/Hardware-Prototyp
- Aktiver Schwerpunkt: ESP32-S3 Hotspot, Web-Dashboard und IR-Sendepfad
- Firmware: Dashboard-v1 mit erweitertem IR-Code-Katalog vorhanden
- Hardware: nur IR-Sendepfad ist fuer v1 Pflicht
- Eingabeentscheidung: Handy/Laptop im Browser; Taster, Joystick und LCD sind future

## Phasenampel

| Phase | Status | Einordnung |
| --- | --- | --- |
| Doku / Repo | in Arbeit | Grundstruktur ist angelegt und wird auf Dashboard-v1 aktualisiert |
| Web-Dashboard | in Arbeit | Access Point und Webserver sind Firmware-Default |
| IR-Sendepfad | offen | Schaltung und Pinmapping muessen real getestet werden |
| IR-Code-Test | offen | Katalog ist vorbereitet, reale Tests fehlen |
| Future-Hardware | geparkt | Taster, Joystick und LCD blockieren v1 nicht |

## Bereits erledigt

- [x] Projektziel und Repository-Struktur definiert
- [x] Hardware- und Firmwareordner angelegt
- [x] Treiberkonzept fuer P2N2222/S8050 dokumentiert
- [x] IR-Code-Kandidaten mit Status `Untested` in den Katalog uebernommen
- [x] Katalog auf 16 Profile und 104 Befehle erweitert
- [x] Datenblatt- und Quellenanker dokumentiert
- [x] Dashboard-v1 als neue Bedienrichtung festgelegt
- [x] Taster, Joystick und LCD1602 aus dem v1-Default entfernt

## Noch offen im aktuellen Schwerpunkt

- [ ] ESP32-S3 flashen
- [ ] WLAN `IR-Remote` auf Handy/Laptop testen
- [ ] Dashboard unter `http://192.168.4.1` oeffnen
- [ ] reale Boardvariante und GPIO-Einschraenkungen pruefen
- [ ] IR-Treiber auf Breadboard aufbauen
- [ ] KY-005/IR-LED Stromaufnahme und Reichweite testen
- [ ] IR-Codes pro Zielgeraet bestaetigen oder verwerfen

## Management-Risiken

- ESP32-S3 Boards unterscheiden sich bei nutzbaren Pins, Bootstrapping und Beschriftung.
- KY-005 Module sind nicht immer gleich aufgebaut; der reale Anschluss muss gemessen werden.
- Der ESP32-Hotspot ist lokal und bequem, aber ohne Authentifizierung jenseits des WPA-Passworts.
- Automatische IR-Testschleifen koennen echte Geraete unbeabsichtigt schalten; Sweep bleibt manuell.

## Pflegepflicht

- Nach jedem realen Verdrahtungsschritt `docs/templates/WIRING_LOG_TEMPLATE.md` als Kopiervorlage nutzen.
- Nach jedem Testlauf ein kurzes Bench-Test-Protokoll in `docs/` ablegen.
- README und Pinmapping aktualisieren, wenn aus Entwurf ein bestaetigter Stand wird.
