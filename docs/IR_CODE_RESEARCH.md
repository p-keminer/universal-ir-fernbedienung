# IR Code Research

## Grundsatz

Internet- und Datenbankquellen liefern nur Kandidaten.
Ein Code wird erst dann `confirmed`, wenn er am echten Zielgeraet mit Aufbau, Abstand und Ergebnis protokolliert wurde.

## Aktueller Firmware-Katalog

Der aktive Katalog steht in:

```text
firmware/esp32_s3_universal_ir_remote/ir_catalog/IrCatalog.h
```

Aktueller Stand: `16` Profile und `141` Befehle.

Profile:

- Samsung TV
- LG TV
- Sony TV
- Panasonic TV
- Philips TV
- Generic NEC
- TCL/Hisense
- Toshiba TV
- Vizio TV
- JVC
- Sharp
- Apple TV
- LED Strip
- BenQ Projector
- Epson Projector
- Yamaha Receiver

Alle Codes sind bewusst als `Untested` markiert.
Ein Code wird erst nach realem Test auf `Confirmed` oder `Rejected` gesetzt.

## Quellen

- Ausgangssketch des Projekts: erste Samsung/LG/Sony/Panasonic/Philips/NEC/JVC/Sharp/Apple/LED-Testcodes
- IRremoteESP8266 Repository und API: https://github.com/crankyoldgit/IRremoteESP8266
- IRremoteESP8266 Supported Protocols: https://github.com/crankyoldgit/IRremoteESP8266/blob/master/SupportedProtocols.md
- IRDB als Kandidatenquelle: https://github.com/probonopd/irdb
- LIRC Remote Database als Kandidatenquelle: https://lirc-remotes.sourceforge.net/remotes-table.html
- Eigene Captures mit `firmware/ir_code_capture/` gelten als hoechste Prioritaet.

## IRDB-Hinweis

Ein Teil der neuen Kandidaten basiert auf ausgewaehlten IRDB-Zeilen.
IRDB speichert Codes als `protocol, device, subdevice, function`.
Die Firmware wandelt diese Werte fuer NEC-/Samsung-, Sony12-, Panasonic-, RC5- und Sharp-basierte Befehle in die von `IRremoteESP8266` erwartete Sendedarstellung um.

## Nachgetragene TV-Kandidaten

| Profil | Quelle | Status |
| --- | --- | --- |
| Panasonic TV | IRDB `Panasonic/TV/128,0.csv`, `128,4.csv`, `128,9.csv` | Kandidaten eingetragen |
| Philips TV | IRDB `Philips/TV/0,-1.csv` | Kandidaten eingetragen |
| Sharp TV | IRDB `Sharp/TV/1,-1.csv` | Kandidaten eingetragen |
| TCL/Hisense | IRDB `TCL/TV/15,-1.csv` | recherchiert, aber `RCA-38` wird noch nicht vom Sender unterstuetzt |

TCL/Hisense ist deshalb bewusst noch nicht erweitert. IRDB listet fuer TCL viele passende Tasten, aber als Protokoll `RCA-38`. Die aktuell eingebundene Bibliothek `IRremoteESP8266` 2.9.0 stellt in dieser Installation keinen direkten `sendRCA`/`sendRCA38`-Sender bereit. Dafuer brauchen wir entweder eine eigene RCA-38-Sendekapselung mit `sendGeneric` oder eine zusaetzliche Bibliothek/Implementierung.

Hinweistext gemaess IRDB-Lizenz:

```text
Contains/accesses irdb by Simon Peter and contributors, used under permission.
For licensing details and for information on how to contribute to the database, see
https://github.com/probonopd/irdb
```

Fuer dieses Bastelprojekt gilt trotzdem: IRDB liefert nur Kandidaten, kein Funktionsversprechen.

## Eigene Captures

Eigene Captures sind fuer reale Geraete die beste Quelle.
Sie werden mit einem 38-kHz-IR-Empfaenger wie `KY-022` oder `VS1838B` aufgezeichnet.

Default:

```text
IR-Receiver S/OUT -> GPIO15
IR-Receiver VCC   -> 3V3
IR-Receiver GND   -> GND
```

Pro Taste werden mindestens drei kurze Tastendruecke verglichen.
Nur stabile Ergebnisse werden in den aktiven Katalog uebernommen.

## Sortierregeln

- TV-Profile zuerst nach Hersteller.
- LED-Strip getrennt von TV-Codes.
- Generic NEC nur fuer Fallbacks und Diagnosetests.
- Pro Profil nur wenige wichtige Funktionen in v1: Power, Lautstaerke, Mute, Source, Play/Pause.
- Neue Codes bekommen erst `Untested`, nie direkt `Confirmed`.

## Aktuelle Geraetegruppen

| Gruppe | Profile |
| --- | --- |
| TV | Samsung, LG, Sony, Panasonic, Philips, TCL/Hisense, Toshiba, Vizio, Sharp |
| Media | JVC, Apple TV |
| LED | LED Strip |
| Projektor | BenQ, Epson |
| Audio | Yamaha Receiver |
| Test | Generic NEC |
