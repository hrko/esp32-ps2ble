
#include "key_translate.hpp"

#include <fmt/core.h>

#include <mapbox/eternal.hpp>


// ScanCode methods
constexpr ScanCode::ScanCode() : code() {}
constexpr ScanCode::ScanCode(std::initializer_list<std::uint8_t> list) : code(list) {}

std::string ScanCode::toString() const {
  std::string str;
  for (auto byte : code) {
    if (str.empty()) {
      str += fmt::format("{:02X}", byte);
    } else {
      str += fmt::format(" {:02X}", byte);
    }
  }

  return str;
}
const std::initializer_list<std::uint8_t>* ScanCode::getCode() const { return &code; }


// ScanCodePair methods
constexpr ScanCodePair::ScanCodePair() : makeCode(nullptr), breakCode(nullptr) {}
constexpr ScanCodePair::ScanCodePair(const ScanCode* makeCode, const ScanCode* breakCode) : makeCode(makeCode), breakCode(breakCode) {}

const ScanCode* ScanCodePair::getMake() const { return makeCode; }
const ScanCode* ScanCodePair::getBreak() const { return breakCode; }


// GENERATED CODE START
// clang-format off
constexpr auto SC_NONE = ScanCode();
constexpr auto SC_UNASSIGNED = ScanCode();
// Usage Page: 01, PS/2 Set 1
constexpr auto PAGE_01_ID_81_SC1_MK = ScanCode{0xE0, 0x5E};
constexpr auto PAGE_01_ID_81_SC1_BRK = ScanCode{0xE0, 0xDE};
constexpr auto PAGE_01_ID_81_SC1_PAIR = ScanCodePair{&PAGE_01_ID_81_SC1_MK, &PAGE_01_ID_81_SC1_BRK};
constexpr auto PAGE_01_ID_82_SC1_MK = ScanCode{0xE0, 0x5F};
constexpr auto PAGE_01_ID_82_SC1_BRK = ScanCode{0xE0, 0xDF};
constexpr auto PAGE_01_ID_82_SC1_PAIR = ScanCodePair{&PAGE_01_ID_82_SC1_MK, &PAGE_01_ID_82_SC1_BRK};
constexpr auto PAGE_01_ID_83_SC1_MK = ScanCode{0xE0, 0x63};
constexpr auto PAGE_01_ID_83_SC1_BRK = ScanCode{0xE0, 0xE3};
constexpr auto PAGE_01_ID_83_SC1_PAIR = ScanCodePair{&PAGE_01_ID_83_SC1_MK, &PAGE_01_ID_83_SC1_BRK};
constexpr auto PAGE_01_SC1_MAP = mapbox::eternal::map<UsageID, const ScanCodePair*>({
  {0x81, &PAGE_01_ID_81_SC1_PAIR},
  {0x82, &PAGE_01_ID_82_SC1_PAIR},
  {0x83, &PAGE_01_ID_83_SC1_PAIR},
});
// Usage Page: 01, PS/2 Set 2
constexpr auto PAGE_01_ID_81_SC2_MK = ScanCode{0xE0, 0x37};
constexpr auto PAGE_01_ID_81_SC2_BRK = ScanCode{0xE0, 0xF0, 0x37};
constexpr auto PAGE_01_ID_81_SC2_PAIR = ScanCodePair{&PAGE_01_ID_81_SC2_MK, &PAGE_01_ID_81_SC2_BRK};
constexpr auto PAGE_01_ID_82_SC2_MK = ScanCode{0xE0, 0x3F};
constexpr auto PAGE_01_ID_82_SC2_BRK = ScanCode{0xE0, 0xF0, 0x3F};
constexpr auto PAGE_01_ID_82_SC2_PAIR = ScanCodePair{&PAGE_01_ID_82_SC2_MK, &PAGE_01_ID_82_SC2_BRK};
constexpr auto PAGE_01_ID_83_SC2_MK = ScanCode{0xE0, 0x5E};
constexpr auto PAGE_01_ID_83_SC2_BRK = ScanCode{0xE0, 0xF0, 0x5E};
constexpr auto PAGE_01_ID_83_SC2_PAIR = ScanCodePair{&PAGE_01_ID_83_SC2_MK, &PAGE_01_ID_83_SC2_BRK};
constexpr auto PAGE_01_SC2_MAP = mapbox::eternal::map<UsageID, const ScanCodePair*>({
  {0x81, &PAGE_01_ID_81_SC2_PAIR},
  {0x82, &PAGE_01_ID_82_SC2_PAIR},
  {0x83, &PAGE_01_ID_83_SC2_PAIR},
});
// Usage Page: 07, PS/2 Set 1
constexpr auto PAGE_07_ID_00_SC1_PAIR = ScanCodePair{&SC_NONE, &SC_NONE};
constexpr auto PAGE_07_ID_01_SC1_MK = ScanCode{0xFF};
constexpr auto PAGE_07_ID_01_SC1_PAIR = ScanCodePair{&PAGE_07_ID_01_SC1_MK, &SC_NONE};
constexpr auto PAGE_07_ID_02_SC1_MK = ScanCode{0xFC};
constexpr auto PAGE_07_ID_02_SC1_PAIR = ScanCodePair{&PAGE_07_ID_02_SC1_MK, &SC_NONE};
constexpr auto PAGE_07_ID_03_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_04_SC1_MK = ScanCode{0x1E};
constexpr auto PAGE_07_ID_04_SC1_BRK = ScanCode{0x9E};
constexpr auto PAGE_07_ID_04_SC1_PAIR = ScanCodePair{&PAGE_07_ID_04_SC1_MK, &PAGE_07_ID_04_SC1_BRK};
constexpr auto PAGE_07_ID_05_SC1_MK = ScanCode{0x30};
constexpr auto PAGE_07_ID_05_SC1_BRK = ScanCode{0xB0};
constexpr auto PAGE_07_ID_05_SC1_PAIR = ScanCodePair{&PAGE_07_ID_05_SC1_MK, &PAGE_07_ID_05_SC1_BRK};
constexpr auto PAGE_07_ID_06_SC1_MK = ScanCode{0x2E};
constexpr auto PAGE_07_ID_06_SC1_BRK = ScanCode{0xAE};
constexpr auto PAGE_07_ID_06_SC1_PAIR = ScanCodePair{&PAGE_07_ID_06_SC1_MK, &PAGE_07_ID_06_SC1_BRK};
constexpr auto PAGE_07_ID_07_SC1_MK = ScanCode{0x20};
constexpr auto PAGE_07_ID_07_SC1_BRK = ScanCode{0xA0};
constexpr auto PAGE_07_ID_07_SC1_PAIR = ScanCodePair{&PAGE_07_ID_07_SC1_MK, &PAGE_07_ID_07_SC1_BRK};
constexpr auto PAGE_07_ID_08_SC1_MK = ScanCode{0x12};
constexpr auto PAGE_07_ID_08_SC1_BRK = ScanCode{0x92};
constexpr auto PAGE_07_ID_08_SC1_PAIR = ScanCodePair{&PAGE_07_ID_08_SC1_MK, &PAGE_07_ID_08_SC1_BRK};
constexpr auto PAGE_07_ID_09_SC1_MK = ScanCode{0x21};
constexpr auto PAGE_07_ID_09_SC1_BRK = ScanCode{0xA1};
constexpr auto PAGE_07_ID_09_SC1_PAIR = ScanCodePair{&PAGE_07_ID_09_SC1_MK, &PAGE_07_ID_09_SC1_BRK};
constexpr auto PAGE_07_ID_0A_SC1_MK = ScanCode{0x22};
constexpr auto PAGE_07_ID_0A_SC1_BRK = ScanCode{0xA2};
constexpr auto PAGE_07_ID_0A_SC1_PAIR = ScanCodePair{&PAGE_07_ID_0A_SC1_MK, &PAGE_07_ID_0A_SC1_BRK};
constexpr auto PAGE_07_ID_0B_SC1_MK = ScanCode{0x23};
constexpr auto PAGE_07_ID_0B_SC1_BRK = ScanCode{0xA3};
constexpr auto PAGE_07_ID_0B_SC1_PAIR = ScanCodePair{&PAGE_07_ID_0B_SC1_MK, &PAGE_07_ID_0B_SC1_BRK};
constexpr auto PAGE_07_ID_0C_SC1_MK = ScanCode{0x17};
constexpr auto PAGE_07_ID_0C_SC1_BRK = ScanCode{0x97};
constexpr auto PAGE_07_ID_0C_SC1_PAIR = ScanCodePair{&PAGE_07_ID_0C_SC1_MK, &PAGE_07_ID_0C_SC1_BRK};
constexpr auto PAGE_07_ID_0D_SC1_MK = ScanCode{0x24};
constexpr auto PAGE_07_ID_0D_SC1_BRK = ScanCode{0xA4};
constexpr auto PAGE_07_ID_0D_SC1_PAIR = ScanCodePair{&PAGE_07_ID_0D_SC1_MK, &PAGE_07_ID_0D_SC1_BRK};
constexpr auto PAGE_07_ID_0E_SC1_MK = ScanCode{0x25};
constexpr auto PAGE_07_ID_0E_SC1_BRK = ScanCode{0xA5};
constexpr auto PAGE_07_ID_0E_SC1_PAIR = ScanCodePair{&PAGE_07_ID_0E_SC1_MK, &PAGE_07_ID_0E_SC1_BRK};
constexpr auto PAGE_07_ID_0F_SC1_MK = ScanCode{0x26};
constexpr auto PAGE_07_ID_0F_SC1_BRK = ScanCode{0xA6};
constexpr auto PAGE_07_ID_0F_SC1_PAIR = ScanCodePair{&PAGE_07_ID_0F_SC1_MK, &PAGE_07_ID_0F_SC1_BRK};
constexpr auto PAGE_07_ID_10_SC1_MK = ScanCode{0x32};
constexpr auto PAGE_07_ID_10_SC1_BRK = ScanCode{0xB2};
constexpr auto PAGE_07_ID_10_SC1_PAIR = ScanCodePair{&PAGE_07_ID_10_SC1_MK, &PAGE_07_ID_10_SC1_BRK};
constexpr auto PAGE_07_ID_11_SC1_MK = ScanCode{0x31};
constexpr auto PAGE_07_ID_11_SC1_BRK = ScanCode{0xB1};
constexpr auto PAGE_07_ID_11_SC1_PAIR = ScanCodePair{&PAGE_07_ID_11_SC1_MK, &PAGE_07_ID_11_SC1_BRK};
constexpr auto PAGE_07_ID_12_SC1_MK = ScanCode{0x18};
constexpr auto PAGE_07_ID_12_SC1_BRK = ScanCode{0x98};
constexpr auto PAGE_07_ID_12_SC1_PAIR = ScanCodePair{&PAGE_07_ID_12_SC1_MK, &PAGE_07_ID_12_SC1_BRK};
constexpr auto PAGE_07_ID_13_SC1_MK = ScanCode{0x19};
constexpr auto PAGE_07_ID_13_SC1_BRK = ScanCode{0x99};
constexpr auto PAGE_07_ID_13_SC1_PAIR = ScanCodePair{&PAGE_07_ID_13_SC1_MK, &PAGE_07_ID_13_SC1_BRK};
constexpr auto PAGE_07_ID_14_SC1_MK = ScanCode{0x10};
constexpr auto PAGE_07_ID_14_SC1_BRK = ScanCode{0x90};
constexpr auto PAGE_07_ID_14_SC1_PAIR = ScanCodePair{&PAGE_07_ID_14_SC1_MK, &PAGE_07_ID_14_SC1_BRK};
constexpr auto PAGE_07_ID_15_SC1_MK = ScanCode{0x13};
constexpr auto PAGE_07_ID_15_SC1_BRK = ScanCode{0x93};
constexpr auto PAGE_07_ID_15_SC1_PAIR = ScanCodePair{&PAGE_07_ID_15_SC1_MK, &PAGE_07_ID_15_SC1_BRK};
constexpr auto PAGE_07_ID_16_SC1_MK = ScanCode{0x1F};
constexpr auto PAGE_07_ID_16_SC1_BRK = ScanCode{0x9F};
constexpr auto PAGE_07_ID_16_SC1_PAIR = ScanCodePair{&PAGE_07_ID_16_SC1_MK, &PAGE_07_ID_16_SC1_BRK};
constexpr auto PAGE_07_ID_17_SC1_MK = ScanCode{0x14};
constexpr auto PAGE_07_ID_17_SC1_BRK = ScanCode{0x94};
constexpr auto PAGE_07_ID_17_SC1_PAIR = ScanCodePair{&PAGE_07_ID_17_SC1_MK, &PAGE_07_ID_17_SC1_BRK};
constexpr auto PAGE_07_ID_18_SC1_MK = ScanCode{0x16};
constexpr auto PAGE_07_ID_18_SC1_BRK = ScanCode{0x96};
constexpr auto PAGE_07_ID_18_SC1_PAIR = ScanCodePair{&PAGE_07_ID_18_SC1_MK, &PAGE_07_ID_18_SC1_BRK};
constexpr auto PAGE_07_ID_19_SC1_MK = ScanCode{0x2F};
constexpr auto PAGE_07_ID_19_SC1_BRK = ScanCode{0xAF};
constexpr auto PAGE_07_ID_19_SC1_PAIR = ScanCodePair{&PAGE_07_ID_19_SC1_MK, &PAGE_07_ID_19_SC1_BRK};
constexpr auto PAGE_07_ID_1A_SC1_MK = ScanCode{0x11};
constexpr auto PAGE_07_ID_1A_SC1_BRK = ScanCode{0x91};
constexpr auto PAGE_07_ID_1A_SC1_PAIR = ScanCodePair{&PAGE_07_ID_1A_SC1_MK, &PAGE_07_ID_1A_SC1_BRK};
constexpr auto PAGE_07_ID_1B_SC1_MK = ScanCode{0x2D};
constexpr auto PAGE_07_ID_1B_SC1_BRK = ScanCode{0xAD};
constexpr auto PAGE_07_ID_1B_SC1_PAIR = ScanCodePair{&PAGE_07_ID_1B_SC1_MK, &PAGE_07_ID_1B_SC1_BRK};
constexpr auto PAGE_07_ID_1C_SC1_MK = ScanCode{0x15};
constexpr auto PAGE_07_ID_1C_SC1_BRK = ScanCode{0x95};
constexpr auto PAGE_07_ID_1C_SC1_PAIR = ScanCodePair{&PAGE_07_ID_1C_SC1_MK, &PAGE_07_ID_1C_SC1_BRK};
constexpr auto PAGE_07_ID_1D_SC1_MK = ScanCode{0x2C};
constexpr auto PAGE_07_ID_1D_SC1_BRK = ScanCode{0xAC};
constexpr auto PAGE_07_ID_1D_SC1_PAIR = ScanCodePair{&PAGE_07_ID_1D_SC1_MK, &PAGE_07_ID_1D_SC1_BRK};
constexpr auto PAGE_07_ID_1E_SC1_MK = ScanCode{0x02};
constexpr auto PAGE_07_ID_1E_SC1_BRK = ScanCode{0x82};
constexpr auto PAGE_07_ID_1E_SC1_PAIR = ScanCodePair{&PAGE_07_ID_1E_SC1_MK, &PAGE_07_ID_1E_SC1_BRK};
constexpr auto PAGE_07_ID_1F_SC1_MK = ScanCode{0x03};
constexpr auto PAGE_07_ID_1F_SC1_BRK = ScanCode{0x83};
constexpr auto PAGE_07_ID_1F_SC1_PAIR = ScanCodePair{&PAGE_07_ID_1F_SC1_MK, &PAGE_07_ID_1F_SC1_BRK};
constexpr auto PAGE_07_ID_20_SC1_MK = ScanCode{0x04};
constexpr auto PAGE_07_ID_20_SC1_BRK = ScanCode{0x84};
constexpr auto PAGE_07_ID_20_SC1_PAIR = ScanCodePair{&PAGE_07_ID_20_SC1_MK, &PAGE_07_ID_20_SC1_BRK};
constexpr auto PAGE_07_ID_21_SC1_MK = ScanCode{0x05};
constexpr auto PAGE_07_ID_21_SC1_BRK = ScanCode{0x85};
constexpr auto PAGE_07_ID_21_SC1_PAIR = ScanCodePair{&PAGE_07_ID_21_SC1_MK, &PAGE_07_ID_21_SC1_BRK};
constexpr auto PAGE_07_ID_22_SC1_MK = ScanCode{0x06};
constexpr auto PAGE_07_ID_22_SC1_BRK = ScanCode{0x86};
constexpr auto PAGE_07_ID_22_SC1_PAIR = ScanCodePair{&PAGE_07_ID_22_SC1_MK, &PAGE_07_ID_22_SC1_BRK};
constexpr auto PAGE_07_ID_23_SC1_MK = ScanCode{0x07};
constexpr auto PAGE_07_ID_23_SC1_BRK = ScanCode{0x87};
constexpr auto PAGE_07_ID_23_SC1_PAIR = ScanCodePair{&PAGE_07_ID_23_SC1_MK, &PAGE_07_ID_23_SC1_BRK};
constexpr auto PAGE_07_ID_24_SC1_MK = ScanCode{0x08};
constexpr auto PAGE_07_ID_24_SC1_BRK = ScanCode{0x88};
constexpr auto PAGE_07_ID_24_SC1_PAIR = ScanCodePair{&PAGE_07_ID_24_SC1_MK, &PAGE_07_ID_24_SC1_BRK};
constexpr auto PAGE_07_ID_25_SC1_MK = ScanCode{0x09};
constexpr auto PAGE_07_ID_25_SC1_BRK = ScanCode{0x89};
constexpr auto PAGE_07_ID_25_SC1_PAIR = ScanCodePair{&PAGE_07_ID_25_SC1_MK, &PAGE_07_ID_25_SC1_BRK};
constexpr auto PAGE_07_ID_26_SC1_MK = ScanCode{0x0A};
constexpr auto PAGE_07_ID_26_SC1_BRK = ScanCode{0x8A};
constexpr auto PAGE_07_ID_26_SC1_PAIR = ScanCodePair{&PAGE_07_ID_26_SC1_MK, &PAGE_07_ID_26_SC1_BRK};
constexpr auto PAGE_07_ID_27_SC1_MK = ScanCode{0x0B};
constexpr auto PAGE_07_ID_27_SC1_BRK = ScanCode{0x8B};
constexpr auto PAGE_07_ID_27_SC1_PAIR = ScanCodePair{&PAGE_07_ID_27_SC1_MK, &PAGE_07_ID_27_SC1_BRK};
constexpr auto PAGE_07_ID_28_SC1_MK = ScanCode{0x1C};
constexpr auto PAGE_07_ID_28_SC1_BRK = ScanCode{0x9C};
constexpr auto PAGE_07_ID_28_SC1_PAIR = ScanCodePair{&PAGE_07_ID_28_SC1_MK, &PAGE_07_ID_28_SC1_BRK};
constexpr auto PAGE_07_ID_29_SC1_MK = ScanCode{0x01};
constexpr auto PAGE_07_ID_29_SC1_BRK = ScanCode{0x81};
constexpr auto PAGE_07_ID_29_SC1_PAIR = ScanCodePair{&PAGE_07_ID_29_SC1_MK, &PAGE_07_ID_29_SC1_BRK};
constexpr auto PAGE_07_ID_2A_SC1_MK = ScanCode{0x0E};
constexpr auto PAGE_07_ID_2A_SC1_BRK = ScanCode{0x8E};
constexpr auto PAGE_07_ID_2A_SC1_PAIR = ScanCodePair{&PAGE_07_ID_2A_SC1_MK, &PAGE_07_ID_2A_SC1_BRK};
constexpr auto PAGE_07_ID_2B_SC1_MK = ScanCode{0x0F};
constexpr auto PAGE_07_ID_2B_SC1_BRK = ScanCode{0x8F};
constexpr auto PAGE_07_ID_2B_SC1_PAIR = ScanCodePair{&PAGE_07_ID_2B_SC1_MK, &PAGE_07_ID_2B_SC1_BRK};
constexpr auto PAGE_07_ID_2C_SC1_MK = ScanCode{0x39};
constexpr auto PAGE_07_ID_2C_SC1_BRK = ScanCode{0xB9};
constexpr auto PAGE_07_ID_2C_SC1_PAIR = ScanCodePair{&PAGE_07_ID_2C_SC1_MK, &PAGE_07_ID_2C_SC1_BRK};
constexpr auto PAGE_07_ID_2D_SC1_MK = ScanCode{0x0C};
constexpr auto PAGE_07_ID_2D_SC1_BRK = ScanCode{0x8C};
constexpr auto PAGE_07_ID_2D_SC1_PAIR = ScanCodePair{&PAGE_07_ID_2D_SC1_MK, &PAGE_07_ID_2D_SC1_BRK};
constexpr auto PAGE_07_ID_2E_SC1_MK = ScanCode{0x0D};
constexpr auto PAGE_07_ID_2E_SC1_BRK = ScanCode{0x8D};
constexpr auto PAGE_07_ID_2E_SC1_PAIR = ScanCodePair{&PAGE_07_ID_2E_SC1_MK, &PAGE_07_ID_2E_SC1_BRK};
constexpr auto PAGE_07_ID_2F_SC1_MK = ScanCode{0x1A};
constexpr auto PAGE_07_ID_2F_SC1_BRK = ScanCode{0x9A};
constexpr auto PAGE_07_ID_2F_SC1_PAIR = ScanCodePair{&PAGE_07_ID_2F_SC1_MK, &PAGE_07_ID_2F_SC1_BRK};
constexpr auto PAGE_07_ID_30_SC1_MK = ScanCode{0x1B};
constexpr auto PAGE_07_ID_30_SC1_BRK = ScanCode{0x9B};
constexpr auto PAGE_07_ID_30_SC1_PAIR = ScanCodePair{&PAGE_07_ID_30_SC1_MK, &PAGE_07_ID_30_SC1_BRK};
constexpr auto PAGE_07_ID_31_SC1_MK = ScanCode{0x2B};
constexpr auto PAGE_07_ID_31_SC1_BRK = ScanCode{0xAB};
constexpr auto PAGE_07_ID_31_SC1_PAIR = ScanCodePair{&PAGE_07_ID_31_SC1_MK, &PAGE_07_ID_31_SC1_BRK};
constexpr auto PAGE_07_ID_32_SC1_MK = ScanCode{0x2B};
constexpr auto PAGE_07_ID_32_SC1_BRK = ScanCode{0xAB};
constexpr auto PAGE_07_ID_32_SC1_PAIR = ScanCodePair{&PAGE_07_ID_32_SC1_MK, &PAGE_07_ID_32_SC1_BRK};
constexpr auto PAGE_07_ID_33_SC1_MK = ScanCode{0x27};
constexpr auto PAGE_07_ID_33_SC1_BRK = ScanCode{0xA7};
constexpr auto PAGE_07_ID_33_SC1_PAIR = ScanCodePair{&PAGE_07_ID_33_SC1_MK, &PAGE_07_ID_33_SC1_BRK};
constexpr auto PAGE_07_ID_34_SC1_MK = ScanCode{0x28};
constexpr auto PAGE_07_ID_34_SC1_BRK = ScanCode{0xA8};
constexpr auto PAGE_07_ID_34_SC1_PAIR = ScanCodePair{&PAGE_07_ID_34_SC1_MK, &PAGE_07_ID_34_SC1_BRK};
constexpr auto PAGE_07_ID_35_SC1_MK = ScanCode{0x29};
constexpr auto PAGE_07_ID_35_SC1_BRK = ScanCode{0xA9};
constexpr auto PAGE_07_ID_35_SC1_PAIR = ScanCodePair{&PAGE_07_ID_35_SC1_MK, &PAGE_07_ID_35_SC1_BRK};
constexpr auto PAGE_07_ID_36_SC1_MK = ScanCode{0x33};
constexpr auto PAGE_07_ID_36_SC1_BRK = ScanCode{0xB3};
constexpr auto PAGE_07_ID_36_SC1_PAIR = ScanCodePair{&PAGE_07_ID_36_SC1_MK, &PAGE_07_ID_36_SC1_BRK};
constexpr auto PAGE_07_ID_37_SC1_MK = ScanCode{0x34};
constexpr auto PAGE_07_ID_37_SC1_BRK = ScanCode{0xB4};
constexpr auto PAGE_07_ID_37_SC1_PAIR = ScanCodePair{&PAGE_07_ID_37_SC1_MK, &PAGE_07_ID_37_SC1_BRK};
constexpr auto PAGE_07_ID_38_SC1_MK = ScanCode{0x35};
constexpr auto PAGE_07_ID_38_SC1_BRK = ScanCode{0xB5};
constexpr auto PAGE_07_ID_38_SC1_PAIR = ScanCodePair{&PAGE_07_ID_38_SC1_MK, &PAGE_07_ID_38_SC1_BRK};
constexpr auto PAGE_07_ID_39_SC1_MK = ScanCode{0x3A};
constexpr auto PAGE_07_ID_39_SC1_BRK = ScanCode{0xBA};
constexpr auto PAGE_07_ID_39_SC1_PAIR = ScanCodePair{&PAGE_07_ID_39_SC1_MK, &PAGE_07_ID_39_SC1_BRK};
constexpr auto PAGE_07_ID_3A_SC1_MK = ScanCode{0x3B};
constexpr auto PAGE_07_ID_3A_SC1_BRK = ScanCode{0xBB};
constexpr auto PAGE_07_ID_3A_SC1_PAIR = ScanCodePair{&PAGE_07_ID_3A_SC1_MK, &PAGE_07_ID_3A_SC1_BRK};
constexpr auto PAGE_07_ID_3B_SC1_MK = ScanCode{0x3C};
constexpr auto PAGE_07_ID_3B_SC1_BRK = ScanCode{0xBC};
constexpr auto PAGE_07_ID_3B_SC1_PAIR = ScanCodePair{&PAGE_07_ID_3B_SC1_MK, &PAGE_07_ID_3B_SC1_BRK};
constexpr auto PAGE_07_ID_3C_SC1_MK = ScanCode{0x3D};
constexpr auto PAGE_07_ID_3C_SC1_BRK = ScanCode{0xBD};
constexpr auto PAGE_07_ID_3C_SC1_PAIR = ScanCodePair{&PAGE_07_ID_3C_SC1_MK, &PAGE_07_ID_3C_SC1_BRK};
constexpr auto PAGE_07_ID_3D_SC1_MK = ScanCode{0x3E};
constexpr auto PAGE_07_ID_3D_SC1_BRK = ScanCode{0xBE};
constexpr auto PAGE_07_ID_3D_SC1_PAIR = ScanCodePair{&PAGE_07_ID_3D_SC1_MK, &PAGE_07_ID_3D_SC1_BRK};
constexpr auto PAGE_07_ID_3E_SC1_MK = ScanCode{0x3F};
constexpr auto PAGE_07_ID_3E_SC1_BRK = ScanCode{0xBF};
constexpr auto PAGE_07_ID_3E_SC1_PAIR = ScanCodePair{&PAGE_07_ID_3E_SC1_MK, &PAGE_07_ID_3E_SC1_BRK};
constexpr auto PAGE_07_ID_3F_SC1_MK = ScanCode{0x40};
constexpr auto PAGE_07_ID_3F_SC1_BRK = ScanCode{0xC0};
constexpr auto PAGE_07_ID_3F_SC1_PAIR = ScanCodePair{&PAGE_07_ID_3F_SC1_MK, &PAGE_07_ID_3F_SC1_BRK};
constexpr auto PAGE_07_ID_40_SC1_MK = ScanCode{0x41};
constexpr auto PAGE_07_ID_40_SC1_BRK = ScanCode{0xC1};
constexpr auto PAGE_07_ID_40_SC1_PAIR = ScanCodePair{&PAGE_07_ID_40_SC1_MK, &PAGE_07_ID_40_SC1_BRK};
constexpr auto PAGE_07_ID_41_SC1_MK = ScanCode{0x42};
constexpr auto PAGE_07_ID_41_SC1_BRK = ScanCode{0xC2};
constexpr auto PAGE_07_ID_41_SC1_PAIR = ScanCodePair{&PAGE_07_ID_41_SC1_MK, &PAGE_07_ID_41_SC1_BRK};
constexpr auto PAGE_07_ID_42_SC1_MK = ScanCode{0x43};
constexpr auto PAGE_07_ID_42_SC1_BRK = ScanCode{0xC3};
constexpr auto PAGE_07_ID_42_SC1_PAIR = ScanCodePair{&PAGE_07_ID_42_SC1_MK, &PAGE_07_ID_42_SC1_BRK};
constexpr auto PAGE_07_ID_43_SC1_MK = ScanCode{0x44};
constexpr auto PAGE_07_ID_43_SC1_BRK = ScanCode{0xC4};
constexpr auto PAGE_07_ID_43_SC1_PAIR = ScanCodePair{&PAGE_07_ID_43_SC1_MK, &PAGE_07_ID_43_SC1_BRK};
constexpr auto PAGE_07_ID_44_SC1_MK = ScanCode{0x57};
constexpr auto PAGE_07_ID_44_SC1_BRK = ScanCode{0xD7};
constexpr auto PAGE_07_ID_44_SC1_PAIR = ScanCodePair{&PAGE_07_ID_44_SC1_MK, &PAGE_07_ID_44_SC1_BRK};
constexpr auto PAGE_07_ID_45_SC1_MK = ScanCode{0x58};
constexpr auto PAGE_07_ID_45_SC1_BRK = ScanCode{0xD8};
constexpr auto PAGE_07_ID_45_SC1_PAIR = ScanCodePair{&PAGE_07_ID_45_SC1_MK, &PAGE_07_ID_45_SC1_BRK};
constexpr auto PAGE_07_ID_46_SC1_MK = ScanCode{0xE0, 0x37};
constexpr auto PAGE_07_ID_46_SC1_BRK = ScanCode{0xE0, 0xB7};
constexpr auto PAGE_07_ID_46_SC1_PAIR = ScanCodePair{&PAGE_07_ID_46_SC1_MK, &PAGE_07_ID_46_SC1_BRK};
constexpr auto PAGE_07_ID_47_SC1_MK = ScanCode{0x46};
constexpr auto PAGE_07_ID_47_SC1_BRK = ScanCode{0xC6};
constexpr auto PAGE_07_ID_47_SC1_PAIR = ScanCodePair{&PAGE_07_ID_47_SC1_MK, &PAGE_07_ID_47_SC1_BRK};
constexpr auto PAGE_07_ID_48_SC1_MK = ScanCode{0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5};
constexpr auto PAGE_07_ID_48_SC1_PAIR = ScanCodePair{&PAGE_07_ID_48_SC1_MK, &SC_NONE};
constexpr auto PAGE_07_ID_49_SC1_MK = ScanCode{0xE0, 0x52};
constexpr auto PAGE_07_ID_49_SC1_BRK = ScanCode{0xE0, 0xD2};
constexpr auto PAGE_07_ID_49_SC1_PAIR = ScanCodePair{&PAGE_07_ID_49_SC1_MK, &PAGE_07_ID_49_SC1_BRK};
constexpr auto PAGE_07_ID_4A_SC1_MK = ScanCode{0xE0, 0x47};
constexpr auto PAGE_07_ID_4A_SC1_BRK = ScanCode{0xE0, 0xC7};
constexpr auto PAGE_07_ID_4A_SC1_PAIR = ScanCodePair{&PAGE_07_ID_4A_SC1_MK, &PAGE_07_ID_4A_SC1_BRK};
constexpr auto PAGE_07_ID_4B_SC1_MK = ScanCode{0xE0, 0x49};
constexpr auto PAGE_07_ID_4B_SC1_BRK = ScanCode{0xE0, 0xC9};
constexpr auto PAGE_07_ID_4B_SC1_PAIR = ScanCodePair{&PAGE_07_ID_4B_SC1_MK, &PAGE_07_ID_4B_SC1_BRK};
constexpr auto PAGE_07_ID_4C_SC1_MK = ScanCode{0xE0, 0x53};
constexpr auto PAGE_07_ID_4C_SC1_BRK = ScanCode{0xE0, 0xD3};
constexpr auto PAGE_07_ID_4C_SC1_PAIR = ScanCodePair{&PAGE_07_ID_4C_SC1_MK, &PAGE_07_ID_4C_SC1_BRK};
constexpr auto PAGE_07_ID_4D_SC1_MK = ScanCode{0xE0, 0x4F};
constexpr auto PAGE_07_ID_4D_SC1_BRK = ScanCode{0xE0, 0xCF};
constexpr auto PAGE_07_ID_4D_SC1_PAIR = ScanCodePair{&PAGE_07_ID_4D_SC1_MK, &PAGE_07_ID_4D_SC1_BRK};
constexpr auto PAGE_07_ID_4E_SC1_MK = ScanCode{0xE0, 0x51};
constexpr auto PAGE_07_ID_4E_SC1_BRK = ScanCode{0xE0, 0xD1};
constexpr auto PAGE_07_ID_4E_SC1_PAIR = ScanCodePair{&PAGE_07_ID_4E_SC1_MK, &PAGE_07_ID_4E_SC1_BRK};
constexpr auto PAGE_07_ID_4F_SC1_MK = ScanCode{0xE0, 0x4D};
constexpr auto PAGE_07_ID_4F_SC1_BRK = ScanCode{0xE0, 0xCD};
constexpr auto PAGE_07_ID_4F_SC1_PAIR = ScanCodePair{&PAGE_07_ID_4F_SC1_MK, &PAGE_07_ID_4F_SC1_BRK};
constexpr auto PAGE_07_ID_50_SC1_MK = ScanCode{0xE0, 0x4B};
constexpr auto PAGE_07_ID_50_SC1_BRK = ScanCode{0xE0, 0xCB};
constexpr auto PAGE_07_ID_50_SC1_PAIR = ScanCodePair{&PAGE_07_ID_50_SC1_MK, &PAGE_07_ID_50_SC1_BRK};
constexpr auto PAGE_07_ID_51_SC1_MK = ScanCode{0xE0, 0x50};
constexpr auto PAGE_07_ID_51_SC1_BRK = ScanCode{0xE0, 0xD0};
constexpr auto PAGE_07_ID_51_SC1_PAIR = ScanCodePair{&PAGE_07_ID_51_SC1_MK, &PAGE_07_ID_51_SC1_BRK};
constexpr auto PAGE_07_ID_52_SC1_MK = ScanCode{0xE0, 0x48};
constexpr auto PAGE_07_ID_52_SC1_BRK = ScanCode{0xE0, 0xC8};
constexpr auto PAGE_07_ID_52_SC1_PAIR = ScanCodePair{&PAGE_07_ID_52_SC1_MK, &PAGE_07_ID_52_SC1_BRK};
constexpr auto PAGE_07_ID_53_SC1_MK = ScanCode{0x45};
constexpr auto PAGE_07_ID_53_SC1_BRK = ScanCode{0xC5};
constexpr auto PAGE_07_ID_53_SC1_PAIR = ScanCodePair{&PAGE_07_ID_53_SC1_MK, &PAGE_07_ID_53_SC1_BRK};
constexpr auto PAGE_07_ID_54_SC1_MK = ScanCode{0xE0, 0x35};
constexpr auto PAGE_07_ID_54_SC1_BRK = ScanCode{0xE0, 0xB5};
constexpr auto PAGE_07_ID_54_SC1_PAIR = ScanCodePair{&PAGE_07_ID_54_SC1_MK, &PAGE_07_ID_54_SC1_BRK};
constexpr auto PAGE_07_ID_55_SC1_MK = ScanCode{0x37};
constexpr auto PAGE_07_ID_55_SC1_BRK = ScanCode{0xB7};
constexpr auto PAGE_07_ID_55_SC1_PAIR = ScanCodePair{&PAGE_07_ID_55_SC1_MK, &PAGE_07_ID_55_SC1_BRK};
constexpr auto PAGE_07_ID_56_SC1_MK = ScanCode{0x4A};
constexpr auto PAGE_07_ID_56_SC1_BRK = ScanCode{0xCA};
constexpr auto PAGE_07_ID_56_SC1_PAIR = ScanCodePair{&PAGE_07_ID_56_SC1_MK, &PAGE_07_ID_56_SC1_BRK};
constexpr auto PAGE_07_ID_57_SC1_MK = ScanCode{0x4E};
constexpr auto PAGE_07_ID_57_SC1_BRK = ScanCode{0xCE};
constexpr auto PAGE_07_ID_57_SC1_PAIR = ScanCodePair{&PAGE_07_ID_57_SC1_MK, &PAGE_07_ID_57_SC1_BRK};
constexpr auto PAGE_07_ID_58_SC1_MK = ScanCode{0xE0, 0x1C};
constexpr auto PAGE_07_ID_58_SC1_BRK = ScanCode{0xE0, 0x9C};
constexpr auto PAGE_07_ID_58_SC1_PAIR = ScanCodePair{&PAGE_07_ID_58_SC1_MK, &PAGE_07_ID_58_SC1_BRK};
constexpr auto PAGE_07_ID_59_SC1_MK = ScanCode{0x4F};
constexpr auto PAGE_07_ID_59_SC1_BRK = ScanCode{0xCF};
constexpr auto PAGE_07_ID_59_SC1_PAIR = ScanCodePair{&PAGE_07_ID_59_SC1_MK, &PAGE_07_ID_59_SC1_BRK};
constexpr auto PAGE_07_ID_5A_SC1_MK = ScanCode{0x50};
constexpr auto PAGE_07_ID_5A_SC1_BRK = ScanCode{0xD0};
constexpr auto PAGE_07_ID_5A_SC1_PAIR = ScanCodePair{&PAGE_07_ID_5A_SC1_MK, &PAGE_07_ID_5A_SC1_BRK};
constexpr auto PAGE_07_ID_5B_SC1_MK = ScanCode{0x51};
constexpr auto PAGE_07_ID_5B_SC1_BRK = ScanCode{0xD1};
constexpr auto PAGE_07_ID_5B_SC1_PAIR = ScanCodePair{&PAGE_07_ID_5B_SC1_MK, &PAGE_07_ID_5B_SC1_BRK};
constexpr auto PAGE_07_ID_5C_SC1_MK = ScanCode{0x4B};
constexpr auto PAGE_07_ID_5C_SC1_BRK = ScanCode{0xCB};
constexpr auto PAGE_07_ID_5C_SC1_PAIR = ScanCodePair{&PAGE_07_ID_5C_SC1_MK, &PAGE_07_ID_5C_SC1_BRK};
constexpr auto PAGE_07_ID_5D_SC1_MK = ScanCode{0x4C};
constexpr auto PAGE_07_ID_5D_SC1_BRK = ScanCode{0xCC};
constexpr auto PAGE_07_ID_5D_SC1_PAIR = ScanCodePair{&PAGE_07_ID_5D_SC1_MK, &PAGE_07_ID_5D_SC1_BRK};
constexpr auto PAGE_07_ID_5E_SC1_MK = ScanCode{0x4D};
constexpr auto PAGE_07_ID_5E_SC1_BRK = ScanCode{0xCD};
constexpr auto PAGE_07_ID_5E_SC1_PAIR = ScanCodePair{&PAGE_07_ID_5E_SC1_MK, &PAGE_07_ID_5E_SC1_BRK};
constexpr auto PAGE_07_ID_5F_SC1_MK = ScanCode{0x47};
constexpr auto PAGE_07_ID_5F_SC1_BRK = ScanCode{0xC7};
constexpr auto PAGE_07_ID_5F_SC1_PAIR = ScanCodePair{&PAGE_07_ID_5F_SC1_MK, &PAGE_07_ID_5F_SC1_BRK};
constexpr auto PAGE_07_ID_60_SC1_MK = ScanCode{0x48};
constexpr auto PAGE_07_ID_60_SC1_BRK = ScanCode{0xC8};
constexpr auto PAGE_07_ID_60_SC1_PAIR = ScanCodePair{&PAGE_07_ID_60_SC1_MK, &PAGE_07_ID_60_SC1_BRK};
constexpr auto PAGE_07_ID_61_SC1_MK = ScanCode{0x49};
constexpr auto PAGE_07_ID_61_SC1_BRK = ScanCode{0xC9};
constexpr auto PAGE_07_ID_61_SC1_PAIR = ScanCodePair{&PAGE_07_ID_61_SC1_MK, &PAGE_07_ID_61_SC1_BRK};
constexpr auto PAGE_07_ID_62_SC1_MK = ScanCode{0x52};
constexpr auto PAGE_07_ID_62_SC1_BRK = ScanCode{0xD2};
constexpr auto PAGE_07_ID_62_SC1_PAIR = ScanCodePair{&PAGE_07_ID_62_SC1_MK, &PAGE_07_ID_62_SC1_BRK};
constexpr auto PAGE_07_ID_63_SC1_MK = ScanCode{0x53};
constexpr auto PAGE_07_ID_63_SC1_BRK = ScanCode{0xD3};
constexpr auto PAGE_07_ID_63_SC1_PAIR = ScanCodePair{&PAGE_07_ID_63_SC1_MK, &PAGE_07_ID_63_SC1_BRK};
constexpr auto PAGE_07_ID_64_SC1_MK = ScanCode{0x56};
constexpr auto PAGE_07_ID_64_SC1_BRK = ScanCode{0xD6};
constexpr auto PAGE_07_ID_64_SC1_PAIR = ScanCodePair{&PAGE_07_ID_64_SC1_MK, &PAGE_07_ID_64_SC1_BRK};
constexpr auto PAGE_07_ID_65_SC1_MK = ScanCode{0xE0, 0x5D};
constexpr auto PAGE_07_ID_65_SC1_BRK = ScanCode{0xE0, 0xDD};
constexpr auto PAGE_07_ID_65_SC1_PAIR = ScanCodePair{&PAGE_07_ID_65_SC1_MK, &PAGE_07_ID_65_SC1_BRK};
constexpr auto PAGE_07_ID_66_SC1_MK = ScanCode{0xE0, 0x5E};
constexpr auto PAGE_07_ID_66_SC1_BRK = ScanCode{0xE0, 0xDE};
constexpr auto PAGE_07_ID_66_SC1_PAIR = ScanCodePair{&PAGE_07_ID_66_SC1_MK, &PAGE_07_ID_66_SC1_BRK};
constexpr auto PAGE_07_ID_67_SC1_MK = ScanCode{0x59};
constexpr auto PAGE_07_ID_67_SC1_BRK = ScanCode{0xD9};
constexpr auto PAGE_07_ID_67_SC1_PAIR = ScanCodePair{&PAGE_07_ID_67_SC1_MK, &PAGE_07_ID_67_SC1_BRK};
constexpr auto PAGE_07_ID_68_SC1_MK = ScanCode{0x64};
constexpr auto PAGE_07_ID_68_SC1_BRK = ScanCode{0xE4};
constexpr auto PAGE_07_ID_68_SC1_PAIR = ScanCodePair{&PAGE_07_ID_68_SC1_MK, &PAGE_07_ID_68_SC1_BRK};
constexpr auto PAGE_07_ID_69_SC1_MK = ScanCode{0x65};
constexpr auto PAGE_07_ID_69_SC1_BRK = ScanCode{0xE5};
constexpr auto PAGE_07_ID_69_SC1_PAIR = ScanCodePair{&PAGE_07_ID_69_SC1_MK, &PAGE_07_ID_69_SC1_BRK};
constexpr auto PAGE_07_ID_6A_SC1_MK = ScanCode{0x66};
constexpr auto PAGE_07_ID_6A_SC1_BRK = ScanCode{0xE6};
constexpr auto PAGE_07_ID_6A_SC1_PAIR = ScanCodePair{&PAGE_07_ID_6A_SC1_MK, &PAGE_07_ID_6A_SC1_BRK};
constexpr auto PAGE_07_ID_6B_SC1_MK = ScanCode{0x67};
constexpr auto PAGE_07_ID_6B_SC1_BRK = ScanCode{0xE7};
constexpr auto PAGE_07_ID_6B_SC1_PAIR = ScanCodePair{&PAGE_07_ID_6B_SC1_MK, &PAGE_07_ID_6B_SC1_BRK};
constexpr auto PAGE_07_ID_6C_SC1_MK = ScanCode{0x68};
constexpr auto PAGE_07_ID_6C_SC1_BRK = ScanCode{0xE8};
constexpr auto PAGE_07_ID_6C_SC1_PAIR = ScanCodePair{&PAGE_07_ID_6C_SC1_MK, &PAGE_07_ID_6C_SC1_BRK};
constexpr auto PAGE_07_ID_6D_SC1_MK = ScanCode{0x69};
constexpr auto PAGE_07_ID_6D_SC1_BRK = ScanCode{0xE9};
constexpr auto PAGE_07_ID_6D_SC1_PAIR = ScanCodePair{&PAGE_07_ID_6D_SC1_MK, &PAGE_07_ID_6D_SC1_BRK};
constexpr auto PAGE_07_ID_6E_SC1_MK = ScanCode{0x6A};
constexpr auto PAGE_07_ID_6E_SC1_BRK = ScanCode{0xEA};
constexpr auto PAGE_07_ID_6E_SC1_PAIR = ScanCodePair{&PAGE_07_ID_6E_SC1_MK, &PAGE_07_ID_6E_SC1_BRK};
constexpr auto PAGE_07_ID_6F_SC1_MK = ScanCode{0x6B};
constexpr auto PAGE_07_ID_6F_SC1_BRK = ScanCode{0xEB};
constexpr auto PAGE_07_ID_6F_SC1_PAIR = ScanCodePair{&PAGE_07_ID_6F_SC1_MK, &PAGE_07_ID_6F_SC1_BRK};
constexpr auto PAGE_07_ID_70_SC1_MK = ScanCode{0x6C};
constexpr auto PAGE_07_ID_70_SC1_BRK = ScanCode{0xEC};
constexpr auto PAGE_07_ID_70_SC1_PAIR = ScanCodePair{&PAGE_07_ID_70_SC1_MK, &PAGE_07_ID_70_SC1_BRK};
constexpr auto PAGE_07_ID_71_SC1_MK = ScanCode{0x6D};
constexpr auto PAGE_07_ID_71_SC1_BRK = ScanCode{0xED};
constexpr auto PAGE_07_ID_71_SC1_PAIR = ScanCodePair{&PAGE_07_ID_71_SC1_MK, &PAGE_07_ID_71_SC1_BRK};
constexpr auto PAGE_07_ID_72_SC1_MK = ScanCode{0x6E};
constexpr auto PAGE_07_ID_72_SC1_BRK = ScanCode{0xEE};
constexpr auto PAGE_07_ID_72_SC1_PAIR = ScanCodePair{&PAGE_07_ID_72_SC1_MK, &PAGE_07_ID_72_SC1_BRK};
constexpr auto PAGE_07_ID_73_SC1_MK = ScanCode{0x76};
constexpr auto PAGE_07_ID_73_SC1_BRK = ScanCode{0xF6};
constexpr auto PAGE_07_ID_73_SC1_PAIR = ScanCodePair{&PAGE_07_ID_73_SC1_MK, &PAGE_07_ID_73_SC1_BRK};
constexpr auto PAGE_07_ID_74_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_75_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_76_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_77_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_78_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_79_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7A_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7B_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7C_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7D_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7E_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7F_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_80_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_81_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_82_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_83_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_84_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_85_SC1_MK = ScanCode{0x7E};
constexpr auto PAGE_07_ID_85_SC1_BRK = ScanCode{0xFE};
constexpr auto PAGE_07_ID_85_SC1_PAIR = ScanCodePair{&PAGE_07_ID_85_SC1_MK, &PAGE_07_ID_85_SC1_BRK};
constexpr auto PAGE_07_ID_86_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_87_SC1_MK = ScanCode{0x73};
constexpr auto PAGE_07_ID_87_SC1_BRK = ScanCode{0xF3};
constexpr auto PAGE_07_ID_87_SC1_PAIR = ScanCodePair{&PAGE_07_ID_87_SC1_MK, &PAGE_07_ID_87_SC1_BRK};
constexpr auto PAGE_07_ID_88_SC1_MK = ScanCode{0x70};
constexpr auto PAGE_07_ID_88_SC1_BRK = ScanCode{0xF0};
constexpr auto PAGE_07_ID_88_SC1_PAIR = ScanCodePair{&PAGE_07_ID_88_SC1_MK, &PAGE_07_ID_88_SC1_BRK};
constexpr auto PAGE_07_ID_89_SC1_MK = ScanCode{0x7D};
constexpr auto PAGE_07_ID_89_SC1_BRK = ScanCode{0xFD};
constexpr auto PAGE_07_ID_89_SC1_PAIR = ScanCodePair{&PAGE_07_ID_89_SC1_MK, &PAGE_07_ID_89_SC1_BRK};
constexpr auto PAGE_07_ID_8A_SC1_MK = ScanCode{0x79};
constexpr auto PAGE_07_ID_8A_SC1_BRK = ScanCode{0xF9};
constexpr auto PAGE_07_ID_8A_SC1_PAIR = ScanCodePair{&PAGE_07_ID_8A_SC1_MK, &PAGE_07_ID_8A_SC1_BRK};
constexpr auto PAGE_07_ID_8B_SC1_MK = ScanCode{0x7B};
constexpr auto PAGE_07_ID_8B_SC1_BRK = ScanCode{0xFB};
constexpr auto PAGE_07_ID_8B_SC1_PAIR = ScanCodePair{&PAGE_07_ID_8B_SC1_MK, &PAGE_07_ID_8B_SC1_BRK};
constexpr auto PAGE_07_ID_8C_SC1_MK = ScanCode{0x5C};
constexpr auto PAGE_07_ID_8C_SC1_BRK = ScanCode{0xDC};
constexpr auto PAGE_07_ID_8C_SC1_PAIR = ScanCodePair{&PAGE_07_ID_8C_SC1_MK, &PAGE_07_ID_8C_SC1_BRK};
constexpr auto PAGE_07_ID_8D_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_8E_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_8F_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_90_SC1_MK = ScanCode{0xF2};
constexpr auto PAGE_07_ID_90_SC1_PAIR = ScanCodePair{&PAGE_07_ID_90_SC1_MK, &SC_NONE};
constexpr auto PAGE_07_ID_91_SC1_MK = ScanCode{0xF1};
constexpr auto PAGE_07_ID_91_SC1_PAIR = ScanCodePair{&PAGE_07_ID_91_SC1_MK, &SC_NONE};
constexpr auto PAGE_07_ID_92_SC1_MK = ScanCode{0x78};
constexpr auto PAGE_07_ID_92_SC1_BRK = ScanCode{0xF8};
constexpr auto PAGE_07_ID_92_SC1_PAIR = ScanCodePair{&PAGE_07_ID_92_SC1_MK, &PAGE_07_ID_92_SC1_BRK};
constexpr auto PAGE_07_ID_93_SC1_MK = ScanCode{0x77};
constexpr auto PAGE_07_ID_93_SC1_BRK = ScanCode{0xF7};
constexpr auto PAGE_07_ID_93_SC1_PAIR = ScanCodePair{&PAGE_07_ID_93_SC1_MK, &PAGE_07_ID_93_SC1_BRK};
constexpr auto PAGE_07_ID_94_SC1_MK = ScanCode{0x76};
constexpr auto PAGE_07_ID_94_SC1_BRK = ScanCode{0xF6};
constexpr auto PAGE_07_ID_94_SC1_PAIR = ScanCodePair{&PAGE_07_ID_94_SC1_MK, &PAGE_07_ID_94_SC1_BRK};
constexpr auto PAGE_07_ID_95_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_96_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_97_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_98_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_99_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9A_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9B_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9C_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9D_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9E_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9F_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_A0_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_A1_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_A2_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_A3_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_A4_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_E0_SC1_MK = ScanCode{0x1D};
constexpr auto PAGE_07_ID_E0_SC1_BRK = ScanCode{0x9D};
constexpr auto PAGE_07_ID_E0_SC1_PAIR = ScanCodePair{&PAGE_07_ID_E0_SC1_MK, &PAGE_07_ID_E0_SC1_BRK};
constexpr auto PAGE_07_ID_E1_SC1_MK = ScanCode{0x2A};
constexpr auto PAGE_07_ID_E1_SC1_BRK = ScanCode{0xAA};
constexpr auto PAGE_07_ID_E1_SC1_PAIR = ScanCodePair{&PAGE_07_ID_E1_SC1_MK, &PAGE_07_ID_E1_SC1_BRK};
constexpr auto PAGE_07_ID_E2_SC1_MK = ScanCode{0x38};
constexpr auto PAGE_07_ID_E2_SC1_BRK = ScanCode{0xB8};
constexpr auto PAGE_07_ID_E2_SC1_PAIR = ScanCodePair{&PAGE_07_ID_E2_SC1_MK, &PAGE_07_ID_E2_SC1_BRK};
constexpr auto PAGE_07_ID_E3_SC1_MK = ScanCode{0xE0, 0x5B};
constexpr auto PAGE_07_ID_E3_SC1_BRK = ScanCode{0xE0, 0xDB};
constexpr auto PAGE_07_ID_E3_SC1_PAIR = ScanCodePair{&PAGE_07_ID_E3_SC1_MK, &PAGE_07_ID_E3_SC1_BRK};
constexpr auto PAGE_07_ID_E4_SC1_MK = ScanCode{0xE0, 0x1D};
constexpr auto PAGE_07_ID_E4_SC1_BRK = ScanCode{0xE0, 0x9D};
constexpr auto PAGE_07_ID_E4_SC1_PAIR = ScanCodePair{&PAGE_07_ID_E4_SC1_MK, &PAGE_07_ID_E4_SC1_BRK};
constexpr auto PAGE_07_ID_E5_SC1_MK = ScanCode{0x36};
constexpr auto PAGE_07_ID_E5_SC1_BRK = ScanCode{0xB6};
constexpr auto PAGE_07_ID_E5_SC1_PAIR = ScanCodePair{&PAGE_07_ID_E5_SC1_MK, &PAGE_07_ID_E5_SC1_BRK};
constexpr auto PAGE_07_ID_E6_SC1_MK = ScanCode{0xE0, 0x38};
constexpr auto PAGE_07_ID_E6_SC1_BRK = ScanCode{0xE0, 0xB8};
constexpr auto PAGE_07_ID_E6_SC1_PAIR = ScanCodePair{&PAGE_07_ID_E6_SC1_MK, &PAGE_07_ID_E6_SC1_BRK};
constexpr auto PAGE_07_ID_E7_SC1_MK = ScanCode{0xE0, 0x5C};
constexpr auto PAGE_07_ID_E7_SC1_BRK = ScanCode{0xE0, 0xDC};
constexpr auto PAGE_07_ID_E7_SC1_PAIR = ScanCodePair{&PAGE_07_ID_E7_SC1_MK, &PAGE_07_ID_E7_SC1_BRK};
constexpr auto PAGE_07_SC1_MAP = mapbox::eternal::map<UsageID, const ScanCodePair*>({
  {0x00, &PAGE_07_ID_00_SC1_PAIR},
  {0x01, &PAGE_07_ID_01_SC1_PAIR},
  {0x02, &PAGE_07_ID_02_SC1_PAIR},
  {0x03, &PAGE_07_ID_03_SC1_PAIR},
  {0x04, &PAGE_07_ID_04_SC1_PAIR},
  {0x05, &PAGE_07_ID_05_SC1_PAIR},
  {0x06, &PAGE_07_ID_06_SC1_PAIR},
  {0x07, &PAGE_07_ID_07_SC1_PAIR},
  {0x08, &PAGE_07_ID_08_SC1_PAIR},
  {0x09, &PAGE_07_ID_09_SC1_PAIR},
  {0x0A, &PAGE_07_ID_0A_SC1_PAIR},
  {0x0B, &PAGE_07_ID_0B_SC1_PAIR},
  {0x0C, &PAGE_07_ID_0C_SC1_PAIR},
  {0x0D, &PAGE_07_ID_0D_SC1_PAIR},
  {0x0E, &PAGE_07_ID_0E_SC1_PAIR},
  {0x0F, &PAGE_07_ID_0F_SC1_PAIR},
  {0x10, &PAGE_07_ID_10_SC1_PAIR},
  {0x11, &PAGE_07_ID_11_SC1_PAIR},
  {0x12, &PAGE_07_ID_12_SC1_PAIR},
  {0x13, &PAGE_07_ID_13_SC1_PAIR},
  {0x14, &PAGE_07_ID_14_SC1_PAIR},
  {0x15, &PAGE_07_ID_15_SC1_PAIR},
  {0x16, &PAGE_07_ID_16_SC1_PAIR},
  {0x17, &PAGE_07_ID_17_SC1_PAIR},
  {0x18, &PAGE_07_ID_18_SC1_PAIR},
  {0x19, &PAGE_07_ID_19_SC1_PAIR},
  {0x1A, &PAGE_07_ID_1A_SC1_PAIR},
  {0x1B, &PAGE_07_ID_1B_SC1_PAIR},
  {0x1C, &PAGE_07_ID_1C_SC1_PAIR},
  {0x1D, &PAGE_07_ID_1D_SC1_PAIR},
  {0x1E, &PAGE_07_ID_1E_SC1_PAIR},
  {0x1F, &PAGE_07_ID_1F_SC1_PAIR},
  {0x20, &PAGE_07_ID_20_SC1_PAIR},
  {0x21, &PAGE_07_ID_21_SC1_PAIR},
  {0x22, &PAGE_07_ID_22_SC1_PAIR},
  {0x23, &PAGE_07_ID_23_SC1_PAIR},
  {0x24, &PAGE_07_ID_24_SC1_PAIR},
  {0x25, &PAGE_07_ID_25_SC1_PAIR},
  {0x26, &PAGE_07_ID_26_SC1_PAIR},
  {0x27, &PAGE_07_ID_27_SC1_PAIR},
  {0x28, &PAGE_07_ID_28_SC1_PAIR},
  {0x29, &PAGE_07_ID_29_SC1_PAIR},
  {0x2A, &PAGE_07_ID_2A_SC1_PAIR},
  {0x2B, &PAGE_07_ID_2B_SC1_PAIR},
  {0x2C, &PAGE_07_ID_2C_SC1_PAIR},
  {0x2D, &PAGE_07_ID_2D_SC1_PAIR},
  {0x2E, &PAGE_07_ID_2E_SC1_PAIR},
  {0x2F, &PAGE_07_ID_2F_SC1_PAIR},
  {0x30, &PAGE_07_ID_30_SC1_PAIR},
  {0x31, &PAGE_07_ID_31_SC1_PAIR},
  {0x32, &PAGE_07_ID_32_SC1_PAIR},
  {0x33, &PAGE_07_ID_33_SC1_PAIR},
  {0x34, &PAGE_07_ID_34_SC1_PAIR},
  {0x35, &PAGE_07_ID_35_SC1_PAIR},
  {0x36, &PAGE_07_ID_36_SC1_PAIR},
  {0x37, &PAGE_07_ID_37_SC1_PAIR},
  {0x38, &PAGE_07_ID_38_SC1_PAIR},
  {0x39, &PAGE_07_ID_39_SC1_PAIR},
  {0x3A, &PAGE_07_ID_3A_SC1_PAIR},
  {0x3B, &PAGE_07_ID_3B_SC1_PAIR},
  {0x3C, &PAGE_07_ID_3C_SC1_PAIR},
  {0x3D, &PAGE_07_ID_3D_SC1_PAIR},
  {0x3E, &PAGE_07_ID_3E_SC1_PAIR},
  {0x3F, &PAGE_07_ID_3F_SC1_PAIR},
  {0x40, &PAGE_07_ID_40_SC1_PAIR},
  {0x41, &PAGE_07_ID_41_SC1_PAIR},
  {0x42, &PAGE_07_ID_42_SC1_PAIR},
  {0x43, &PAGE_07_ID_43_SC1_PAIR},
  {0x44, &PAGE_07_ID_44_SC1_PAIR},
  {0x45, &PAGE_07_ID_45_SC1_PAIR},
  {0x46, &PAGE_07_ID_46_SC1_PAIR},
  {0x47, &PAGE_07_ID_47_SC1_PAIR},
  {0x48, &PAGE_07_ID_48_SC1_PAIR},
  {0x49, &PAGE_07_ID_49_SC1_PAIR},
  {0x4A, &PAGE_07_ID_4A_SC1_PAIR},
  {0x4B, &PAGE_07_ID_4B_SC1_PAIR},
  {0x4C, &PAGE_07_ID_4C_SC1_PAIR},
  {0x4D, &PAGE_07_ID_4D_SC1_PAIR},
  {0x4E, &PAGE_07_ID_4E_SC1_PAIR},
  {0x4F, &PAGE_07_ID_4F_SC1_PAIR},
  {0x50, &PAGE_07_ID_50_SC1_PAIR},
  {0x51, &PAGE_07_ID_51_SC1_PAIR},
  {0x52, &PAGE_07_ID_52_SC1_PAIR},
  {0x53, &PAGE_07_ID_53_SC1_PAIR},
  {0x54, &PAGE_07_ID_54_SC1_PAIR},
  {0x55, &PAGE_07_ID_55_SC1_PAIR},
  {0x56, &PAGE_07_ID_56_SC1_PAIR},
  {0x57, &PAGE_07_ID_57_SC1_PAIR},
  {0x58, &PAGE_07_ID_58_SC1_PAIR},
  {0x59, &PAGE_07_ID_59_SC1_PAIR},
  {0x5A, &PAGE_07_ID_5A_SC1_PAIR},
  {0x5B, &PAGE_07_ID_5B_SC1_PAIR},
  {0x5C, &PAGE_07_ID_5C_SC1_PAIR},
  {0x5D, &PAGE_07_ID_5D_SC1_PAIR},
  {0x5E, &PAGE_07_ID_5E_SC1_PAIR},
  {0x5F, &PAGE_07_ID_5F_SC1_PAIR},
  {0x60, &PAGE_07_ID_60_SC1_PAIR},
  {0x61, &PAGE_07_ID_61_SC1_PAIR},
  {0x62, &PAGE_07_ID_62_SC1_PAIR},
  {0x63, &PAGE_07_ID_63_SC1_PAIR},
  {0x64, &PAGE_07_ID_64_SC1_PAIR},
  {0x65, &PAGE_07_ID_65_SC1_PAIR},
  {0x66, &PAGE_07_ID_66_SC1_PAIR},
  {0x67, &PAGE_07_ID_67_SC1_PAIR},
  {0x68, &PAGE_07_ID_68_SC1_PAIR},
  {0x69, &PAGE_07_ID_69_SC1_PAIR},
  {0x6A, &PAGE_07_ID_6A_SC1_PAIR},
  {0x6B, &PAGE_07_ID_6B_SC1_PAIR},
  {0x6C, &PAGE_07_ID_6C_SC1_PAIR},
  {0x6D, &PAGE_07_ID_6D_SC1_PAIR},
  {0x6E, &PAGE_07_ID_6E_SC1_PAIR},
  {0x6F, &PAGE_07_ID_6F_SC1_PAIR},
  {0x70, &PAGE_07_ID_70_SC1_PAIR},
  {0x71, &PAGE_07_ID_71_SC1_PAIR},
  {0x72, &PAGE_07_ID_72_SC1_PAIR},
  {0x73, &PAGE_07_ID_73_SC1_PAIR},
  {0x74, &PAGE_07_ID_74_SC1_PAIR},
  {0x75, &PAGE_07_ID_75_SC1_PAIR},
  {0x76, &PAGE_07_ID_76_SC1_PAIR},
  {0x77, &PAGE_07_ID_77_SC1_PAIR},
  {0x78, &PAGE_07_ID_78_SC1_PAIR},
  {0x79, &PAGE_07_ID_79_SC1_PAIR},
  {0x7A, &PAGE_07_ID_7A_SC1_PAIR},
  {0x7B, &PAGE_07_ID_7B_SC1_PAIR},
  {0x7C, &PAGE_07_ID_7C_SC1_PAIR},
  {0x7D, &PAGE_07_ID_7D_SC1_PAIR},
  {0x7E, &PAGE_07_ID_7E_SC1_PAIR},
  {0x7F, &PAGE_07_ID_7F_SC1_PAIR},
  {0x80, &PAGE_07_ID_80_SC1_PAIR},
  {0x81, &PAGE_07_ID_81_SC1_PAIR},
  {0x82, &PAGE_07_ID_82_SC1_PAIR},
  {0x83, &PAGE_07_ID_83_SC1_PAIR},
  {0x84, &PAGE_07_ID_84_SC1_PAIR},
  {0x85, &PAGE_07_ID_85_SC1_PAIR},
  {0x86, &PAGE_07_ID_86_SC1_PAIR},
  {0x87, &PAGE_07_ID_87_SC1_PAIR},
  {0x88, &PAGE_07_ID_88_SC1_PAIR},
  {0x89, &PAGE_07_ID_89_SC1_PAIR},
  {0x8A, &PAGE_07_ID_8A_SC1_PAIR},
  {0x8B, &PAGE_07_ID_8B_SC1_PAIR},
  {0x8C, &PAGE_07_ID_8C_SC1_PAIR},
  {0x8D, &PAGE_07_ID_8D_SC1_PAIR},
  {0x8E, &PAGE_07_ID_8E_SC1_PAIR},
  {0x8F, &PAGE_07_ID_8F_SC1_PAIR},
  {0x90, &PAGE_07_ID_90_SC1_PAIR},
  {0x91, &PAGE_07_ID_91_SC1_PAIR},
  {0x92, &PAGE_07_ID_92_SC1_PAIR},
  {0x93, &PAGE_07_ID_93_SC1_PAIR},
  {0x94, &PAGE_07_ID_94_SC1_PAIR},
  {0x95, &PAGE_07_ID_95_SC1_PAIR},
  {0x96, &PAGE_07_ID_96_SC1_PAIR},
  {0x97, &PAGE_07_ID_97_SC1_PAIR},
  {0x98, &PAGE_07_ID_98_SC1_PAIR},
  {0x99, &PAGE_07_ID_99_SC1_PAIR},
  {0x9A, &PAGE_07_ID_9A_SC1_PAIR},
  {0x9B, &PAGE_07_ID_9B_SC1_PAIR},
  {0x9C, &PAGE_07_ID_9C_SC1_PAIR},
  {0x9D, &PAGE_07_ID_9D_SC1_PAIR},
  {0x9E, &PAGE_07_ID_9E_SC1_PAIR},
  {0x9F, &PAGE_07_ID_9F_SC1_PAIR},
  {0xA0, &PAGE_07_ID_A0_SC1_PAIR},
  {0xA1, &PAGE_07_ID_A1_SC1_PAIR},
  {0xA2, &PAGE_07_ID_A2_SC1_PAIR},
  {0xA3, &PAGE_07_ID_A3_SC1_PAIR},
  {0xA4, &PAGE_07_ID_A4_SC1_PAIR},
  {0xE0, &PAGE_07_ID_E0_SC1_PAIR},
  {0xE1, &PAGE_07_ID_E1_SC1_PAIR},
  {0xE2, &PAGE_07_ID_E2_SC1_PAIR},
  {0xE3, &PAGE_07_ID_E3_SC1_PAIR},
  {0xE4, &PAGE_07_ID_E4_SC1_PAIR},
  {0xE5, &PAGE_07_ID_E5_SC1_PAIR},
  {0xE6, &PAGE_07_ID_E6_SC1_PAIR},
  {0xE7, &PAGE_07_ID_E7_SC1_PAIR},
});
// Usage Page: 07, PS/2 Set 2
constexpr auto PAGE_07_ID_00_SC2_PAIR = ScanCodePair{&SC_NONE, &SC_NONE};
constexpr auto PAGE_07_ID_01_SC2_MK = ScanCode{0x00};
constexpr auto PAGE_07_ID_01_SC2_PAIR = ScanCodePair{&PAGE_07_ID_01_SC2_MK, &SC_NONE};
constexpr auto PAGE_07_ID_02_SC2_MK = ScanCode{0xFC};
constexpr auto PAGE_07_ID_02_SC2_PAIR = ScanCodePair{&PAGE_07_ID_02_SC2_MK, &SC_NONE};
constexpr auto PAGE_07_ID_03_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_04_SC2_MK = ScanCode{0x1C};
constexpr auto PAGE_07_ID_04_SC2_BRK = ScanCode{0xF0, 0x1C};
constexpr auto PAGE_07_ID_04_SC2_PAIR = ScanCodePair{&PAGE_07_ID_04_SC2_MK, &PAGE_07_ID_04_SC2_BRK};
constexpr auto PAGE_07_ID_05_SC2_MK = ScanCode{0x32};
constexpr auto PAGE_07_ID_05_SC2_BRK = ScanCode{0xF0, 0x32};
constexpr auto PAGE_07_ID_05_SC2_PAIR = ScanCodePair{&PAGE_07_ID_05_SC2_MK, &PAGE_07_ID_05_SC2_BRK};
constexpr auto PAGE_07_ID_06_SC2_MK = ScanCode{0x21};
constexpr auto PAGE_07_ID_06_SC2_BRK = ScanCode{0xF0, 0x21};
constexpr auto PAGE_07_ID_06_SC2_PAIR = ScanCodePair{&PAGE_07_ID_06_SC2_MK, &PAGE_07_ID_06_SC2_BRK};
constexpr auto PAGE_07_ID_07_SC2_MK = ScanCode{0x23};
constexpr auto PAGE_07_ID_07_SC2_BRK = ScanCode{0xF0, 0x23};
constexpr auto PAGE_07_ID_07_SC2_PAIR = ScanCodePair{&PAGE_07_ID_07_SC2_MK, &PAGE_07_ID_07_SC2_BRK};
constexpr auto PAGE_07_ID_08_SC2_MK = ScanCode{0x24};
constexpr auto PAGE_07_ID_08_SC2_BRK = ScanCode{0xF0, 0x24};
constexpr auto PAGE_07_ID_08_SC2_PAIR = ScanCodePair{&PAGE_07_ID_08_SC2_MK, &PAGE_07_ID_08_SC2_BRK};
constexpr auto PAGE_07_ID_09_SC2_MK = ScanCode{0x2B};
constexpr auto PAGE_07_ID_09_SC2_BRK = ScanCode{0xF0, 0x2B};
constexpr auto PAGE_07_ID_09_SC2_PAIR = ScanCodePair{&PAGE_07_ID_09_SC2_MK, &PAGE_07_ID_09_SC2_BRK};
constexpr auto PAGE_07_ID_0A_SC2_MK = ScanCode{0x34};
constexpr auto PAGE_07_ID_0A_SC2_BRK = ScanCode{0xF0, 0x34};
constexpr auto PAGE_07_ID_0A_SC2_PAIR = ScanCodePair{&PAGE_07_ID_0A_SC2_MK, &PAGE_07_ID_0A_SC2_BRK};
constexpr auto PAGE_07_ID_0B_SC2_MK = ScanCode{0x33};
constexpr auto PAGE_07_ID_0B_SC2_BRK = ScanCode{0xF0, 0x33};
constexpr auto PAGE_07_ID_0B_SC2_PAIR = ScanCodePair{&PAGE_07_ID_0B_SC2_MK, &PAGE_07_ID_0B_SC2_BRK};
constexpr auto PAGE_07_ID_0C_SC2_MK = ScanCode{0x43};
constexpr auto PAGE_07_ID_0C_SC2_BRK = ScanCode{0xF0, 0x43};
constexpr auto PAGE_07_ID_0C_SC2_PAIR = ScanCodePair{&PAGE_07_ID_0C_SC2_MK, &PAGE_07_ID_0C_SC2_BRK};
constexpr auto PAGE_07_ID_0D_SC2_MK = ScanCode{0x3B};
constexpr auto PAGE_07_ID_0D_SC2_BRK = ScanCode{0xF0, 0x3B};
constexpr auto PAGE_07_ID_0D_SC2_PAIR = ScanCodePair{&PAGE_07_ID_0D_SC2_MK, &PAGE_07_ID_0D_SC2_BRK};
constexpr auto PAGE_07_ID_0E_SC2_MK = ScanCode{0x42};
constexpr auto PAGE_07_ID_0E_SC2_BRK = ScanCode{0xF0, 0x42};
constexpr auto PAGE_07_ID_0E_SC2_PAIR = ScanCodePair{&PAGE_07_ID_0E_SC2_MK, &PAGE_07_ID_0E_SC2_BRK};
constexpr auto PAGE_07_ID_0F_SC2_MK = ScanCode{0x4B};
constexpr auto PAGE_07_ID_0F_SC2_BRK = ScanCode{0xF0, 0x4B};
constexpr auto PAGE_07_ID_0F_SC2_PAIR = ScanCodePair{&PAGE_07_ID_0F_SC2_MK, &PAGE_07_ID_0F_SC2_BRK};
constexpr auto PAGE_07_ID_10_SC2_MK = ScanCode{0x3A};
constexpr auto PAGE_07_ID_10_SC2_BRK = ScanCode{0xF0, 0x3A};
constexpr auto PAGE_07_ID_10_SC2_PAIR = ScanCodePair{&PAGE_07_ID_10_SC2_MK, &PAGE_07_ID_10_SC2_BRK};
constexpr auto PAGE_07_ID_11_SC2_MK = ScanCode{0x31};
constexpr auto PAGE_07_ID_11_SC2_BRK = ScanCode{0xF0, 0x31};
constexpr auto PAGE_07_ID_11_SC2_PAIR = ScanCodePair{&PAGE_07_ID_11_SC2_MK, &PAGE_07_ID_11_SC2_BRK};
constexpr auto PAGE_07_ID_12_SC2_MK = ScanCode{0x44};
constexpr auto PAGE_07_ID_12_SC2_BRK = ScanCode{0xF0, 0x44};
constexpr auto PAGE_07_ID_12_SC2_PAIR = ScanCodePair{&PAGE_07_ID_12_SC2_MK, &PAGE_07_ID_12_SC2_BRK};
constexpr auto PAGE_07_ID_13_SC2_MK = ScanCode{0x4D};
constexpr auto PAGE_07_ID_13_SC2_BRK = ScanCode{0xF0, 0x4D};
constexpr auto PAGE_07_ID_13_SC2_PAIR = ScanCodePair{&PAGE_07_ID_13_SC2_MK, &PAGE_07_ID_13_SC2_BRK};
constexpr auto PAGE_07_ID_14_SC2_MK = ScanCode{0x15};
constexpr auto PAGE_07_ID_14_SC2_BRK = ScanCode{0xF0, 0x15};
constexpr auto PAGE_07_ID_14_SC2_PAIR = ScanCodePair{&PAGE_07_ID_14_SC2_MK, &PAGE_07_ID_14_SC2_BRK};
constexpr auto PAGE_07_ID_15_SC2_MK = ScanCode{0x2D};
constexpr auto PAGE_07_ID_15_SC2_BRK = ScanCode{0xF0, 0x2D};
constexpr auto PAGE_07_ID_15_SC2_PAIR = ScanCodePair{&PAGE_07_ID_15_SC2_MK, &PAGE_07_ID_15_SC2_BRK};
constexpr auto PAGE_07_ID_16_SC2_MK = ScanCode{0x1B};
constexpr auto PAGE_07_ID_16_SC2_BRK = ScanCode{0xF0, 0x1B};
constexpr auto PAGE_07_ID_16_SC2_PAIR = ScanCodePair{&PAGE_07_ID_16_SC2_MK, &PAGE_07_ID_16_SC2_BRK};
constexpr auto PAGE_07_ID_17_SC2_MK = ScanCode{0x2C};
constexpr auto PAGE_07_ID_17_SC2_BRK = ScanCode{0xF0, 0x2C};
constexpr auto PAGE_07_ID_17_SC2_PAIR = ScanCodePair{&PAGE_07_ID_17_SC2_MK, &PAGE_07_ID_17_SC2_BRK};
constexpr auto PAGE_07_ID_18_SC2_MK = ScanCode{0x3C};
constexpr auto PAGE_07_ID_18_SC2_BRK = ScanCode{0xF0, 0x3C};
constexpr auto PAGE_07_ID_18_SC2_PAIR = ScanCodePair{&PAGE_07_ID_18_SC2_MK, &PAGE_07_ID_18_SC2_BRK};
constexpr auto PAGE_07_ID_19_SC2_MK = ScanCode{0x2A};
constexpr auto PAGE_07_ID_19_SC2_BRK = ScanCode{0xF0, 0x2A};
constexpr auto PAGE_07_ID_19_SC2_PAIR = ScanCodePair{&PAGE_07_ID_19_SC2_MK, &PAGE_07_ID_19_SC2_BRK};
constexpr auto PAGE_07_ID_1A_SC2_MK = ScanCode{0x1D};
constexpr auto PAGE_07_ID_1A_SC2_BRK = ScanCode{0xF0, 0x1D};
constexpr auto PAGE_07_ID_1A_SC2_PAIR = ScanCodePair{&PAGE_07_ID_1A_SC2_MK, &PAGE_07_ID_1A_SC2_BRK};
constexpr auto PAGE_07_ID_1B_SC2_MK = ScanCode{0x22};
constexpr auto PAGE_07_ID_1B_SC2_BRK = ScanCode{0xF0, 0x22};
constexpr auto PAGE_07_ID_1B_SC2_PAIR = ScanCodePair{&PAGE_07_ID_1B_SC2_MK, &PAGE_07_ID_1B_SC2_BRK};
constexpr auto PAGE_07_ID_1C_SC2_MK = ScanCode{0x35};
constexpr auto PAGE_07_ID_1C_SC2_BRK = ScanCode{0xF0, 0x35};
constexpr auto PAGE_07_ID_1C_SC2_PAIR = ScanCodePair{&PAGE_07_ID_1C_SC2_MK, &PAGE_07_ID_1C_SC2_BRK};
constexpr auto PAGE_07_ID_1D_SC2_MK = ScanCode{0x1A};
constexpr auto PAGE_07_ID_1D_SC2_BRK = ScanCode{0xF0, 0x1A};
constexpr auto PAGE_07_ID_1D_SC2_PAIR = ScanCodePair{&PAGE_07_ID_1D_SC2_MK, &PAGE_07_ID_1D_SC2_BRK};
constexpr auto PAGE_07_ID_1E_SC2_MK = ScanCode{0x16};
constexpr auto PAGE_07_ID_1E_SC2_BRK = ScanCode{0xF0, 0x16};
constexpr auto PAGE_07_ID_1E_SC2_PAIR = ScanCodePair{&PAGE_07_ID_1E_SC2_MK, &PAGE_07_ID_1E_SC2_BRK};
constexpr auto PAGE_07_ID_1F_SC2_MK = ScanCode{0x1E};
constexpr auto PAGE_07_ID_1F_SC2_BRK = ScanCode{0xF0, 0x1E};
constexpr auto PAGE_07_ID_1F_SC2_PAIR = ScanCodePair{&PAGE_07_ID_1F_SC2_MK, &PAGE_07_ID_1F_SC2_BRK};
constexpr auto PAGE_07_ID_20_SC2_MK = ScanCode{0x26};
constexpr auto PAGE_07_ID_20_SC2_BRK = ScanCode{0xF0, 0x26};
constexpr auto PAGE_07_ID_20_SC2_PAIR = ScanCodePair{&PAGE_07_ID_20_SC2_MK, &PAGE_07_ID_20_SC2_BRK};
constexpr auto PAGE_07_ID_21_SC2_MK = ScanCode{0x25};
constexpr auto PAGE_07_ID_21_SC2_BRK = ScanCode{0xF0, 0x25};
constexpr auto PAGE_07_ID_21_SC2_PAIR = ScanCodePair{&PAGE_07_ID_21_SC2_MK, &PAGE_07_ID_21_SC2_BRK};
constexpr auto PAGE_07_ID_22_SC2_MK = ScanCode{0x2E};
constexpr auto PAGE_07_ID_22_SC2_BRK = ScanCode{0xF0, 0x2E};
constexpr auto PAGE_07_ID_22_SC2_PAIR = ScanCodePair{&PAGE_07_ID_22_SC2_MK, &PAGE_07_ID_22_SC2_BRK};
constexpr auto PAGE_07_ID_23_SC2_MK = ScanCode{0x36};
constexpr auto PAGE_07_ID_23_SC2_BRK = ScanCode{0xF0, 0x36};
constexpr auto PAGE_07_ID_23_SC2_PAIR = ScanCodePair{&PAGE_07_ID_23_SC2_MK, &PAGE_07_ID_23_SC2_BRK};
constexpr auto PAGE_07_ID_24_SC2_MK = ScanCode{0x3D};
constexpr auto PAGE_07_ID_24_SC2_BRK = ScanCode{0xF0, 0x3D};
constexpr auto PAGE_07_ID_24_SC2_PAIR = ScanCodePair{&PAGE_07_ID_24_SC2_MK, &PAGE_07_ID_24_SC2_BRK};
constexpr auto PAGE_07_ID_25_SC2_MK = ScanCode{0x3E};
constexpr auto PAGE_07_ID_25_SC2_BRK = ScanCode{0xF0, 0x3E};
constexpr auto PAGE_07_ID_25_SC2_PAIR = ScanCodePair{&PAGE_07_ID_25_SC2_MK, &PAGE_07_ID_25_SC2_BRK};
constexpr auto PAGE_07_ID_26_SC2_MK = ScanCode{0x46};
constexpr auto PAGE_07_ID_26_SC2_BRK = ScanCode{0xF0, 0x46};
constexpr auto PAGE_07_ID_26_SC2_PAIR = ScanCodePair{&PAGE_07_ID_26_SC2_MK, &PAGE_07_ID_26_SC2_BRK};
constexpr auto PAGE_07_ID_27_SC2_MK = ScanCode{0x45};
constexpr auto PAGE_07_ID_27_SC2_BRK = ScanCode{0xF0, 0x45};
constexpr auto PAGE_07_ID_27_SC2_PAIR = ScanCodePair{&PAGE_07_ID_27_SC2_MK, &PAGE_07_ID_27_SC2_BRK};
constexpr auto PAGE_07_ID_28_SC2_MK = ScanCode{0x5A};
constexpr auto PAGE_07_ID_28_SC2_BRK = ScanCode{0xF0, 0x5A};
constexpr auto PAGE_07_ID_28_SC2_PAIR = ScanCodePair{&PAGE_07_ID_28_SC2_MK, &PAGE_07_ID_28_SC2_BRK};
constexpr auto PAGE_07_ID_29_SC2_MK = ScanCode{0x76};
constexpr auto PAGE_07_ID_29_SC2_BRK = ScanCode{0xF0, 0x76};
constexpr auto PAGE_07_ID_29_SC2_PAIR = ScanCodePair{&PAGE_07_ID_29_SC2_MK, &PAGE_07_ID_29_SC2_BRK};
constexpr auto PAGE_07_ID_2A_SC2_MK = ScanCode{0x66};
constexpr auto PAGE_07_ID_2A_SC2_BRK = ScanCode{0xF0, 0x66};
constexpr auto PAGE_07_ID_2A_SC2_PAIR = ScanCodePair{&PAGE_07_ID_2A_SC2_MK, &PAGE_07_ID_2A_SC2_BRK};
constexpr auto PAGE_07_ID_2B_SC2_MK = ScanCode{0x0D};
constexpr auto PAGE_07_ID_2B_SC2_BRK = ScanCode{0xF0, 0x0D};
constexpr auto PAGE_07_ID_2B_SC2_PAIR = ScanCodePair{&PAGE_07_ID_2B_SC2_MK, &PAGE_07_ID_2B_SC2_BRK};
constexpr auto PAGE_07_ID_2C_SC2_MK = ScanCode{0x29};
constexpr auto PAGE_07_ID_2C_SC2_BRK = ScanCode{0xF0, 0x29};
constexpr auto PAGE_07_ID_2C_SC2_PAIR = ScanCodePair{&PAGE_07_ID_2C_SC2_MK, &PAGE_07_ID_2C_SC2_BRK};
constexpr auto PAGE_07_ID_2D_SC2_MK = ScanCode{0x4E};
constexpr auto PAGE_07_ID_2D_SC2_BRK = ScanCode{0xF0, 0x4E};
constexpr auto PAGE_07_ID_2D_SC2_PAIR = ScanCodePair{&PAGE_07_ID_2D_SC2_MK, &PAGE_07_ID_2D_SC2_BRK};
constexpr auto PAGE_07_ID_2E_SC2_MK = ScanCode{0x55};
constexpr auto PAGE_07_ID_2E_SC2_BRK = ScanCode{0xF0, 0x55};
constexpr auto PAGE_07_ID_2E_SC2_PAIR = ScanCodePair{&PAGE_07_ID_2E_SC2_MK, &PAGE_07_ID_2E_SC2_BRK};
constexpr auto PAGE_07_ID_2F_SC2_MK = ScanCode{0x54};
constexpr auto PAGE_07_ID_2F_SC2_BRK = ScanCode{0xF0, 0x54};
constexpr auto PAGE_07_ID_2F_SC2_PAIR = ScanCodePair{&PAGE_07_ID_2F_SC2_MK, &PAGE_07_ID_2F_SC2_BRK};
constexpr auto PAGE_07_ID_30_SC2_MK = ScanCode{0x5B};
constexpr auto PAGE_07_ID_30_SC2_BRK = ScanCode{0xF0, 0x5B};
constexpr auto PAGE_07_ID_30_SC2_PAIR = ScanCodePair{&PAGE_07_ID_30_SC2_MK, &PAGE_07_ID_30_SC2_BRK};
constexpr auto PAGE_07_ID_31_SC2_MK = ScanCode{0x5D};
constexpr auto PAGE_07_ID_31_SC2_BRK = ScanCode{0xF0, 0x5D};
constexpr auto PAGE_07_ID_31_SC2_PAIR = ScanCodePair{&PAGE_07_ID_31_SC2_MK, &PAGE_07_ID_31_SC2_BRK};
constexpr auto PAGE_07_ID_32_SC2_MK = ScanCode{0x5D};
constexpr auto PAGE_07_ID_32_SC2_BRK = ScanCode{0xF0, 0x5D};
constexpr auto PAGE_07_ID_32_SC2_PAIR = ScanCodePair{&PAGE_07_ID_32_SC2_MK, &PAGE_07_ID_32_SC2_BRK};
constexpr auto PAGE_07_ID_33_SC2_MK = ScanCode{0x4C};
constexpr auto PAGE_07_ID_33_SC2_BRK = ScanCode{0xF0, 0x4C};
constexpr auto PAGE_07_ID_33_SC2_PAIR = ScanCodePair{&PAGE_07_ID_33_SC2_MK, &PAGE_07_ID_33_SC2_BRK};
constexpr auto PAGE_07_ID_34_SC2_MK = ScanCode{0x52};
constexpr auto PAGE_07_ID_34_SC2_BRK = ScanCode{0xF0, 0x52};
constexpr auto PAGE_07_ID_34_SC2_PAIR = ScanCodePair{&PAGE_07_ID_34_SC2_MK, &PAGE_07_ID_34_SC2_BRK};
constexpr auto PAGE_07_ID_35_SC2_MK = ScanCode{0x0E};
constexpr auto PAGE_07_ID_35_SC2_BRK = ScanCode{0xF0, 0x0E};
constexpr auto PAGE_07_ID_35_SC2_PAIR = ScanCodePair{&PAGE_07_ID_35_SC2_MK, &PAGE_07_ID_35_SC2_BRK};
constexpr auto PAGE_07_ID_36_SC2_MK = ScanCode{0x41};
constexpr auto PAGE_07_ID_36_SC2_BRK = ScanCode{0xF0, 0x41};
constexpr auto PAGE_07_ID_36_SC2_PAIR = ScanCodePair{&PAGE_07_ID_36_SC2_MK, &PAGE_07_ID_36_SC2_BRK};
constexpr auto PAGE_07_ID_37_SC2_MK = ScanCode{0x49};
constexpr auto PAGE_07_ID_37_SC2_BRK = ScanCode{0xF0, 0x49};
constexpr auto PAGE_07_ID_37_SC2_PAIR = ScanCodePair{&PAGE_07_ID_37_SC2_MK, &PAGE_07_ID_37_SC2_BRK};
constexpr auto PAGE_07_ID_38_SC2_MK = ScanCode{0x4A};
constexpr auto PAGE_07_ID_38_SC2_BRK = ScanCode{0xF0, 0x4A};
constexpr auto PAGE_07_ID_38_SC2_PAIR = ScanCodePair{&PAGE_07_ID_38_SC2_MK, &PAGE_07_ID_38_SC2_BRK};
constexpr auto PAGE_07_ID_39_SC2_MK = ScanCode{0x58};
constexpr auto PAGE_07_ID_39_SC2_BRK = ScanCode{0xF0, 0x58};
constexpr auto PAGE_07_ID_39_SC2_PAIR = ScanCodePair{&PAGE_07_ID_39_SC2_MK, &PAGE_07_ID_39_SC2_BRK};
constexpr auto PAGE_07_ID_3A_SC2_MK = ScanCode{0x05};
constexpr auto PAGE_07_ID_3A_SC2_BRK = ScanCode{0xF0, 0x05};
constexpr auto PAGE_07_ID_3A_SC2_PAIR = ScanCodePair{&PAGE_07_ID_3A_SC2_MK, &PAGE_07_ID_3A_SC2_BRK};
constexpr auto PAGE_07_ID_3B_SC2_MK = ScanCode{0x06};
constexpr auto PAGE_07_ID_3B_SC2_BRK = ScanCode{0xF0, 0x06};
constexpr auto PAGE_07_ID_3B_SC2_PAIR = ScanCodePair{&PAGE_07_ID_3B_SC2_MK, &PAGE_07_ID_3B_SC2_BRK};
constexpr auto PAGE_07_ID_3C_SC2_MK = ScanCode{0x04};
constexpr auto PAGE_07_ID_3C_SC2_BRK = ScanCode{0xF0, 0x04};
constexpr auto PAGE_07_ID_3C_SC2_PAIR = ScanCodePair{&PAGE_07_ID_3C_SC2_MK, &PAGE_07_ID_3C_SC2_BRK};
constexpr auto PAGE_07_ID_3D_SC2_MK = ScanCode{0x0C};
constexpr auto PAGE_07_ID_3D_SC2_BRK = ScanCode{0xF0, 0x0C};
constexpr auto PAGE_07_ID_3D_SC2_PAIR = ScanCodePair{&PAGE_07_ID_3D_SC2_MK, &PAGE_07_ID_3D_SC2_BRK};
constexpr auto PAGE_07_ID_3E_SC2_MK = ScanCode{0x03};
constexpr auto PAGE_07_ID_3E_SC2_BRK = ScanCode{0xF0, 0x03};
constexpr auto PAGE_07_ID_3E_SC2_PAIR = ScanCodePair{&PAGE_07_ID_3E_SC2_MK, &PAGE_07_ID_3E_SC2_BRK};
constexpr auto PAGE_07_ID_3F_SC2_MK = ScanCode{0x0B};
constexpr auto PAGE_07_ID_3F_SC2_BRK = ScanCode{0xF0, 0x0B};
constexpr auto PAGE_07_ID_3F_SC2_PAIR = ScanCodePair{&PAGE_07_ID_3F_SC2_MK, &PAGE_07_ID_3F_SC2_BRK};
constexpr auto PAGE_07_ID_40_SC2_MK = ScanCode{0x83};
constexpr auto PAGE_07_ID_40_SC2_BRK = ScanCode{0xF0, 0x83};
constexpr auto PAGE_07_ID_40_SC2_PAIR = ScanCodePair{&PAGE_07_ID_40_SC2_MK, &PAGE_07_ID_40_SC2_BRK};
constexpr auto PAGE_07_ID_41_SC2_MK = ScanCode{0x0A};
constexpr auto PAGE_07_ID_41_SC2_BRK = ScanCode{0xF0, 0x0A};
constexpr auto PAGE_07_ID_41_SC2_PAIR = ScanCodePair{&PAGE_07_ID_41_SC2_MK, &PAGE_07_ID_41_SC2_BRK};
constexpr auto PAGE_07_ID_42_SC2_MK = ScanCode{0x01};
constexpr auto PAGE_07_ID_42_SC2_BRK = ScanCode{0xF0, 0x01};
constexpr auto PAGE_07_ID_42_SC2_PAIR = ScanCodePair{&PAGE_07_ID_42_SC2_MK, &PAGE_07_ID_42_SC2_BRK};
constexpr auto PAGE_07_ID_43_SC2_MK = ScanCode{0x09};
constexpr auto PAGE_07_ID_43_SC2_BRK = ScanCode{0xF0, 0x09};
constexpr auto PAGE_07_ID_43_SC2_PAIR = ScanCodePair{&PAGE_07_ID_43_SC2_MK, &PAGE_07_ID_43_SC2_BRK};
constexpr auto PAGE_07_ID_44_SC2_MK = ScanCode{0x78};
constexpr auto PAGE_07_ID_44_SC2_BRK = ScanCode{0xF0, 0x78};
constexpr auto PAGE_07_ID_44_SC2_PAIR = ScanCodePair{&PAGE_07_ID_44_SC2_MK, &PAGE_07_ID_44_SC2_BRK};
constexpr auto PAGE_07_ID_45_SC2_MK = ScanCode{0x07};
constexpr auto PAGE_07_ID_45_SC2_BRK = ScanCode{0xF0, 0x07};
constexpr auto PAGE_07_ID_45_SC2_PAIR = ScanCodePair{&PAGE_07_ID_45_SC2_MK, &PAGE_07_ID_45_SC2_BRK};
constexpr auto PAGE_07_ID_46_SC2_MK = ScanCode{0xE0, 0x7C};
constexpr auto PAGE_07_ID_46_SC2_BRK = ScanCode{0xE0, 0xF0, 0x7C};
constexpr auto PAGE_07_ID_46_SC2_PAIR = ScanCodePair{&PAGE_07_ID_46_SC2_MK, &PAGE_07_ID_46_SC2_BRK};
constexpr auto PAGE_07_ID_47_SC2_MK = ScanCode{0x7E};
constexpr auto PAGE_07_ID_47_SC2_BRK = ScanCode{0xF0, 0x7E};
constexpr auto PAGE_07_ID_47_SC2_PAIR = ScanCodePair{&PAGE_07_ID_47_SC2_MK, &PAGE_07_ID_47_SC2_BRK};
constexpr auto PAGE_07_ID_48_SC2_MK = ScanCode{0xE1, 0x14, 0x77, 0xE1, 0xF0, 0x14, 0xF0, 0x77};
constexpr auto PAGE_07_ID_48_SC2_PAIR = ScanCodePair{&PAGE_07_ID_48_SC2_MK, &SC_NONE};
constexpr auto PAGE_07_ID_49_SC2_MK = ScanCode{0xE0, 0x70};
constexpr auto PAGE_07_ID_49_SC2_BRK = ScanCode{0xE0, 0xF0, 0x70};
constexpr auto PAGE_07_ID_49_SC2_PAIR = ScanCodePair{&PAGE_07_ID_49_SC2_MK, &PAGE_07_ID_49_SC2_BRK};
constexpr auto PAGE_07_ID_4A_SC2_MK = ScanCode{0xE0, 0x6C};
constexpr auto PAGE_07_ID_4A_SC2_BRK = ScanCode{0xE0, 0xF0, 0x6C};
constexpr auto PAGE_07_ID_4A_SC2_PAIR = ScanCodePair{&PAGE_07_ID_4A_SC2_MK, &PAGE_07_ID_4A_SC2_BRK};
constexpr auto PAGE_07_ID_4B_SC2_MK = ScanCode{0xE0, 0x7D};
constexpr auto PAGE_07_ID_4B_SC2_BRK = ScanCode{0xE0, 0xF0, 0x7D};
constexpr auto PAGE_07_ID_4B_SC2_PAIR = ScanCodePair{&PAGE_07_ID_4B_SC2_MK, &PAGE_07_ID_4B_SC2_BRK};
constexpr auto PAGE_07_ID_4C_SC2_MK = ScanCode{0xE0, 0x71};
constexpr auto PAGE_07_ID_4C_SC2_BRK = ScanCode{0xE0, 0xF0, 0x71};
constexpr auto PAGE_07_ID_4C_SC2_PAIR = ScanCodePair{&PAGE_07_ID_4C_SC2_MK, &PAGE_07_ID_4C_SC2_BRK};
constexpr auto PAGE_07_ID_4D_SC2_MK = ScanCode{0xE0, 0x69};
constexpr auto PAGE_07_ID_4D_SC2_BRK = ScanCode{0xE0, 0xF0, 0x69};
constexpr auto PAGE_07_ID_4D_SC2_PAIR = ScanCodePair{&PAGE_07_ID_4D_SC2_MK, &PAGE_07_ID_4D_SC2_BRK};
constexpr auto PAGE_07_ID_4E_SC2_MK = ScanCode{0xE0, 0x7A};
constexpr auto PAGE_07_ID_4E_SC2_BRK = ScanCode{0xE0, 0xF0, 0x7A};
constexpr auto PAGE_07_ID_4E_SC2_PAIR = ScanCodePair{&PAGE_07_ID_4E_SC2_MK, &PAGE_07_ID_4E_SC2_BRK};
constexpr auto PAGE_07_ID_4F_SC2_MK = ScanCode{0xE0, 0x74};
constexpr auto PAGE_07_ID_4F_SC2_BRK = ScanCode{0xE0, 0xF0, 0x74};
constexpr auto PAGE_07_ID_4F_SC2_PAIR = ScanCodePair{&PAGE_07_ID_4F_SC2_MK, &PAGE_07_ID_4F_SC2_BRK};
constexpr auto PAGE_07_ID_50_SC2_MK = ScanCode{0xE0, 0x6B};
constexpr auto PAGE_07_ID_50_SC2_BRK = ScanCode{0xE0, 0xF0, 0x6B};
constexpr auto PAGE_07_ID_50_SC2_PAIR = ScanCodePair{&PAGE_07_ID_50_SC2_MK, &PAGE_07_ID_50_SC2_BRK};
constexpr auto PAGE_07_ID_51_SC2_MK = ScanCode{0xE0, 0x72};
constexpr auto PAGE_07_ID_51_SC2_BRK = ScanCode{0xE0, 0xF0, 0x72};
constexpr auto PAGE_07_ID_51_SC2_PAIR = ScanCodePair{&PAGE_07_ID_51_SC2_MK, &PAGE_07_ID_51_SC2_BRK};
constexpr auto PAGE_07_ID_52_SC2_MK = ScanCode{0xE0, 0x75};
constexpr auto PAGE_07_ID_52_SC2_BRK = ScanCode{0xE0, 0xF0, 0x75};
constexpr auto PAGE_07_ID_52_SC2_PAIR = ScanCodePair{&PAGE_07_ID_52_SC2_MK, &PAGE_07_ID_52_SC2_BRK};
constexpr auto PAGE_07_ID_53_SC2_MK = ScanCode{0x77};
constexpr auto PAGE_07_ID_53_SC2_BRK = ScanCode{0xF0, 0x77};
constexpr auto PAGE_07_ID_53_SC2_PAIR = ScanCodePair{&PAGE_07_ID_53_SC2_MK, &PAGE_07_ID_53_SC2_BRK};
constexpr auto PAGE_07_ID_54_SC2_MK = ScanCode{0xE0, 0x4A};
constexpr auto PAGE_07_ID_54_SC2_BRK = ScanCode{0xE0, 0xF0, 0x4A};
constexpr auto PAGE_07_ID_54_SC2_PAIR = ScanCodePair{&PAGE_07_ID_54_SC2_MK, &PAGE_07_ID_54_SC2_BRK};
constexpr auto PAGE_07_ID_55_SC2_MK = ScanCode{0x7C};
constexpr auto PAGE_07_ID_55_SC2_BRK = ScanCode{0xF0, 0x7C};
constexpr auto PAGE_07_ID_55_SC2_PAIR = ScanCodePair{&PAGE_07_ID_55_SC2_MK, &PAGE_07_ID_55_SC2_BRK};
constexpr auto PAGE_07_ID_56_SC2_MK = ScanCode{0x7B};
constexpr auto PAGE_07_ID_56_SC2_BRK = ScanCode{0xF0, 0x7B};
constexpr auto PAGE_07_ID_56_SC2_PAIR = ScanCodePair{&PAGE_07_ID_56_SC2_MK, &PAGE_07_ID_56_SC2_BRK};
constexpr auto PAGE_07_ID_57_SC2_MK = ScanCode{0x79};
constexpr auto PAGE_07_ID_57_SC2_BRK = ScanCode{0xF0, 0x79};
constexpr auto PAGE_07_ID_57_SC2_PAIR = ScanCodePair{&PAGE_07_ID_57_SC2_MK, &PAGE_07_ID_57_SC2_BRK};
constexpr auto PAGE_07_ID_58_SC2_MK = ScanCode{0xE0, 0x5A};
constexpr auto PAGE_07_ID_58_SC2_BRK = ScanCode{0xE0, 0xF0, 0x5A};
constexpr auto PAGE_07_ID_58_SC2_PAIR = ScanCodePair{&PAGE_07_ID_58_SC2_MK, &PAGE_07_ID_58_SC2_BRK};
constexpr auto PAGE_07_ID_59_SC2_MK = ScanCode{0x69};
constexpr auto PAGE_07_ID_59_SC2_BRK = ScanCode{0xF0, 0x69};
constexpr auto PAGE_07_ID_59_SC2_PAIR = ScanCodePair{&PAGE_07_ID_59_SC2_MK, &PAGE_07_ID_59_SC2_BRK};
constexpr auto PAGE_07_ID_5A_SC2_MK = ScanCode{0x72};
constexpr auto PAGE_07_ID_5A_SC2_BRK = ScanCode{0xF0, 0x72};
constexpr auto PAGE_07_ID_5A_SC2_PAIR = ScanCodePair{&PAGE_07_ID_5A_SC2_MK, &PAGE_07_ID_5A_SC2_BRK};
constexpr auto PAGE_07_ID_5B_SC2_MK = ScanCode{0x7A};
constexpr auto PAGE_07_ID_5B_SC2_BRK = ScanCode{0xF0, 0x7A};
constexpr auto PAGE_07_ID_5B_SC2_PAIR = ScanCodePair{&PAGE_07_ID_5B_SC2_MK, &PAGE_07_ID_5B_SC2_BRK};
constexpr auto PAGE_07_ID_5C_SC2_MK = ScanCode{0x6B};
constexpr auto PAGE_07_ID_5C_SC2_BRK = ScanCode{0xF0, 0x6B};
constexpr auto PAGE_07_ID_5C_SC2_PAIR = ScanCodePair{&PAGE_07_ID_5C_SC2_MK, &PAGE_07_ID_5C_SC2_BRK};
constexpr auto PAGE_07_ID_5D_SC2_MK = ScanCode{0x73};
constexpr auto PAGE_07_ID_5D_SC2_BRK = ScanCode{0xF0, 0x73};
constexpr auto PAGE_07_ID_5D_SC2_PAIR = ScanCodePair{&PAGE_07_ID_5D_SC2_MK, &PAGE_07_ID_5D_SC2_BRK};
constexpr auto PAGE_07_ID_5E_SC2_MK = ScanCode{0x74};
constexpr auto PAGE_07_ID_5E_SC2_BRK = ScanCode{0xF0, 0x74};
constexpr auto PAGE_07_ID_5E_SC2_PAIR = ScanCodePair{&PAGE_07_ID_5E_SC2_MK, &PAGE_07_ID_5E_SC2_BRK};
constexpr auto PAGE_07_ID_5F_SC2_MK = ScanCode{0x6C};
constexpr auto PAGE_07_ID_5F_SC2_BRK = ScanCode{0xF0, 0x6C};
constexpr auto PAGE_07_ID_5F_SC2_PAIR = ScanCodePair{&PAGE_07_ID_5F_SC2_MK, &PAGE_07_ID_5F_SC2_BRK};
constexpr auto PAGE_07_ID_60_SC2_MK = ScanCode{0x75};
constexpr auto PAGE_07_ID_60_SC2_BRK = ScanCode{0xF0, 0x75};
constexpr auto PAGE_07_ID_60_SC2_PAIR = ScanCodePair{&PAGE_07_ID_60_SC2_MK, &PAGE_07_ID_60_SC2_BRK};
constexpr auto PAGE_07_ID_61_SC2_MK = ScanCode{0x7D};
constexpr auto PAGE_07_ID_61_SC2_BRK = ScanCode{0xF0, 0x7D};
constexpr auto PAGE_07_ID_61_SC2_PAIR = ScanCodePair{&PAGE_07_ID_61_SC2_MK, &PAGE_07_ID_61_SC2_BRK};
constexpr auto PAGE_07_ID_62_SC2_MK = ScanCode{0x70};
constexpr auto PAGE_07_ID_62_SC2_BRK = ScanCode{0xF0, 0x70};
constexpr auto PAGE_07_ID_62_SC2_PAIR = ScanCodePair{&PAGE_07_ID_62_SC2_MK, &PAGE_07_ID_62_SC2_BRK};
constexpr auto PAGE_07_ID_63_SC2_MK = ScanCode{0x71};
constexpr auto PAGE_07_ID_63_SC2_BRK = ScanCode{0xF0, 0x71};
constexpr auto PAGE_07_ID_63_SC2_PAIR = ScanCodePair{&PAGE_07_ID_63_SC2_MK, &PAGE_07_ID_63_SC2_BRK};
constexpr auto PAGE_07_ID_64_SC2_MK = ScanCode{0x61};
constexpr auto PAGE_07_ID_64_SC2_BRK = ScanCode{0xF0, 0x61};
constexpr auto PAGE_07_ID_64_SC2_PAIR = ScanCodePair{&PAGE_07_ID_64_SC2_MK, &PAGE_07_ID_64_SC2_BRK};
constexpr auto PAGE_07_ID_65_SC2_MK = ScanCode{0xE0, 0x2F};
constexpr auto PAGE_07_ID_65_SC2_BRK = ScanCode{0xE0, 0xF0, 0x2F};
constexpr auto PAGE_07_ID_65_SC2_PAIR = ScanCodePair{&PAGE_07_ID_65_SC2_MK, &PAGE_07_ID_65_SC2_BRK};
constexpr auto PAGE_07_ID_66_SC2_MK = ScanCode{0xE0, 0x37};
constexpr auto PAGE_07_ID_66_SC2_BRK = ScanCode{0xE0, 0xF0, 0x37};
constexpr auto PAGE_07_ID_66_SC2_PAIR = ScanCodePair{&PAGE_07_ID_66_SC2_MK, &PAGE_07_ID_66_SC2_BRK};
constexpr auto PAGE_07_ID_67_SC2_MK = ScanCode{0x0F};
constexpr auto PAGE_07_ID_67_SC2_BRK = ScanCode{0xF0, 0x0F};
constexpr auto PAGE_07_ID_67_SC2_PAIR = ScanCodePair{&PAGE_07_ID_67_SC2_MK, &PAGE_07_ID_67_SC2_BRK};
constexpr auto PAGE_07_ID_68_SC2_MK = ScanCode{0x08};
constexpr auto PAGE_07_ID_68_SC2_BRK = ScanCode{0xF0, 0x08};
constexpr auto PAGE_07_ID_68_SC2_PAIR = ScanCodePair{&PAGE_07_ID_68_SC2_MK, &PAGE_07_ID_68_SC2_BRK};
constexpr auto PAGE_07_ID_69_SC2_MK = ScanCode{0x10};
constexpr auto PAGE_07_ID_69_SC2_BRK = ScanCode{0xF0, 0x10};
constexpr auto PAGE_07_ID_69_SC2_PAIR = ScanCodePair{&PAGE_07_ID_69_SC2_MK, &PAGE_07_ID_69_SC2_BRK};
constexpr auto PAGE_07_ID_6A_SC2_MK = ScanCode{0x18};
constexpr auto PAGE_07_ID_6A_SC2_BRK = ScanCode{0xF0, 0x18};
constexpr auto PAGE_07_ID_6A_SC2_PAIR = ScanCodePair{&PAGE_07_ID_6A_SC2_MK, &PAGE_07_ID_6A_SC2_BRK};
constexpr auto PAGE_07_ID_6B_SC2_MK = ScanCode{0x20};
constexpr auto PAGE_07_ID_6B_SC2_BRK = ScanCode{0xF0, 0x20};
constexpr auto PAGE_07_ID_6B_SC2_PAIR = ScanCodePair{&PAGE_07_ID_6B_SC2_MK, &PAGE_07_ID_6B_SC2_BRK};
constexpr auto PAGE_07_ID_6C_SC2_MK = ScanCode{0x28};
constexpr auto PAGE_07_ID_6C_SC2_BRK = ScanCode{0xF0, 0x28};
constexpr auto PAGE_07_ID_6C_SC2_PAIR = ScanCodePair{&PAGE_07_ID_6C_SC2_MK, &PAGE_07_ID_6C_SC2_BRK};
constexpr auto PAGE_07_ID_6D_SC2_MK = ScanCode{0x30};
constexpr auto PAGE_07_ID_6D_SC2_BRK = ScanCode{0xF0, 0x30};
constexpr auto PAGE_07_ID_6D_SC2_PAIR = ScanCodePair{&PAGE_07_ID_6D_SC2_MK, &PAGE_07_ID_6D_SC2_BRK};
constexpr auto PAGE_07_ID_6E_SC2_MK = ScanCode{0x38};
constexpr auto PAGE_07_ID_6E_SC2_BRK = ScanCode{0xF0, 0x38};
constexpr auto PAGE_07_ID_6E_SC2_PAIR = ScanCodePair{&PAGE_07_ID_6E_SC2_MK, &PAGE_07_ID_6E_SC2_BRK};
constexpr auto PAGE_07_ID_6F_SC2_MK = ScanCode{0x40};
constexpr auto PAGE_07_ID_6F_SC2_BRK = ScanCode{0xF0, 0x40};
constexpr auto PAGE_07_ID_6F_SC2_PAIR = ScanCodePair{&PAGE_07_ID_6F_SC2_MK, &PAGE_07_ID_6F_SC2_BRK};
constexpr auto PAGE_07_ID_70_SC2_MK = ScanCode{0x48};
constexpr auto PAGE_07_ID_70_SC2_BRK = ScanCode{0xF0, 0x48};
constexpr auto PAGE_07_ID_70_SC2_PAIR = ScanCodePair{&PAGE_07_ID_70_SC2_MK, &PAGE_07_ID_70_SC2_BRK};
constexpr auto PAGE_07_ID_71_SC2_MK = ScanCode{0x50};
constexpr auto PAGE_07_ID_71_SC2_BRK = ScanCode{0xF0, 0x50};
constexpr auto PAGE_07_ID_71_SC2_PAIR = ScanCodePair{&PAGE_07_ID_71_SC2_MK, &PAGE_07_ID_71_SC2_BRK};
constexpr auto PAGE_07_ID_72_SC2_MK = ScanCode{0x57};
constexpr auto PAGE_07_ID_72_SC2_BRK = ScanCode{0xF0, 0x57};
constexpr auto PAGE_07_ID_72_SC2_PAIR = ScanCodePair{&PAGE_07_ID_72_SC2_MK, &PAGE_07_ID_72_SC2_BRK};
constexpr auto PAGE_07_ID_73_SC2_MK = ScanCode{0x5F};
constexpr auto PAGE_07_ID_73_SC2_BRK = ScanCode{0xF0, 0x5F};
constexpr auto PAGE_07_ID_73_SC2_PAIR = ScanCodePair{&PAGE_07_ID_73_SC2_MK, &PAGE_07_ID_73_SC2_BRK};
constexpr auto PAGE_07_ID_74_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_75_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_76_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_77_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_78_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_79_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7A_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7B_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7C_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7D_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7E_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_7F_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_80_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_81_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_82_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_83_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_84_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_85_SC2_MK = ScanCode{0x6D};
constexpr auto PAGE_07_ID_85_SC2_BRK = ScanCode{0xF0, 0x6D};
constexpr auto PAGE_07_ID_85_SC2_PAIR = ScanCodePair{&PAGE_07_ID_85_SC2_MK, &PAGE_07_ID_85_SC2_BRK};
constexpr auto PAGE_07_ID_86_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_87_SC2_MK = ScanCode{0x51};
constexpr auto PAGE_07_ID_87_SC2_BRK = ScanCode{0xF0, 0x51};
constexpr auto PAGE_07_ID_87_SC2_PAIR = ScanCodePair{&PAGE_07_ID_87_SC2_MK, &PAGE_07_ID_87_SC2_BRK};
constexpr auto PAGE_07_ID_88_SC2_MK = ScanCode{0x13};
constexpr auto PAGE_07_ID_88_SC2_BRK = ScanCode{0xF0, 0x13};
constexpr auto PAGE_07_ID_88_SC2_PAIR = ScanCodePair{&PAGE_07_ID_88_SC2_MK, &PAGE_07_ID_88_SC2_BRK};
constexpr auto PAGE_07_ID_89_SC2_MK = ScanCode{0x6A};
constexpr auto PAGE_07_ID_89_SC2_BRK = ScanCode{0xF0, 0x6A};
constexpr auto PAGE_07_ID_89_SC2_PAIR = ScanCodePair{&PAGE_07_ID_89_SC2_MK, &PAGE_07_ID_89_SC2_BRK};
constexpr auto PAGE_07_ID_8A_SC2_MK = ScanCode{0x64};
constexpr auto PAGE_07_ID_8A_SC2_BRK = ScanCode{0xF0, 0x64};
constexpr auto PAGE_07_ID_8A_SC2_PAIR = ScanCodePair{&PAGE_07_ID_8A_SC2_MK, &PAGE_07_ID_8A_SC2_BRK};
constexpr auto PAGE_07_ID_8B_SC2_MK = ScanCode{0x67};
constexpr auto PAGE_07_ID_8B_SC2_BRK = ScanCode{0xF0, 0x67};
constexpr auto PAGE_07_ID_8B_SC2_PAIR = ScanCodePair{&PAGE_07_ID_8B_SC2_MK, &PAGE_07_ID_8B_SC2_BRK};
constexpr auto PAGE_07_ID_8C_SC2_MK = ScanCode{0x27};
constexpr auto PAGE_07_ID_8C_SC2_BRK = ScanCode{0xF0, 0x27};
constexpr auto PAGE_07_ID_8C_SC2_PAIR = ScanCodePair{&PAGE_07_ID_8C_SC2_MK, &PAGE_07_ID_8C_SC2_BRK};
constexpr auto PAGE_07_ID_8D_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_8E_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_8F_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_90_SC2_MK = ScanCode{0xF2};
constexpr auto PAGE_07_ID_90_SC2_PAIR = ScanCodePair{&PAGE_07_ID_90_SC2_MK, &SC_NONE};
constexpr auto PAGE_07_ID_91_SC2_MK = ScanCode{0xF1};
constexpr auto PAGE_07_ID_91_SC2_PAIR = ScanCodePair{&PAGE_07_ID_91_SC2_MK, &SC_NONE};
constexpr auto PAGE_07_ID_92_SC2_MK = ScanCode{0x63};
constexpr auto PAGE_07_ID_92_SC2_BRK = ScanCode{0xF0, 0x63};
constexpr auto PAGE_07_ID_92_SC2_PAIR = ScanCodePair{&PAGE_07_ID_92_SC2_MK, &PAGE_07_ID_92_SC2_BRK};
constexpr auto PAGE_07_ID_93_SC2_MK = ScanCode{0x62};
constexpr auto PAGE_07_ID_93_SC2_BRK = ScanCode{0xF0, 0x62};
constexpr auto PAGE_07_ID_93_SC2_PAIR = ScanCodePair{&PAGE_07_ID_93_SC2_MK, &PAGE_07_ID_93_SC2_BRK};
constexpr auto PAGE_07_ID_94_SC2_MK = ScanCode{0x5F};
constexpr auto PAGE_07_ID_94_SC2_BRK = ScanCode{0xF0, 0x5F};
constexpr auto PAGE_07_ID_94_SC2_PAIR = ScanCodePair{&PAGE_07_ID_94_SC2_MK, &PAGE_07_ID_94_SC2_BRK};
constexpr auto PAGE_07_ID_95_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_96_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_97_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_98_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_99_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9A_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9B_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9C_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9D_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9E_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_9F_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_A0_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_A1_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_A2_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_A3_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_A4_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_07_ID_E0_SC2_MK = ScanCode{0x14};
constexpr auto PAGE_07_ID_E0_SC2_BRK = ScanCode{0xF0, 0x14};
constexpr auto PAGE_07_ID_E0_SC2_PAIR = ScanCodePair{&PAGE_07_ID_E0_SC2_MK, &PAGE_07_ID_E0_SC2_BRK};
constexpr auto PAGE_07_ID_E1_SC2_MK = ScanCode{0x12};
constexpr auto PAGE_07_ID_E1_SC2_BRK = ScanCode{0xF0, 0x12};
constexpr auto PAGE_07_ID_E1_SC2_PAIR = ScanCodePair{&PAGE_07_ID_E1_SC2_MK, &PAGE_07_ID_E1_SC2_BRK};
constexpr auto PAGE_07_ID_E2_SC2_MK = ScanCode{0x11};
constexpr auto PAGE_07_ID_E2_SC2_BRK = ScanCode{0xF0, 0x11};
constexpr auto PAGE_07_ID_E2_SC2_PAIR = ScanCodePair{&PAGE_07_ID_E2_SC2_MK, &PAGE_07_ID_E2_SC2_BRK};
constexpr auto PAGE_07_ID_E3_SC2_MK = ScanCode{0xE0, 0x1F};
constexpr auto PAGE_07_ID_E3_SC2_BRK = ScanCode{0xE0, 0xF0, 0x1F};
constexpr auto PAGE_07_ID_E3_SC2_PAIR = ScanCodePair{&PAGE_07_ID_E3_SC2_MK, &PAGE_07_ID_E3_SC2_BRK};
constexpr auto PAGE_07_ID_E4_SC2_MK = ScanCode{0xE0, 0x14};
constexpr auto PAGE_07_ID_E4_SC2_BRK = ScanCode{0xE0, 0xF0, 0x14};
constexpr auto PAGE_07_ID_E4_SC2_PAIR = ScanCodePair{&PAGE_07_ID_E4_SC2_MK, &PAGE_07_ID_E4_SC2_BRK};
constexpr auto PAGE_07_ID_E5_SC2_MK = ScanCode{0x59};
constexpr auto PAGE_07_ID_E5_SC2_BRK = ScanCode{0xF0, 0x59};
constexpr auto PAGE_07_ID_E5_SC2_PAIR = ScanCodePair{&PAGE_07_ID_E5_SC2_MK, &PAGE_07_ID_E5_SC2_BRK};
constexpr auto PAGE_07_ID_E6_SC2_MK = ScanCode{0xE0, 0x11};
constexpr auto PAGE_07_ID_E6_SC2_BRK = ScanCode{0xE0, 0xF0, 0x11};
constexpr auto PAGE_07_ID_E6_SC2_PAIR = ScanCodePair{&PAGE_07_ID_E6_SC2_MK, &PAGE_07_ID_E6_SC2_BRK};
constexpr auto PAGE_07_ID_E7_SC2_MK = ScanCode{0xE0, 0x27};
constexpr auto PAGE_07_ID_E7_SC2_BRK = ScanCode{0xE0, 0xF0, 0x27};
constexpr auto PAGE_07_ID_E7_SC2_PAIR = ScanCodePair{&PAGE_07_ID_E7_SC2_MK, &PAGE_07_ID_E7_SC2_BRK};
constexpr auto PAGE_07_SC2_MAP = mapbox::eternal::map<UsageID, const ScanCodePair*>({
  {0x00, &PAGE_07_ID_00_SC2_PAIR},
  {0x01, &PAGE_07_ID_01_SC2_PAIR},
  {0x02, &PAGE_07_ID_02_SC2_PAIR},
  {0x03, &PAGE_07_ID_03_SC2_PAIR},
  {0x04, &PAGE_07_ID_04_SC2_PAIR},
  {0x05, &PAGE_07_ID_05_SC2_PAIR},
  {0x06, &PAGE_07_ID_06_SC2_PAIR},
  {0x07, &PAGE_07_ID_07_SC2_PAIR},
  {0x08, &PAGE_07_ID_08_SC2_PAIR},
  {0x09, &PAGE_07_ID_09_SC2_PAIR},
  {0x0A, &PAGE_07_ID_0A_SC2_PAIR},
  {0x0B, &PAGE_07_ID_0B_SC2_PAIR},
  {0x0C, &PAGE_07_ID_0C_SC2_PAIR},
  {0x0D, &PAGE_07_ID_0D_SC2_PAIR},
  {0x0E, &PAGE_07_ID_0E_SC2_PAIR},
  {0x0F, &PAGE_07_ID_0F_SC2_PAIR},
  {0x10, &PAGE_07_ID_10_SC2_PAIR},
  {0x11, &PAGE_07_ID_11_SC2_PAIR},
  {0x12, &PAGE_07_ID_12_SC2_PAIR},
  {0x13, &PAGE_07_ID_13_SC2_PAIR},
  {0x14, &PAGE_07_ID_14_SC2_PAIR},
  {0x15, &PAGE_07_ID_15_SC2_PAIR},
  {0x16, &PAGE_07_ID_16_SC2_PAIR},
  {0x17, &PAGE_07_ID_17_SC2_PAIR},
  {0x18, &PAGE_07_ID_18_SC2_PAIR},
  {0x19, &PAGE_07_ID_19_SC2_PAIR},
  {0x1A, &PAGE_07_ID_1A_SC2_PAIR},
  {0x1B, &PAGE_07_ID_1B_SC2_PAIR},
  {0x1C, &PAGE_07_ID_1C_SC2_PAIR},
  {0x1D, &PAGE_07_ID_1D_SC2_PAIR},
  {0x1E, &PAGE_07_ID_1E_SC2_PAIR},
  {0x1F, &PAGE_07_ID_1F_SC2_PAIR},
  {0x20, &PAGE_07_ID_20_SC2_PAIR},
  {0x21, &PAGE_07_ID_21_SC2_PAIR},
  {0x22, &PAGE_07_ID_22_SC2_PAIR},
  {0x23, &PAGE_07_ID_23_SC2_PAIR},
  {0x24, &PAGE_07_ID_24_SC2_PAIR},
  {0x25, &PAGE_07_ID_25_SC2_PAIR},
  {0x26, &PAGE_07_ID_26_SC2_PAIR},
  {0x27, &PAGE_07_ID_27_SC2_PAIR},
  {0x28, &PAGE_07_ID_28_SC2_PAIR},
  {0x29, &PAGE_07_ID_29_SC2_PAIR},
  {0x2A, &PAGE_07_ID_2A_SC2_PAIR},
  {0x2B, &PAGE_07_ID_2B_SC2_PAIR},
  {0x2C, &PAGE_07_ID_2C_SC2_PAIR},
  {0x2D, &PAGE_07_ID_2D_SC2_PAIR},
  {0x2E, &PAGE_07_ID_2E_SC2_PAIR},
  {0x2F, &PAGE_07_ID_2F_SC2_PAIR},
  {0x30, &PAGE_07_ID_30_SC2_PAIR},
  {0x31, &PAGE_07_ID_31_SC2_PAIR},
  {0x32, &PAGE_07_ID_32_SC2_PAIR},
  {0x33, &PAGE_07_ID_33_SC2_PAIR},
  {0x34, &PAGE_07_ID_34_SC2_PAIR},
  {0x35, &PAGE_07_ID_35_SC2_PAIR},
  {0x36, &PAGE_07_ID_36_SC2_PAIR},
  {0x37, &PAGE_07_ID_37_SC2_PAIR},
  {0x38, &PAGE_07_ID_38_SC2_PAIR},
  {0x39, &PAGE_07_ID_39_SC2_PAIR},
  {0x3A, &PAGE_07_ID_3A_SC2_PAIR},
  {0x3B, &PAGE_07_ID_3B_SC2_PAIR},
  {0x3C, &PAGE_07_ID_3C_SC2_PAIR},
  {0x3D, &PAGE_07_ID_3D_SC2_PAIR},
  {0x3E, &PAGE_07_ID_3E_SC2_PAIR},
  {0x3F, &PAGE_07_ID_3F_SC2_PAIR},
  {0x40, &PAGE_07_ID_40_SC2_PAIR},
  {0x41, &PAGE_07_ID_41_SC2_PAIR},
  {0x42, &PAGE_07_ID_42_SC2_PAIR},
  {0x43, &PAGE_07_ID_43_SC2_PAIR},
  {0x44, &PAGE_07_ID_44_SC2_PAIR},
  {0x45, &PAGE_07_ID_45_SC2_PAIR},
  {0x46, &PAGE_07_ID_46_SC2_PAIR},
  {0x47, &PAGE_07_ID_47_SC2_PAIR},
  {0x48, &PAGE_07_ID_48_SC2_PAIR},
  {0x49, &PAGE_07_ID_49_SC2_PAIR},
  {0x4A, &PAGE_07_ID_4A_SC2_PAIR},
  {0x4B, &PAGE_07_ID_4B_SC2_PAIR},
  {0x4C, &PAGE_07_ID_4C_SC2_PAIR},
  {0x4D, &PAGE_07_ID_4D_SC2_PAIR},
  {0x4E, &PAGE_07_ID_4E_SC2_PAIR},
  {0x4F, &PAGE_07_ID_4F_SC2_PAIR},
  {0x50, &PAGE_07_ID_50_SC2_PAIR},
  {0x51, &PAGE_07_ID_51_SC2_PAIR},
  {0x52, &PAGE_07_ID_52_SC2_PAIR},
  {0x53, &PAGE_07_ID_53_SC2_PAIR},
  {0x54, &PAGE_07_ID_54_SC2_PAIR},
  {0x55, &PAGE_07_ID_55_SC2_PAIR},
  {0x56, &PAGE_07_ID_56_SC2_PAIR},
  {0x57, &PAGE_07_ID_57_SC2_PAIR},
  {0x58, &PAGE_07_ID_58_SC2_PAIR},
  {0x59, &PAGE_07_ID_59_SC2_PAIR},
  {0x5A, &PAGE_07_ID_5A_SC2_PAIR},
  {0x5B, &PAGE_07_ID_5B_SC2_PAIR},
  {0x5C, &PAGE_07_ID_5C_SC2_PAIR},
  {0x5D, &PAGE_07_ID_5D_SC2_PAIR},
  {0x5E, &PAGE_07_ID_5E_SC2_PAIR},
  {0x5F, &PAGE_07_ID_5F_SC2_PAIR},
  {0x60, &PAGE_07_ID_60_SC2_PAIR},
  {0x61, &PAGE_07_ID_61_SC2_PAIR},
  {0x62, &PAGE_07_ID_62_SC2_PAIR},
  {0x63, &PAGE_07_ID_63_SC2_PAIR},
  {0x64, &PAGE_07_ID_64_SC2_PAIR},
  {0x65, &PAGE_07_ID_65_SC2_PAIR},
  {0x66, &PAGE_07_ID_66_SC2_PAIR},
  {0x67, &PAGE_07_ID_67_SC2_PAIR},
  {0x68, &PAGE_07_ID_68_SC2_PAIR},
  {0x69, &PAGE_07_ID_69_SC2_PAIR},
  {0x6A, &PAGE_07_ID_6A_SC2_PAIR},
  {0x6B, &PAGE_07_ID_6B_SC2_PAIR},
  {0x6C, &PAGE_07_ID_6C_SC2_PAIR},
  {0x6D, &PAGE_07_ID_6D_SC2_PAIR},
  {0x6E, &PAGE_07_ID_6E_SC2_PAIR},
  {0x6F, &PAGE_07_ID_6F_SC2_PAIR},
  {0x70, &PAGE_07_ID_70_SC2_PAIR},
  {0x71, &PAGE_07_ID_71_SC2_PAIR},
  {0x72, &PAGE_07_ID_72_SC2_PAIR},
  {0x73, &PAGE_07_ID_73_SC2_PAIR},
  {0x74, &PAGE_07_ID_74_SC2_PAIR},
  {0x75, &PAGE_07_ID_75_SC2_PAIR},
  {0x76, &PAGE_07_ID_76_SC2_PAIR},
  {0x77, &PAGE_07_ID_77_SC2_PAIR},
  {0x78, &PAGE_07_ID_78_SC2_PAIR},
  {0x79, &PAGE_07_ID_79_SC2_PAIR},
  {0x7A, &PAGE_07_ID_7A_SC2_PAIR},
  {0x7B, &PAGE_07_ID_7B_SC2_PAIR},
  {0x7C, &PAGE_07_ID_7C_SC2_PAIR},
  {0x7D, &PAGE_07_ID_7D_SC2_PAIR},
  {0x7E, &PAGE_07_ID_7E_SC2_PAIR},
  {0x7F, &PAGE_07_ID_7F_SC2_PAIR},
  {0x80, &PAGE_07_ID_80_SC2_PAIR},
  {0x81, &PAGE_07_ID_81_SC2_PAIR},
  {0x82, &PAGE_07_ID_82_SC2_PAIR},
  {0x83, &PAGE_07_ID_83_SC2_PAIR},
  {0x84, &PAGE_07_ID_84_SC2_PAIR},
  {0x85, &PAGE_07_ID_85_SC2_PAIR},
  {0x86, &PAGE_07_ID_86_SC2_PAIR},
  {0x87, &PAGE_07_ID_87_SC2_PAIR},
  {0x88, &PAGE_07_ID_88_SC2_PAIR},
  {0x89, &PAGE_07_ID_89_SC2_PAIR},
  {0x8A, &PAGE_07_ID_8A_SC2_PAIR},
  {0x8B, &PAGE_07_ID_8B_SC2_PAIR},
  {0x8C, &PAGE_07_ID_8C_SC2_PAIR},
  {0x8D, &PAGE_07_ID_8D_SC2_PAIR},
  {0x8E, &PAGE_07_ID_8E_SC2_PAIR},
  {0x8F, &PAGE_07_ID_8F_SC2_PAIR},
  {0x90, &PAGE_07_ID_90_SC2_PAIR},
  {0x91, &PAGE_07_ID_91_SC2_PAIR},
  {0x92, &PAGE_07_ID_92_SC2_PAIR},
  {0x93, &PAGE_07_ID_93_SC2_PAIR},
  {0x94, &PAGE_07_ID_94_SC2_PAIR},
  {0x95, &PAGE_07_ID_95_SC2_PAIR},
  {0x96, &PAGE_07_ID_96_SC2_PAIR},
  {0x97, &PAGE_07_ID_97_SC2_PAIR},
  {0x98, &PAGE_07_ID_98_SC2_PAIR},
  {0x99, &PAGE_07_ID_99_SC2_PAIR},
  {0x9A, &PAGE_07_ID_9A_SC2_PAIR},
  {0x9B, &PAGE_07_ID_9B_SC2_PAIR},
  {0x9C, &PAGE_07_ID_9C_SC2_PAIR},
  {0x9D, &PAGE_07_ID_9D_SC2_PAIR},
  {0x9E, &PAGE_07_ID_9E_SC2_PAIR},
  {0x9F, &PAGE_07_ID_9F_SC2_PAIR},
  {0xA0, &PAGE_07_ID_A0_SC2_PAIR},
  {0xA1, &PAGE_07_ID_A1_SC2_PAIR},
  {0xA2, &PAGE_07_ID_A2_SC2_PAIR},
  {0xA3, &PAGE_07_ID_A3_SC2_PAIR},
  {0xA4, &PAGE_07_ID_A4_SC2_PAIR},
  {0xE0, &PAGE_07_ID_E0_SC2_PAIR},
  {0xE1, &PAGE_07_ID_E1_SC2_PAIR},
  {0xE2, &PAGE_07_ID_E2_SC2_PAIR},
  {0xE3, &PAGE_07_ID_E3_SC2_PAIR},
  {0xE4, &PAGE_07_ID_E4_SC2_PAIR},
  {0xE5, &PAGE_07_ID_E5_SC2_PAIR},
  {0xE6, &PAGE_07_ID_E6_SC2_PAIR},
  {0xE7, &PAGE_07_ID_E7_SC2_PAIR},
});
// Usage Page: 0C, PS/2 Set 1
constexpr auto PAGE_0C_ID_00B5_SC1_MK = ScanCode{0xE0, 0x19};
constexpr auto PAGE_0C_ID_00B5_SC1_BRK = ScanCode{0xE0, 0x99};
constexpr auto PAGE_0C_ID_00B5_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_00B5_SC1_MK, &PAGE_0C_ID_00B5_SC1_BRK};
constexpr auto PAGE_0C_ID_00B6_SC1_MK = ScanCode{0xE0, 0x10};
constexpr auto PAGE_0C_ID_00B6_SC1_BRK = ScanCode{0xE0, 0x90};
constexpr auto PAGE_0C_ID_00B6_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_00B6_SC1_MK, &PAGE_0C_ID_00B6_SC1_BRK};
constexpr auto PAGE_0C_ID_00B7_SC1_MK = ScanCode{0xE0, 0x24};
constexpr auto PAGE_0C_ID_00B7_SC1_BRK = ScanCode{0xE0, 0xA4};
constexpr auto PAGE_0C_ID_00B7_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_00B7_SC1_MK, &PAGE_0C_ID_00B7_SC1_BRK};
constexpr auto PAGE_0C_ID_00CD_SC1_MK = ScanCode{0xE0, 0x22};
constexpr auto PAGE_0C_ID_00CD_SC1_BRK = ScanCode{0xE0, 0xA2};
constexpr auto PAGE_0C_ID_00CD_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_00CD_SC1_MK, &PAGE_0C_ID_00CD_SC1_BRK};
constexpr auto PAGE_0C_ID_00E2_SC1_MK = ScanCode{0xE0, 0x20};
constexpr auto PAGE_0C_ID_00E2_SC1_BRK = ScanCode{0xE0, 0xA0};
constexpr auto PAGE_0C_ID_00E2_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_00E2_SC1_MK, &PAGE_0C_ID_00E2_SC1_BRK};
constexpr auto PAGE_0C_ID_00E5_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_00E7_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_00E9_SC1_MK = ScanCode{0xE0, 0x30};
constexpr auto PAGE_0C_ID_00E9_SC1_BRK = ScanCode{0xE0, 0xB0};
constexpr auto PAGE_0C_ID_00E9_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_00E9_SC1_MK, &PAGE_0C_ID_00E9_SC1_BRK};
constexpr auto PAGE_0C_ID_00EA_SC1_MK = ScanCode{0xE0, 0x2E};
constexpr auto PAGE_0C_ID_00EA_SC1_BRK = ScanCode{0xE0, 0xAE};
constexpr auto PAGE_0C_ID_00EA_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_00EA_SC1_MK, &PAGE_0C_ID_00EA_SC1_BRK};
constexpr auto PAGE_0C_ID_0152_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_0153_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_0154_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_0155_SC1_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_0183_SC1_MK = ScanCode{0xE0, 0x6D};
constexpr auto PAGE_0C_ID_0183_SC1_BRK = ScanCode{0xE0, 0xED};
constexpr auto PAGE_0C_ID_0183_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_0183_SC1_MK, &PAGE_0C_ID_0183_SC1_BRK};
constexpr auto PAGE_0C_ID_018A_SC1_MK = ScanCode{0xE0, 0x6C};
constexpr auto PAGE_0C_ID_018A_SC1_BRK = ScanCode{0xE0, 0xEC};
constexpr auto PAGE_0C_ID_018A_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_018A_SC1_MK, &PAGE_0C_ID_018A_SC1_BRK};
constexpr auto PAGE_0C_ID_0192_SC1_MK = ScanCode{0xE0, 0x21};
constexpr auto PAGE_0C_ID_0192_SC1_BRK = ScanCode{0xE0, 0xA1};
constexpr auto PAGE_0C_ID_0192_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_0192_SC1_MK, &PAGE_0C_ID_0192_SC1_BRK};
constexpr auto PAGE_0C_ID_0194_SC1_MK = ScanCode{0xE0, 0x6B};
constexpr auto PAGE_0C_ID_0194_SC1_BRK = ScanCode{0xE0, 0xEB};
constexpr auto PAGE_0C_ID_0194_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_0194_SC1_MK, &PAGE_0C_ID_0194_SC1_BRK};
constexpr auto PAGE_0C_ID_0221_SC1_MK = ScanCode{0xE0, 0x65};
constexpr auto PAGE_0C_ID_0221_SC1_BRK = ScanCode{0xE0, 0xE5};
constexpr auto PAGE_0C_ID_0221_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_0221_SC1_MK, &PAGE_0C_ID_0221_SC1_BRK};
constexpr auto PAGE_0C_ID_0223_SC1_MK = ScanCode{0xE0, 0x32};
constexpr auto PAGE_0C_ID_0223_SC1_BRK = ScanCode{0xE0, 0xB2};
constexpr auto PAGE_0C_ID_0223_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_0223_SC1_MK, &PAGE_0C_ID_0223_SC1_BRK};
constexpr auto PAGE_0C_ID_0224_SC1_MK = ScanCode{0xE0, 0x6A};
constexpr auto PAGE_0C_ID_0224_SC1_BRK = ScanCode{0xE0, 0xEA};
constexpr auto PAGE_0C_ID_0224_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_0224_SC1_MK, &PAGE_0C_ID_0224_SC1_BRK};
constexpr auto PAGE_0C_ID_0225_SC1_MK = ScanCode{0xE0, 0x69};
constexpr auto PAGE_0C_ID_0225_SC1_BRK = ScanCode{0xE0, 0xE9};
constexpr auto PAGE_0C_ID_0225_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_0225_SC1_MK, &PAGE_0C_ID_0225_SC1_BRK};
constexpr auto PAGE_0C_ID_0226_SC1_MK = ScanCode{0xE0, 0x68};
constexpr auto PAGE_0C_ID_0226_SC1_BRK = ScanCode{0xE0, 0xE8};
constexpr auto PAGE_0C_ID_0226_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_0226_SC1_MK, &PAGE_0C_ID_0226_SC1_BRK};
constexpr auto PAGE_0C_ID_0227_SC1_MK = ScanCode{0xE0, 0x67};
constexpr auto PAGE_0C_ID_0227_SC1_BRK = ScanCode{0xE0, 0xE7};
constexpr auto PAGE_0C_ID_0227_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_0227_SC1_MK, &PAGE_0C_ID_0227_SC1_BRK};
constexpr auto PAGE_0C_ID_022A_SC1_MK = ScanCode{0xE0, 0x66};
constexpr auto PAGE_0C_ID_022A_SC1_BRK = ScanCode{0xE0, 0xE6};
constexpr auto PAGE_0C_ID_022A_SC1_PAIR = ScanCodePair{&PAGE_0C_ID_022A_SC1_MK, &PAGE_0C_ID_022A_SC1_BRK};
constexpr auto PAGE_0C_SC1_MAP = mapbox::eternal::map<UsageID, const ScanCodePair*>({
  {0x00B5, &PAGE_0C_ID_00B5_SC1_PAIR},
  {0x00B6, &PAGE_0C_ID_00B6_SC1_PAIR},
  {0x00B7, &PAGE_0C_ID_00B7_SC1_PAIR},
  {0x00CD, &PAGE_0C_ID_00CD_SC1_PAIR},
  {0x00E2, &PAGE_0C_ID_00E2_SC1_PAIR},
  {0x00E5, &PAGE_0C_ID_00E5_SC1_PAIR},
  {0x00E7, &PAGE_0C_ID_00E7_SC1_PAIR},
  {0x00E9, &PAGE_0C_ID_00E9_SC1_PAIR},
  {0x00EA, &PAGE_0C_ID_00EA_SC1_PAIR},
  {0x0152, &PAGE_0C_ID_0152_SC1_PAIR},
  {0x0153, &PAGE_0C_ID_0153_SC1_PAIR},
  {0x0154, &PAGE_0C_ID_0154_SC1_PAIR},
  {0x0155, &PAGE_0C_ID_0155_SC1_PAIR},
  {0x0183, &PAGE_0C_ID_0183_SC1_PAIR},
  {0x018A, &PAGE_0C_ID_018A_SC1_PAIR},
  {0x0192, &PAGE_0C_ID_0192_SC1_PAIR},
  {0x0194, &PAGE_0C_ID_0194_SC1_PAIR},
  {0x0221, &PAGE_0C_ID_0221_SC1_PAIR},
  {0x0223, &PAGE_0C_ID_0223_SC1_PAIR},
  {0x0224, &PAGE_0C_ID_0224_SC1_PAIR},
  {0x0225, &PAGE_0C_ID_0225_SC1_PAIR},
  {0x0226, &PAGE_0C_ID_0226_SC1_PAIR},
  {0x0227, &PAGE_0C_ID_0227_SC1_PAIR},
  {0x022A, &PAGE_0C_ID_022A_SC1_PAIR},
});
// Usage Page: 0C, PS/2 Set 2
constexpr auto PAGE_0C_ID_00B5_SC2_MK = ScanCode{0xE0, 0x4D};
constexpr auto PAGE_0C_ID_00B5_SC2_BRK = ScanCode{0xE0, 0xF0, 0x4D};
constexpr auto PAGE_0C_ID_00B5_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_00B5_SC2_MK, &PAGE_0C_ID_00B5_SC2_BRK};
constexpr auto PAGE_0C_ID_00B6_SC2_MK = ScanCode{0xE0, 0x15};
constexpr auto PAGE_0C_ID_00B6_SC2_BRK = ScanCode{0xE0, 0xF0, 0x15};
constexpr auto PAGE_0C_ID_00B6_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_00B6_SC2_MK, &PAGE_0C_ID_00B6_SC2_BRK};
constexpr auto PAGE_0C_ID_00B7_SC2_MK = ScanCode{0xE0, 0x3B};
constexpr auto PAGE_0C_ID_00B7_SC2_BRK = ScanCode{0xE0, 0xF0, 0x3B};
constexpr auto PAGE_0C_ID_00B7_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_00B7_SC2_MK, &PAGE_0C_ID_00B7_SC2_BRK};
constexpr auto PAGE_0C_ID_00CD_SC2_MK = ScanCode{0xE0, 0x34};
constexpr auto PAGE_0C_ID_00CD_SC2_BRK = ScanCode{0xE0, 0xF0, 0x34};
constexpr auto PAGE_0C_ID_00CD_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_00CD_SC2_MK, &PAGE_0C_ID_00CD_SC2_BRK};
constexpr auto PAGE_0C_ID_00E2_SC2_MK = ScanCode{0xE0, 0x23};
constexpr auto PAGE_0C_ID_00E2_SC2_BRK = ScanCode{0xE0, 0xF0, 0x23};
constexpr auto PAGE_0C_ID_00E2_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_00E2_SC2_MK, &PAGE_0C_ID_00E2_SC2_BRK};
constexpr auto PAGE_0C_ID_00E5_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_00E7_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_00E9_SC2_MK = ScanCode{0xE0, 0x32};
constexpr auto PAGE_0C_ID_00E9_SC2_BRK = ScanCode{0xE0, 0xF0, 0x32};
constexpr auto PAGE_0C_ID_00E9_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_00E9_SC2_MK, &PAGE_0C_ID_00E9_SC2_BRK};
constexpr auto PAGE_0C_ID_00EA_SC2_MK = ScanCode{0xE0, 0x21};
constexpr auto PAGE_0C_ID_00EA_SC2_BRK = ScanCode{0xE0, 0xF0, 0x21};
constexpr auto PAGE_0C_ID_00EA_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_00EA_SC2_MK, &PAGE_0C_ID_00EA_SC2_BRK};
constexpr auto PAGE_0C_ID_0152_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_0153_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_0154_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_0155_SC2_PAIR = ScanCodePair{&SC_UNASSIGNED, &SC_UNASSIGNED};
constexpr auto PAGE_0C_ID_0183_SC2_MK = ScanCode{0xE0, 0x50};
constexpr auto PAGE_0C_ID_0183_SC2_BRK = ScanCode{0xE0, 0xF0, 0x50};
constexpr auto PAGE_0C_ID_0183_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_0183_SC2_MK, &PAGE_0C_ID_0183_SC2_BRK};
constexpr auto PAGE_0C_ID_018A_SC2_MK = ScanCode{0xE0, 0x48};
constexpr auto PAGE_0C_ID_018A_SC2_BRK = ScanCode{0xE0, 0xF0, 0x48};
constexpr auto PAGE_0C_ID_018A_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_018A_SC2_MK, &PAGE_0C_ID_018A_SC2_BRK};
constexpr auto PAGE_0C_ID_0192_SC2_MK = ScanCode{0xE0, 0x2B};
constexpr auto PAGE_0C_ID_0192_SC2_BRK = ScanCode{0xE0, 0xF0, 0x2B};
constexpr auto PAGE_0C_ID_0192_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_0192_SC2_MK, &PAGE_0C_ID_0192_SC2_BRK};
constexpr auto PAGE_0C_ID_0194_SC2_MK = ScanCode{0xE0, 0x40};
constexpr auto PAGE_0C_ID_0194_SC2_BRK = ScanCode{0xE0, 0xF0, 0x40};
constexpr auto PAGE_0C_ID_0194_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_0194_SC2_MK, &PAGE_0C_ID_0194_SC2_BRK};
constexpr auto PAGE_0C_ID_0221_SC2_MK = ScanCode{0xE0, 0x10};
constexpr auto PAGE_0C_ID_0221_SC2_BRK = ScanCode{0xE0, 0xF0, 0x10};
constexpr auto PAGE_0C_ID_0221_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_0221_SC2_MK, &PAGE_0C_ID_0221_SC2_BRK};
constexpr auto PAGE_0C_ID_0223_SC2_MK = ScanCode{0xE0, 0x3A};
constexpr auto PAGE_0C_ID_0223_SC2_BRK = ScanCode{0xE0, 0xF0, 0x3A};
constexpr auto PAGE_0C_ID_0223_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_0223_SC2_MK, &PAGE_0C_ID_0223_SC2_BRK};
constexpr auto PAGE_0C_ID_0224_SC2_MK = ScanCode{0xE0, 0x38};
constexpr auto PAGE_0C_ID_0224_SC2_BRK = ScanCode{0xE0, 0xF0, 0x38};
constexpr auto PAGE_0C_ID_0224_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_0224_SC2_MK, &PAGE_0C_ID_0224_SC2_BRK};
constexpr auto PAGE_0C_ID_0225_SC2_MK = ScanCode{0xE0, 0x30};
constexpr auto PAGE_0C_ID_0225_SC2_BRK = ScanCode{0xE0, 0xF0, 0x30};
constexpr auto PAGE_0C_ID_0225_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_0225_SC2_MK, &PAGE_0C_ID_0225_SC2_BRK};
constexpr auto PAGE_0C_ID_0226_SC2_MK = ScanCode{0xE0, 0x28};
constexpr auto PAGE_0C_ID_0226_SC2_BRK = ScanCode{0xE0, 0xF0, 0x28};
constexpr auto PAGE_0C_ID_0226_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_0226_SC2_MK, &PAGE_0C_ID_0226_SC2_BRK};
constexpr auto PAGE_0C_ID_0227_SC2_MK = ScanCode{0xE0, 0x20};
constexpr auto PAGE_0C_ID_0227_SC2_BRK = ScanCode{0xE0, 0xF0, 0x20};
constexpr auto PAGE_0C_ID_0227_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_0227_SC2_MK, &PAGE_0C_ID_0227_SC2_BRK};
constexpr auto PAGE_0C_ID_022A_SC2_MK = ScanCode{0xE0, 0x18};
constexpr auto PAGE_0C_ID_022A_SC2_BRK = ScanCode{0xE0, 0xF0, 0x18};
constexpr auto PAGE_0C_ID_022A_SC2_PAIR = ScanCodePair{&PAGE_0C_ID_022A_SC2_MK, &PAGE_0C_ID_022A_SC2_BRK};
constexpr auto PAGE_0C_SC2_MAP = mapbox::eternal::map<UsageID, const ScanCodePair*>({
  {0x00B5, &PAGE_0C_ID_00B5_SC2_PAIR},
  {0x00B6, &PAGE_0C_ID_00B6_SC2_PAIR},
  {0x00B7, &PAGE_0C_ID_00B7_SC2_PAIR},
  {0x00CD, &PAGE_0C_ID_00CD_SC2_PAIR},
  {0x00E2, &PAGE_0C_ID_00E2_SC2_PAIR},
  {0x00E5, &PAGE_0C_ID_00E5_SC2_PAIR},
  {0x00E7, &PAGE_0C_ID_00E7_SC2_PAIR},
  {0x00E9, &PAGE_0C_ID_00E9_SC2_PAIR},
  {0x00EA, &PAGE_0C_ID_00EA_SC2_PAIR},
  {0x0152, &PAGE_0C_ID_0152_SC2_PAIR},
  {0x0153, &PAGE_0C_ID_0153_SC2_PAIR},
  {0x0154, &PAGE_0C_ID_0154_SC2_PAIR},
  {0x0155, &PAGE_0C_ID_0155_SC2_PAIR},
  {0x0183, &PAGE_0C_ID_0183_SC2_PAIR},
  {0x018A, &PAGE_0C_ID_018A_SC2_PAIR},
  {0x0192, &PAGE_0C_ID_0192_SC2_PAIR},
  {0x0194, &PAGE_0C_ID_0194_SC2_PAIR},
  {0x0221, &PAGE_0C_ID_0221_SC2_PAIR},
  {0x0223, &PAGE_0C_ID_0223_SC2_PAIR},
  {0x0224, &PAGE_0C_ID_0224_SC2_PAIR},
  {0x0225, &PAGE_0C_ID_0225_SC2_PAIR},
  {0x0226, &PAGE_0C_ID_0226_SC2_PAIR},
  {0x0227, &PAGE_0C_ID_0227_SC2_PAIR},
  {0x022A, &PAGE_0C_ID_022A_SC2_PAIR},
});
// clang-format on
// GENERATED CODE END

