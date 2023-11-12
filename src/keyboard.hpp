#ifndef A3BB5F1C_3BAE_4E29_ACA7_14CE74FC633C
#define A3BB5F1C_3BAE_4E29_ACA7_14CE74FC633C

#include <string>
#include <initializer_list>
#include <cstdint>

class ScanCode {
  std::initializer_list<std::uint8_t> code;

 public:
  constexpr ScanCode();
  constexpr ScanCode(std::initializer_list<std::uint8_t> list);

  std::string toString() const;
  const std::initializer_list<std::uint8_t>* getCode() const;
};

class ScanCodePair {
  const ScanCode* makeCode;
  const ScanCode* breakCode;

 public:
  constexpr ScanCodePair();
  constexpr ScanCodePair(const ScanCode* makeCode, const ScanCode* breakCode);

  const ScanCode* getMake() const;
  const ScanCode* getBreak() const;
};

enum class UsagePage : std::uint16_t {
  GenericDesktop = 0x01,
  KeyboardKeypad = 0x07,
  Consumer = 0x0C,
};

enum class ScanCodeSet {
  Set1 = 1,
  Set2 = 2,
};

enum class ScanCodeType {
  Make,
  Break,
};

using UsageID = std::uint16_t;

const ScanCode* getScanCode(UsageID id, ScanCodeType type, UsagePage page, ScanCodeSet set);

#endif /* A3BB5F1C_3BAE_4E29_ACA7_14CE74FC633C */
