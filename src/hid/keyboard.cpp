#include "keyboard.hpp"

#include "util.hpp"

void KeyboardPressedKeys::add(usageID_t usageID) { pressedKeys.push_back(usageID); }

void KeyboardPressedKeys::remove(usageID_t usageID) {
  for (auto it = pressedKeys.begin(); it != pressedKeys.end(); it++) {
    if (*it == usageID) {
      pressedKeys.erase(it);
      break;
    }
  }
}

std::vector<usageID_t> KeyboardPressedKeys::get() { return pressedKeys; }

KeyboardPressedKeys decodeKeyboardInputReport(const std::uint8_t* report, ReportItemList* inputReportItemList) {
  KeyboardPressedKeys pressedKeys;
  for (auto item : inputReportItemList->getItems()) {
    bool isBitmap = item->getReportSize() == 1 && item->getReportCount() > 1;
    bool isUsageIDArray = item->getReportSize() == 8 || item->getReportSize() == 16;
    if (isBitmap) {
      auto bitmap = extractBitsUnsigned(report, item->getBitOffset(), item->getBitLength());
      for (size_t i = 0; i < item->getBitLength(); i++) {
        if (bitmap & (1 << i)) {
          pressedKeys.add(item->getUsageIDs()[i]);
        }
      }
    } else if (isUsageIDArray) {
      for (size_t i = 0; i < item->getReportCount(); i++) {
        auto bitOffset = item->getBitOffset() + i * item->getReportSize();
        auto usageID = extractBitsUnsigned(report, bitOffset, item->getReportSize());
        if (usageID != 0) {  // 0 means no key pressed
          pressedKeys.add(usageID);
        }
      }
    }
  }
  return pressedKeys;
}
