# Agent Notes

Dieses Repo ist ein dokumentationsgetriebenes Embedded-Projekt.

## Arbeitsregeln

- Bestehende Doku-Struktur respektieren.
- Keine lokalen Secrets, absoluten Nutzerpfade oder privaten Geraetedaten einchecken.
- GPIOs erst nach realem Boardcheck als bestaetigt markieren.
- Hardware-Entscheidungen in `hardware/` dokumentieren, nicht nur im Sketch.
- Firmware-Aenderungen kurz in `firmware/README.md` oder im jeweiligen Sketch-README spiegeln.
- Automatische IR-Testschleifen klar als Testmodus kennzeichnen.

## Stil

- Deutsch als Projektsprache.
- ASCII-Schreibweise fuer Umlaute in neuen Dateien verwenden.
- Kurze, konkrete Markdown-Dokumente bevorzugen.
- Keine grossen Zukunftsideen in den ersten Prototyp ziehen; dafuer `future/` nutzen.
