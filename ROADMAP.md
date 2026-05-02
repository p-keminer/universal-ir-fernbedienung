# Roadmap

## Phase 1: Repository- und Dokumentationsbasis

- [x] GitHub-taugliche Grundstruktur anlegen
- [x] Root-README, Roadmap, Architektur und Projektstand erstellen
- [x] Hardware- und Firmwarebereiche trennen
- [x] Vorlagen fuer Wiring- und Bench-Test-Protokolle anlegen
- [x] Datenblatt- und IR-Code-Quellen dokumentieren

## Phase 2: Web-Dashboard als v1-Bedienung

- [x] Taster/LCD-Default verwerfen
- [x] ESP32-S3 Access Point als Standard festlegen
- [x] Web-Dashboard in die Firmware aufnehmen
- [x] Dashboard-Route fuer einzelne Befehle anlegen
- [x] Diagnostic Sweep nur manuell per Webroute erreichbar machen
- [ ] Dashboard am echten ESP32-S3 im Handy-Browser testen

## Phase 3: IR-Sendepfad nachweisen

- [ ] ESP32-S3 Boardtyp und nutzbare GPIOs bestaetigen
- [ ] P2N2222 und S8050 Pinout am realen Bauteil pruefen
- [ ] Basiswiderstand einsetzen und Strompfad messen
- [ ] KY-005 oder IR-LED mit Dashboard-Befehl pruefen
- [ ] Reichweite und Zielwinkel dokumentieren

## Phase 4: IR-Codes testen und sortieren

- [ ] erste TV-Power-Codes testen
- [ ] LED-Strip-Codes testen
- [ ] Beamer-/Projektor-Codes testen
- [ ] Audio-/Receiver-Codes testen
- [ ] `Confirmed`-/`Rejected`-Status im Katalog pflegen

## Phase 5: GitHub-Praesentation

- [ ] Foto vom Minimalaufbau in `docs/photos/` ablegen
- [ ] finales Wiring-Log schreiben
- [ ] README mit Demo-Ablauf erweitern
- [ ] erste stabile Firmware-Version taggen

## Future

- [ ] Captive Portal
- [ ] Favoritenliste
- [ ] optionaler Tastermodus
- [ ] optionales LCD1602
- [ ] IR-Lernmodus
