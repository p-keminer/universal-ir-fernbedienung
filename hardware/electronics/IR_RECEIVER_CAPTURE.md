# IR Receiver Capture

## Einordnung

Der IR-Empfaenger ist kein Teil der normalen Dashboard-Fernbedienung.
Er wird nur temporaer genutzt, um echte Codes von Originalfernbedienungen einzufangen.

## Geeignete Module

- `KY-022`
- `VS1838B`
- andere 38-kHz-IR-Receiver-Module

Diese Module demodulieren das 38-kHz-IR-Signal und geben ein digitales Signal aus.

## Default-Pin fuer Capture

| Signal | ESP32-S3 Pin | Status |
| --- | --- | --- |
| IR_RECV | `GPIO15` | Capture-Default |
| VCC | `3V3` | empfohlen fuer ESP32-S3 |
| GND | `GND` | gemeinsame Masse |

## Verdrahtung

```text
ESP32-S3                 IR-Receiver-Modul

3V3   -----------------> VCC / +
GND   -----------------> GND / -
GPIO15 <---------------- S / OUT
```

Kurzform:

```text
S/OUT -> GPIO15
VCC   -> 3V3
GND   -> GND
```

## Hinweise

- Pinout am konkreten Modul pruefen; Reihenfolge kann je nach Board abweichen.
- Empfaenger nicht an `5 V` betreiben, solange sein Ausgang direkt zum ESP32-S3 geht.
- Sender-KY-005 und Receiver nicht direkt voreinander legen, sonst koennen Reflexionen oder Uebersteuerung stoeren.
- Originalfernbedienung zum Capturen zuerst mit 10-50 cm Abstand testen.

## Capture-Regel

Ein Code gilt erst als guter Kandidat, wenn mindestens drei kurze Tastendruecke denselben Protokoll-/Codewert oder sehr aehnliche Raw-Daten liefern.
