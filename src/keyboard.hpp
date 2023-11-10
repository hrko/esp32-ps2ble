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

// HID Usage Page: 01
enum class HIDUsageIdGenericDesktop : uint16_t {
  System_Power = 0x81,
  System_Sleep = 0x82,
  System_Wake = 0x83,
};

// HID Usage Page: 07
enum class HIDUsageIdKeyboardKeypad : uint16_t {
  No_Event = 0x00,
  Overrun_Error = 0x01,
  POST_Fail = 0x02,
  ErrorUndefined = 0x03,
  a_A = 0x04,
  b_B = 0x05,
  c_C = 0x06,
  d_D = 0x07,
  e_E = 0x08,
  f_F = 0x09,
  g_G = 0x0A,
  h_H = 0x0B,
  i_I = 0x0C,
  j_J = 0x0D,
  k_K = 0x0E,
  l_L = 0x0F,
  m_M = 0x10,
  n_N = 0x11,
  o_O = 0x12,
  p_P = 0x13,
  q_Q = 0x14,
  r_R = 0x15,
  s_S = 0x16,
  t_T = 0x17,
  u_U = 0x18,
  v_V = 0x19,
  w_W = 0x1A,
  x_X = 0x1B,
  y_Y = 0x1C,
  z_Z = 0x1D,
  _1_Exclamation = 0x1E,
  _2_At = 0x1F,
  _3_Hash = 0x20,
  _4_Dollar = 0x21,
  _5_Percent = 0x22,
  _6_Caret = 0x23,
  _7_Ampersand = 0x24,
  _8_Asterisk = 0x25,
  _9_OpenParen = 0x26,
  _0_CloseParen = 0x27,
  Return = 0x28,
  Escape = 0x29,
  Backspace = 0x2A,
  Tab = 0x2B,
  Space = 0x2C,
  HyphenMinus_Underscore = 0x2D,
  Equal_Plus = 0x2E,
  OpenBracket_OpenBrace = 0x2F,
  CloseBracket_CloseBrace = 0x30,
  Backslash_Pipe = 0x31,
  Europe_1 = 0x32,
  Semicolon_Colon = 0x33,
  SingleQuote_DoubleQuote = 0x34,
  Backtick_Tilde = 0x35,
  Comma_LessThan = 0x36,
  Dot_GreaterThan = 0x37,
  Slash_Question = 0x38,
  Caps_Lock = 0x39,
  F1 = 0x3A,
  F2 = 0x3B,
  F3 = 0x3C,
  F4 = 0x3D,
  F5 = 0x3E,
  F6 = 0x3F,
  F7 = 0x40,
  F8 = 0x41,
  F9 = 0x42,
  F10 = 0x43,
  F11 = 0x44,
  F12 = 0x45,
  Print_Screen = 0x46,
  Scroll_Lock = 0x47,
  Pause = 0x48,
  Insert = 0x49,
  Home = 0x4A,
  Page_Up = 0x4B,
  Delete = 0x4C,
  End = 0x4D,
  Page_Down = 0x4E,
  Right_Arrow = 0x4F,
  Left_Arrow = 0x50,
  Down_Arrow = 0x51,
  Up_Arrow = 0x52,
  Num_Lock = 0x53,
  Keypad_Slash = 0x54,
  Keypad_Asterisk = 0x55,
  Keypad_Minus = 0x56,
  Keypad_Plus = 0x57,
  Keypad_Enter = 0x58,
  Keypad_1_End = 0x59,
  Keypad_2_Down = 0x5A,
  Keypad_3_PageDn = 0x5B,
  Keypad_4_Left = 0x5C,
  Keypad_5 = 0x5D,
  Keypad_6_Right = 0x5E,
  Keypad_7_Home = 0x5F,
  Keypad_8_Up = 0x60,
  Keypad_9_PageUp = 0x61,
  Keypad_0_Insert = 0x62,
  Keypad_Dot_Delete = 0x63,
  Europe_2 = 0x64,
  App = 0x65,
  Keyboard_Power = 0x66,
  Keypad_Equal = 0x67,
  F13 = 0x68,
  F14 = 0x69,
  F15 = 0x6A,
  F16 = 0x6B,
  F17 = 0x6C,
  F18 = 0x6D,
  F19 = 0x6E,
  F20 = 0x6F,
  F21 = 0x70,
  F22 = 0x71,
  F23 = 0x72,
  F24 = 0x73,
  Keyboard_Execute = 0x74,
  Keyboard_Help = 0x75,
  Keyboard_Menu = 0x76,
  Keyboard_Select = 0x77,
  Keyboard_Stop = 0x78,
  Keyboard_Again = 0x79,
  Keyboard_Undo = 0x7A,
  Keyboard_Cut = 0x7B,
  Keyboard_Copy = 0x7C,
  Keyboard_Paste = 0x7D,
  Keyboard_Find = 0x7E,
  Keyboard_Mute = 0x7F,
  Keyboard_Volume_Up = 0x80,
  Keyboard_Volume_Dn = 0x81,
  Keyboard_Locking_Caps_Lock = 0x82,
  Keyboard_Locking_Num_Lock = 0x83,
  Keyboard_Locking_Scroll_Lock = 0x84,
  Keypad_Comma = 0x85,
  Keyboard_Equal_Sign = 0x86,
  Ro = 0x87,
  Katakana_Hiragana = 0x88,
  Yen = 0x89,
  Henkan = 0x8A,
  Muhenkan = 0x8B,
  PC9800_Keypad_Comma = 0x8C,
  Keyboard_IntSingleQuotel_7 = 0x8D,
  Keyboard_IntSingleQuotel_8 = 0x8E,
  Keyboard_IntSingleQuotel_9 = 0x8F,
  Hanguel_English = 0x90,
  Hanja = 0x91,
  Katakana = 0x92,
  Hiragana = 0x93,
  Zenkaku_Hankaku = 0x94,
  Keyboard_Lang_6 = 0x95,
  Keyboard_Lang_7 = 0x96,
  Keyboard_Lang_8 = 0x97,
  Keyboard_Lang_9 = 0x98,
  Keyboard_Alternate_Erase = 0x99,
  Keyboard_SysReq_Attention = 0x9A,
  Keyboard_Cancel = 0x9B,
  Keyboard_Clear = 0x9C,
  Keyboard_Prior = 0x9D,
  Keyboard_Return = 0x9E,
  Keyboard_Separator = 0x9F,
  Keyboard_Out = 0xA0,
  Keyboard_Oper = 0xA1,
  Keyboard_Clear_Again = 0xA2,
  Keyboard_CrSel_Props = 0xA3,
  Keyboard_ExSel = 0xA4,
  Left_Control = 0xE0,
  Left_Shift = 0xE1,
  Left_Alt = 0xE2,
  Left_GUI = 0xE3,
  Right_Control = 0xE4,
  Right_Shift = 0xE5,
  Right_Alt = 0xE6,
  Right_GUI = 0xE7,
};

// HID Usage Page: 0C
enum class HIDUsageIdConsumer : uint16_t {
  Scan_Next_Track = 0x00B5,
  Scan_Previous_Track = 0x00B6,
  Stop = 0x00B7,
  Play_Pause = 0x00CD,
  Mute = 0x00E2,
  Bass_Boost = 0x00E5,
  Loudness = 0x00E7,
  Volume_Up = 0x00E9,
  Volume_Down = 0x00EA,
  Bass_Up = 0x0152,
  Bass_Down = 0x0153,
  Treble_Up = 0x0154,
  Treble_Down = 0x0155,
  Media_Select = 0x0183,
  Mail = 0x018A,
  Calculator = 0x0192,
  My_Computer = 0x0194,
  WWW_Search = 0x0221,
  WWW_Home = 0x0223,
  WWW_Back = 0x0224,
  WWW_Forward = 0x0225,
  WWW_Stop = 0x0226,
  WWW_Refresh = 0x0227,
  WWW_Favorites = 0x022A,
};

#endif /* A3BB5F1C_3BAE_4E29_ACA7_14CE74FC633C */
