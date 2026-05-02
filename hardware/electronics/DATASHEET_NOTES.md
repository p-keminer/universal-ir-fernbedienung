# Datasheet Notes

Dieses Dokument sammelt Quellen und Grenznotizen.
PDFs werden nicht eingecheckt; Links und relevante Entscheidungen reichen fuer den Projektstand.

## ESP32-S3

- Quelle: Espressif ESP32-S3 Series Datasheet
- Link: https://documentation.espressif.com/esp32-s3_datasheet_en.pdf
- Relevanz: GPIO-Funktionen, elektrische Grenzwerte, Boot-/USB-Pins, 3,3-V-Logik.
- Projektregel: Arduino-ESP32 `esp32:esp32:esp32s3` Defaults sind Startwerte, das konkrete Board-Pinout bleibt massgeblich.

## ESP32-S3-DevKitC-1

- Quelle: Espressif ESP32-S3-DevKitC-1 v1.1 User Guide
- Link: https://documentation.espressif.com/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html
- Relevanz: Header J1/J3, erreichbare GPIOs, Versorgungspins, Boardrevisionen.
- Projektregel: Fuer Step 01 werden nur `GPIO4`, `3V3` und `GND` aktiv genutzt. Strapping-, USB-, UART- und interne Flash/PSRAM-Pins bleiben frei.

## P2N2222A

- Quelle: onsemi P2N2222A Datasheet, z.B. ueber DigiKey HTML-Datenblatt
- Link: https://www.digikey.com/en/htmldatasheets/production/101607/0/0/1/p2n2222a.html
- Relevanz: NPN-Transistor, TO-92 Pinout und Strom-/Leistungsgrenzen.
- Projektregel: Pinout am realen Bauteil pruefen; P2N2222A/P2N2222/PN2222 koennen in Bastelsets verwirrend beschriftet sein.

## S8050

- Quelle: UTC S8050 NPN Epitaxial Silicon Transistor Datasheet
- Link: https://www.electronicscomp.com/datasheet/s8050-transistor-datasheet.pdf
- Relevanz: Alternative zum P2N2222 fuer Low-Side-Schalter.
- Projektregel: Hersteller und Pinout am konkreten Bauteil pruefen; nicht blind aus Symbolbibliotheken uebernehmen.

## KY-005 IR Transmitter

- Quelle: Joy-IT KY-005 Datenblatt
- Link: https://uelectronics.com/wp-content/uploads/2017/06/KY-005-Joy-IT.pdf
- Relevanz: IR-LED-Modul, 940-nm-Emitter, Widerstands-/Versorgungsvarianten.
- Projektregel: Modulaufbau messen; nicht annehmen, dass jedes KY-005 Board denselben Vorwiderstand besitzt.

## HD44780 / LCD1602

- Quelle: Hitachi HD44780U Datasheet
- Link: https://www.alldatasheet.com/datasheet-pdf/pdf/63663/HITACHI/HD44780U.html
- Relevanz: 16x2-Zeichen-LCD, Zeichenspeicher, Timing, Kontrast.
- Projektregel: LCD1602 ist future/optional; Dashboard-v1 nutzt kein LCD.

## PCF8574 / I2C Backpack

- Quelle: NXP PCF8574/PCF8574A Datasheet
- Link: https://www.nxp.com/docs/en/data-sheet/PCF8574_PCF8574A.pdf
- Relevanz: I2C-Portexpander auf vielen LCD1602-Backpacks.
- Projektregel: Nur pruefen, falls der optionale LCD-Pfad spaeter wieder aktiviert wird.
