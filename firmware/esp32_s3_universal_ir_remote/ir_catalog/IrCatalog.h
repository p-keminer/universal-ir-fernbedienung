#pragma once

#include <Arduino.h>

namespace ir_catalog {

enum class DeviceCategory : uint8_t {
  Tv,
  Media,
  LedStrip,
  GenericTest,
  Projector,
  Audio,
};

enum class CodeStatus : uint8_t {
  Untested,
  Confirmed,
  Rejected,
};

enum class IrProtocol : uint8_t {
  Samsung,
  Nec,
  Sony,
  Panasonic,
  Rc5,
  Jvc,
  SharpRaw,
  Epson,
};

enum class ProfileId : uint8_t {
  SamsungTv,
  LgTv,
  SonyTv,
  PanasonicTv,
  PhilipsTv,
  GenericNec,
  TclHisense,
  ToshibaTv,
  VizioTv,
  Jvc,
  Sharp,
  AppleTv,
  LedStrip,
  BenqProjector,
  EpsonProjector,
  YamahaReceiver,
};

struct IrProfile {
  ProfileId id;
  const char* name;
  DeviceCategory category;
};

struct IrCommand {
  ProfileId profile;
  const char* name;
  DeviceCategory category;
  IrProtocol protocol;
  CodeStatus status;
  uint64_t data;
  uint16_t address;
  uint16_t bits;
  bool diagnostic;
  const char* note;
};

constexpr uint8_t reverse8(const uint8_t value) {
  return ((value & 0x01) << 7) | ((value & 0x02) << 5) |
         ((value & 0x04) << 3) | ((value & 0x08) << 1) |
         ((value & 0x10) >> 1) | ((value & 0x20) >> 3) |
         ((value & 0x40) >> 5) | ((value & 0x80) >> 7);
}

constexpr uint16_t reverseBits(uint16_t value, const uint8_t bits) {
  uint16_t result = 0;

  for (uint8_t i = 0; i < bits; ++i) {
    result <<= 1;
    result |= value & 0x01;
    value >>= 1;
  }

  return result;
}

constexpr uint32_t necIrdb(const uint8_t device, const int16_t subdevice,
                           const uint8_t function) {
  const uint8_t dev = reverse8(device);
  const uint8_t sub = subdevice < 0 ? static_cast<uint8_t>(~dev)
                                    : reverse8(static_cast<uint8_t>(subdevice));
  const uint8_t fn = reverse8(function);

  return (static_cast<uint32_t>(dev) << 24) |
         (static_cast<uint32_t>(sub) << 16) |
         (static_cast<uint32_t>(fn) << 8) | static_cast<uint8_t>(~fn);
}

constexpr uint16_t sony12Irdb(const uint8_t device, const uint8_t function) {
  return reverseBits((static_cast<uint16_t>(device) << 7) | function, 12);
}

static constexpr IrProfile kProfiles[] = {
    {ProfileId::SamsungTv, "Samsung TV", DeviceCategory::Tv},
    {ProfileId::LgTv, "LG TV", DeviceCategory::Tv},
    {ProfileId::SonyTv, "Sony TV", DeviceCategory::Tv},
    {ProfileId::PanasonicTv, "Panasonic TV", DeviceCategory::Tv},
    {ProfileId::PhilipsTv, "Philips TV", DeviceCategory::Tv},
    {ProfileId::GenericNec, "Generic NEC", DeviceCategory::GenericTest},
    {ProfileId::TclHisense, "TCL/Hisense", DeviceCategory::Tv},
    {ProfileId::ToshibaTv, "Toshiba TV", DeviceCategory::Tv},
    {ProfileId::VizioTv, "Vizio TV", DeviceCategory::Tv},
    {ProfileId::Jvc, "JVC", DeviceCategory::Media},
    {ProfileId::Sharp, "Sharp", DeviceCategory::Tv},
    {ProfileId::AppleTv, "Apple TV", DeviceCategory::Media},
    {ProfileId::LedStrip, "LED Strip", DeviceCategory::LedStrip},
    {ProfileId::BenqProjector, "BenQ Projector", DeviceCategory::Projector},
    {ProfileId::EpsonProjector, "Epson Projector", DeviceCategory::Projector},
    {ProfileId::YamahaReceiver, "Yamaha Receiver", DeviceCategory::Audio},
};

static constexpr IrCommand kCommands[] = {
    {ProfileId::SamsungTv, "Power", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, 0xE0E040BF, 0, 32, true, "TV power toggle"},
    {ProfileId::SamsungTv, "Vol +", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, 0xE0E0E01F, 0, 32, false, "volume up"},
    {ProfileId::SamsungTv, "Vol -", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, 0xE0E0D02F, 0, 32, false, "volume down"},
    {ProfileId::SamsungTv, "Mute", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, 0xE0E0F00F, 0, 32, false, "mute toggle"},
    {ProfileId::SamsungTv, "Source", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, 0xE0E0807F, 0, 32, false, "input source"},
    {ProfileId::SamsungTv, "Ch +", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 18), 0, 32, false,
     "IRDB Samsung TV 7,7 f=18"},
    {ProfileId::SamsungTv, "Ch -", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 16), 0, 32, false,
     "IRDB Samsung TV 7,7 f=16"},
    {ProfileId::SamsungTv, "Menu", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 26), 0, 32, false,
     "IRDB Samsung TV 7,7 f=26"},
    {ProfileId::SamsungTv, "Info", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 31), 0, 32, false,
     "IRDB Samsung TV 7,7 f=31"},
    {ProfileId::SamsungTv, "Exit", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 45), 0, 32, false,
     "IRDB Samsung TV 7,7 f=45"},
    {ProfileId::SamsungTv, "Return", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 88), 0, 32, false,
     "IRDB Samsung TV 7,7 f=88"},
    {ProfileId::SamsungTv, "Up", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 96), 0, 32, false,
     "IRDB Samsung TV 7,7 f=96"},
    {ProfileId::SamsungTv, "Down", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 97), 0, 32, false,
     "IRDB Samsung TV 7,7 f=97"},
    {ProfileId::SamsungTv, "Right", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 98), 0, 32, false,
     "IRDB Samsung TV 7,7 f=98"},
    {ProfileId::SamsungTv, "Left", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 101), 0, 32, false,
     "IRDB Samsung TV 7,7 f=101"},
    {ProfileId::SamsungTv, "Enter", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 104), 0, 32, false,
     "IRDB Samsung TV 7,7 f=104"},
    {ProfileId::SamsungTv, "Smart Hub", DeviceCategory::Tv,
     IrProtocol::Samsung, CodeStatus::Untested, necIrdb(7, 7, 121), 0, 32,
     false, "IRDB Samsung TV 7,7 f=121"},
    {ProfileId::SamsungTv, "HDMI 1", DeviceCategory::Tv, IrProtocol::Samsung,
     CodeStatus::Untested, necIrdb(7, 7, 233), 0, 32, false,
     "IRDB Samsung TV 7,7 f=233"},

    {ProfileId::LgTv, "Power", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, 0x20DF10EF, 0, 32, true, "LG/NEC power toggle"},
    {ProfileId::LgTv, "Vol +", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, 0x20DF40BF, 0, 32, false, "volume up"},
    {ProfileId::LgTv, "Vol -", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, 0x20DFC03F, 0, 32, false, "volume down"},
    {ProfileId::LgTv, "Mute", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, 0x20DF906F, 0, 32, false, "mute toggle"},
    {ProfileId::LgTv, "Input", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 11), 0, 32, false,
     "IRDB LG TV 4,-1 f=11"},
    {ProfileId::LgTv, "Ch +", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 0), 0, 32, false,
     "IRDB LG TV 4,-1 f=0"},
    {ProfileId::LgTv, "Ch -", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 1), 0, 32, false,
     "IRDB LG TV 4,-1 f=1"},
    {ProfileId::LgTv, "Up", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 64), 0, 32, false,
     "IRDB LG TV 4,-1 f=64"},
    {ProfileId::LgTv, "Down", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 65), 0, 32, false,
     "IRDB LG TV 4,-1 f=65"},
    {ProfileId::LgTv, "Left", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 7), 0, 32, false,
     "IRDB LG TV 4,-1 f=7"},
    {ProfileId::LgTv, "Right", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 6), 0, 32, false,
     "IRDB LG TV 4,-1 f=6"},
    {ProfileId::LgTv, "OK", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 68), 0, 32, false,
     "IRDB LG TV 4,-1 f=68"},
    {ProfileId::LgTv, "Menu", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 67), 0, 32, false,
     "IRDB LG TV 4,-1 f=67"},
    {ProfileId::LgTv, "Exit", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 91), 0, 32, false,
     "IRDB LG TV 4,-1 f=91"},
    {ProfileId::LgTv, "HDMI 1", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 198), 0, 32, false,
     "IRDB LG TV 4,-1 f=198"},

    {ProfileId::SonyTv, "Power", DeviceCategory::Tv, IrProtocol::Sony,
     CodeStatus::Untested, 0xA90, 0, 12, true, "Sony SIRC power"},
    {ProfileId::SonyTv, "Vol +", DeviceCategory::Tv, IrProtocol::Sony,
     CodeStatus::Untested, sony12Irdb(1, 18), 0, 12, false,
     "IRDB Sony TV 1,-1 f=18"},
    {ProfileId::SonyTv, "Vol -", DeviceCategory::Tv, IrProtocol::Sony,
     CodeStatus::Untested, sony12Irdb(1, 19), 0, 12, false,
     "IRDB Sony TV 1,-1 f=19"},
    {ProfileId::SonyTv, "Mute", DeviceCategory::Tv, IrProtocol::Sony,
     CodeStatus::Untested, sony12Irdb(1, 20), 0, 12, false,
     "IRDB Sony TV 1,-1 f=20"},
    {ProfileId::SonyTv, "Ch +", DeviceCategory::Tv, IrProtocol::Sony,
     CodeStatus::Untested, sony12Irdb(1, 16), 0, 12, false,
     "IRDB Sony TV 1,-1 f=16"},
    {ProfileId::SonyTv, "Ch -", DeviceCategory::Tv, IrProtocol::Sony,
     CodeStatus::Untested, sony12Irdb(1, 17), 0, 12, false,
     "IRDB Sony TV 1,-1 f=17"},
    {ProfileId::SonyTv, "Input", DeviceCategory::Tv, IrProtocol::Sony,
     CodeStatus::Untested, sony12Irdb(1, 37), 0, 12, false,
     "IRDB Sony TV 1,-1 f=37"},
    {ProfileId::SonyTv, "Info", DeviceCategory::Tv, IrProtocol::Sony,
     CodeStatus::Untested, sony12Irdb(1, 58), 0, 12, false,
     "IRDB Sony TV 1,-1 f=58"},

    {ProfileId::PanasonicTv, "Power", DeviceCategory::Tv,
     IrProtocol::Panasonic, CodeStatus::Untested, 0x0100BCBD, 0x4004, 32, true,
     "Panasonic address/data power"},

    {ProfileId::PhilipsTv, "Power", DeviceCategory::Tv, IrProtocol::Rc5,
     CodeStatus::Untested, 0x0C, 0, 12, true, "Philips RC5 power"},

    {ProfileId::GenericNec, "Power A", DeviceCategory::GenericTest,
     IrProtocol::Nec, CodeStatus::Untested, 0x10EFC03F, 0, 32, true,
     "generic NEC fallback"},

    {ProfileId::TclHisense, "Power", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, 0x00FF00FF, 0, 32, true, "TCL/Hisense NEC test"},

    {ProfileId::ToshibaTv, "Power", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(64, -1, 18), 0, 32, true,
     "IRDB Toshiba TV 64,-1 f=18"},
    {ProfileId::ToshibaTv, "Input", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(64, -1, 15), 0, 32, false,
     "IRDB Toshiba TV 64,-1 f=15"},
    {ProfileId::ToshibaTv, "Mute", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(64, -1, 16), 0, 32, false,
     "IRDB Toshiba TV 64,-1 f=16"},
    {ProfileId::ToshibaTv, "Vol +", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(64, -1, 26), 0, 32, false,
     "IRDB Toshiba TV 64,-1 f=26"},
    {ProfileId::ToshibaTv, "Vol -", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(64, -1, 30), 0, 32, false,
     "IRDB Toshiba TV 64,-1 f=30"},
    {ProfileId::ToshibaTv, "Ch +", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(64, -1, 27), 0, 32, false,
     "IRDB Toshiba TV 64,-1 f=27"},
    {ProfileId::ToshibaTv, "Ch -", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(64, -1, 31), 0, 32, false,
     "IRDB Toshiba TV 64,-1 f=31"},
    {ProfileId::ToshibaTv, "Exit", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(64, -1, 88), 0, 32, false,
     "IRDB Toshiba TV 64,-1 f=88"},
    {ProfileId::ToshibaTv, "Menu/Enter", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(64, -1, 128), 0, 32, false,
     "IRDB Toshiba TV 64,-1 f=128"},

    {ProfileId::VizioTv, "Power", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 8), 0, 32, true,
     "IRDB Vizio Unknown_Vizio 4,-1 f=8"},
    {ProfileId::VizioTv, "Vol +", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 2), 0, 32, false,
     "IRDB Vizio Unknown_Vizio 4,-1 f=2"},
    {ProfileId::VizioTv, "Vol -", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 3), 0, 32, false,
     "IRDB Vizio Unknown_Vizio 4,-1 f=3"},
    {ProfileId::VizioTv, "Mute", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 9), 0, 32, false,
     "IRDB Vizio Unknown_Vizio 4,-1 f=9"},
    {ProfileId::VizioTv, "Input", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 47), 0, 32, false,
     "IRDB Vizio Unknown_Vizio 4,-1 f=47"},
    {ProfileId::VizioTv, "Menu", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 67), 0, 32, false,
     "IRDB Vizio Unknown_Vizio 4,-1 f=67"},
    {ProfileId::VizioTv, "Up", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 69), 0, 32, false,
     "IRDB Vizio Unknown_Vizio 4,-1 f=69"},
    {ProfileId::VizioTv, "Down", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 70), 0, 32, false,
     "IRDB Vizio Unknown_Vizio 4,-1 f=70"},
    {ProfileId::VizioTv, "Left", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 71), 0, 32, false,
     "IRDB Vizio Unknown_Vizio 4,-1 f=71"},
    {ProfileId::VizioTv, "Right", DeviceCategory::Tv, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(4, -1, 72), 0, 32, false,
     "IRDB Vizio Unknown_Vizio 4,-1 f=72"},

    {ProfileId::Jvc, "Power", DeviceCategory::Media, IrProtocol::Jvc,
     CodeStatus::Untested, 0xC2A4, 0, 16, true, "JVC test code"},

    {ProfileId::Sharp, "Power", DeviceCategory::Tv, IrProtocol::SharpRaw,
     CodeStatus::Untested, 0x42AD, 0, 15, true, "Sharp raw 15-bit test"},

    {ProfileId::AppleTv, "Play/Pause", DeviceCategory::Media, IrProtocol::Nec,
     CodeStatus::Untested, 0xEE8704FB, 0, 32, false, "Apple TV play/pause"},

    {ProfileId::LedStrip, "Power", DeviceCategory::LedStrip, IrProtocol::Nec,
     CodeStatus::Untested, 0xF7C03F, 0, 32, false, "LED strip power toggle"},
    {ProfileId::LedStrip, "Strobe", DeviceCategory::LedStrip, IrProtocol::Nec,
     CodeStatus::Untested, 0xF7E01F, 0, 32, false, "LED strip strobe/flash"},
    {ProfileId::LedStrip, "Bright +", DeviceCategory::LedStrip, IrProtocol::Nec,
     CodeStatus::Untested, 0xF700FF, 0, 32, false,
     "common 24/44-key LED remote"},
    {ProfileId::LedStrip, "Bright -", DeviceCategory::LedStrip, IrProtocol::Nec,
     CodeStatus::Untested, 0xF7807F, 0, 32, false,
     "common 24/44-key LED remote"},
    {ProfileId::LedStrip, "Red", DeviceCategory::LedStrip, IrProtocol::Nec,
     CodeStatus::Untested, 0xF720DF, 0, 32, false,
     "common 24/44-key LED remote"},
    {ProfileId::LedStrip, "Green", DeviceCategory::LedStrip, IrProtocol::Nec,
     CodeStatus::Untested, 0xF7A05F, 0, 32, false,
     "common 24/44-key LED remote"},
    {ProfileId::LedStrip, "Blue", DeviceCategory::LedStrip, IrProtocol::Nec,
     CodeStatus::Untested, 0xF7609F, 0, 32, false,
     "common 24/44-key LED remote"},
    {ProfileId::LedStrip, "White", DeviceCategory::LedStrip, IrProtocol::Nec,
     CodeStatus::Untested, 0xF7E01F, 0, 32, false,
     "common 24/44-key LED remote"},
    {ProfileId::LedStrip, "Flash", DeviceCategory::LedStrip, IrProtocol::Nec,
     CodeStatus::Untested, 0xF7D02F, 0, 32, false,
     "common 24/44-key LED remote"},
    {ProfileId::LedStrip, "Fade", DeviceCategory::LedStrip, IrProtocol::Nec,
     CodeStatus::Untested, 0xF7C837, 0, 32, false,
     "common 24/44-key LED remote"},

    {ProfileId::BenqProjector, "Power", DeviceCategory::Projector,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(48, -1, 24), 0, 32, true,
     "IRDB BenQ Projector 48,-1 f=24"},
    {ProfileId::BenqProjector, "Source", DeviceCategory::Projector,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(48, -1, 8), 0, 32, false,
     "IRDB BenQ Projector 48,-1 f=8"},
    {ProfileId::BenqProjector, "Menu", DeviceCategory::Projector,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(48, -1, 12), 0, 32, false,
     "IRDB BenQ Projector 48,-1 f=12"},
    {ProfileId::BenqProjector, "Up", DeviceCategory::Projector,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(48, -1, 10), 0, 32, false,
     "IRDB BenQ Projector 48,-1 f=10"},
    {ProfileId::BenqProjector, "Down", DeviceCategory::Projector,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(48, -1, 32), 0, 32, false,
     "IRDB BenQ Projector 48,-1 f=32"},
    {ProfileId::BenqProjector, "Left", DeviceCategory::Projector,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(48, -1, 0), 0, 32, false,
     "IRDB BenQ Projector 48,-1 f=0"},
    {ProfileId::BenqProjector, "Right", DeviceCategory::Projector,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(48, -1, 2), 0, 32, false,
     "IRDB BenQ Projector 48,-1 f=2"},
    {ProfileId::BenqProjector, "Mute", DeviceCategory::Projector,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(48, -1, 18), 0, 32, false,
     "IRDB BenQ Projector 48,-1 f=18"},

    {ProfileId::EpsonProjector, "Power", DeviceCategory::Projector,
     IrProtocol::Epson, CodeStatus::Untested, necIrdb(131, 85, 144), 0, 32,
     true, "IRDB Epson Projector 131,85 f=144"},
    {ProfileId::EpsonProjector, "Off", DeviceCategory::Projector,
     IrProtocol::Epson, CodeStatus::Untested, necIrdb(131, 85, 145), 0, 32,
     false, "IRDB Epson Projector 131,85 f=145"},
    {ProfileId::EpsonProjector, "HDMI 1", DeviceCategory::Projector,
     IrProtocol::Epson, CodeStatus::Untested, necIrdb(131, 85, 115), 0, 32,
     false, "IRDB Epson Projector 131,85 f=115"},
    {ProfileId::EpsonProjector, "HDMI 2", DeviceCategory::Projector,
     IrProtocol::Epson, CodeStatus::Untested, necIrdb(131, 85, 119), 0, 32,
     false, "IRDB Epson Projector 131,85 f=119"},
    {ProfileId::EpsonProjector, "Menu", DeviceCategory::Projector,
     IrProtocol::Epson, CodeStatus::Untested, necIrdb(131, 85, 154), 0, 32,
     false, "IRDB Epson Projector 131,85 f=154"},
    {ProfileId::EpsonProjector, "Enter", DeviceCategory::Projector,
     IrProtocol::Epson, CodeStatus::Untested, necIrdb(131, 85, 133), 0, 32,
     false, "IRDB Epson Projector 131,85 f=133"},
    {ProfileId::EpsonProjector, "Up", DeviceCategory::Projector,
     IrProtocol::Epson, CodeStatus::Untested, necIrdb(131, 85, 176), 0, 32,
     false, "IRDB Epson Projector 131,85 f=176"},
    {ProfileId::EpsonProjector, "Down", DeviceCategory::Projector,
     IrProtocol::Epson, CodeStatus::Untested, necIrdb(131, 85, 178), 0, 32,
     false, "IRDB Epson Projector 131,85 f=178"},
    {ProfileId::EpsonProjector, "Left", DeviceCategory::Projector,
     IrProtocol::Epson, CodeStatus::Untested, necIrdb(131, 85, 179), 0, 32,
     false, "IRDB Epson Projector 131,85 f=179"},
    {ProfileId::EpsonProjector, "Right", DeviceCategory::Projector,
     IrProtocol::Epson, CodeStatus::Untested, necIrdb(131, 85, 177), 0, 32,
     false, "IRDB Epson Projector 131,85 f=177"},

    {ProfileId::YamahaReceiver, "Power On", DeviceCategory::Audio,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(122, -1, 29), 0, 32, false,
     "IRDB Yamaha Receiver 122,-1 f=29"},
    {ProfileId::YamahaReceiver, "Power Off", DeviceCategory::Audio,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(122, -1, 30), 0, 32, false,
     "IRDB Yamaha Receiver 122,-1 f=30"},
    {ProfileId::YamahaReceiver, "Power Toggle", DeviceCategory::Audio,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(122, -1, 31), 0, 32, true,
     "IRDB Yamaha Receiver 122,-1 f=31"},
    {ProfileId::YamahaReceiver, "Vol +", DeviceCategory::Audio,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(122, -1, 26), 0, 32, false,
     "IRDB Yamaha Receiver 122,-1 f=26"},
    {ProfileId::YamahaReceiver, "Vol -", DeviceCategory::Audio,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(122, -1, 27), 0, 32, false,
     "IRDB Yamaha Receiver 122,-1 f=27"},
    {ProfileId::YamahaReceiver, "Mute", DeviceCategory::Audio, IrProtocol::Nec,
     CodeStatus::Untested, necIrdb(122, -1, 28), 0, 32, false,
     "IRDB Yamaha Receiver 122,-1 f=28"},
    {ProfileId::YamahaReceiver, "Input CD", DeviceCategory::Audio,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(122, -1, 21), 0, 32, false,
     "IRDB Yamaha Receiver 122,-1 f=21"},
    {ProfileId::YamahaReceiver, "Input Tuner", DeviceCategory::Audio,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(122, -1, 22), 0, 32, false,
     "IRDB Yamaha Receiver 122,-1 f=22"},
    {ProfileId::YamahaReceiver, "Input DVD", DeviceCategory::Audio,
     IrProtocol::Nec, CodeStatus::Untested, necIrdb(122, -1, 23), 0, 32, false,
     "IRDB Yamaha Receiver 122,-1 f=23"},
};