const ScanCode* getScanCode(UsageID id, ScanCodeType type, UsagePage page, ScanCodeSet set) {
  const ScanCodePair* pair = nullptr;
  switch (page) {
    case UsagePage::GENERIC_DESKTOP:
      switch (set) {
        case ScanCodeSet::Set1:
          if (PAGE_01_SC1_MAP.contains(id)) {
            pair = PAGE_01_SC1_MAP.at(id);
          }
          break;
        case ScanCodeSet::Set2:
          if (PAGE_01_SC2_MAP.contains(id)) {
            pair = PAGE_01_SC2_MAP.at(id);
          }
          break;
      }
      break;
    case UsagePage::KEYBOARD_KEYPAD:
      switch (set) {
        case ScanCodeSet::Set1:
          if (PAGE_07_SC1_MAP.contains(id)) {
            pair = PAGE_07_SC1_MAP.at(id);
          }
          break;
        case ScanCodeSet::Set2:
          if (PAGE_07_SC2_MAP.contains(id)) {
            pair = PAGE_07_SC2_MAP.at(id);
          }
          break;
      }
      break;
    case UsagePage::CONSUMER:
      switch (set) {
        case ScanCodeSet::Set1:
          if (PAGE_0C_SC1_MAP.contains(id)) {
            pair = PAGE_0C_SC1_MAP.at(id);
          }
          break;
        case ScanCodeSet::Set2:
          if (PAGE_0C_SC2_MAP.contains(id)) {
            pair = PAGE_0C_SC2_MAP.at(id);
          }
          break;
      }
      break;
  }
  if (pair) {
    switch (type) {
      case ScanCodeType::Make:
        return pair->getMake();
      case ScanCodeType::Break:
        return pair->getBreak();
      default:
        return nullptr;
    }
  } else {
    return nullptr;
  }
}