inline uint8_t profileCount() {
  return sizeof(kProfiles) / sizeof(kProfiles[0]);
}

inline const IrProfile& profileAt(const uint8_t index) {
  return kProfiles[index % profileCount()];
}

inline uint8_t commandCount() {
  return sizeof(kCommands) / sizeof(kCommands[0]);
}

inline uint8_t commandCountForProfile(const ProfileId profile) {
  uint8_t count = 0;

  for (uint8_t i = 0; i < commandCount(); ++i) {
    if (kCommands[i].profile == profile) {
      ++count;
    }
  }

  return count;
}

inline const IrCommand* commandAt(const ProfileId profile, uint8_t commandIndex) {
  for (uint8_t i = 0; i < commandCount(); ++i) {
    if (kCommands[i].profile == profile) {
      if (commandIndex == 0) {
        return &kCommands[i];
      }
      --commandIndex;
    }
  }

  return nullptr;
}

inline const __FlashStringHelper* protocolName(const IrProtocol protocol) {
  switch (protocol) {
    case IrProtocol::Samsung:
      return F("SAMSUNG");
    case IrProtocol::Nec:
      return F("NEC");
    case IrProtocol::Sony:
      return F("SONY");
    case IrProtocol::Panasonic:
      return F("PANASONIC");
    case IrProtocol::Rc5:
      return F("RC5");
    case IrProtocol::Jvc:
      return F("JVC");
    case IrProtocol::SharpRaw:
      return F("SHARP_RAW");
    case IrProtocol::Epson:
      return F("EPSON");
    default:
      return F("UNKNOWN");
  }
}

}  // namespace ir_catalog
