#include "keyboard.hpp"

#include "util.hpp"

void KeyboardReport::add(usageID_t usageID) { pressedKeys.push_back(usageID); }

std::vector<usageID_t> KeyboardReport::getPressedKeys() { return pressedKeys; }

KeyboardReport decodeKeyboardInputReport(const std::uint8_t* rawReport, const ReportItemList& inputReportItemList) {
  KeyboardReport pressedKeys;
  for (auto item : inputReportItemList.getItems()) {
    bool isBitmap = item->getReportSize() == 1 && item->getReportCount() > 1;
    bool isUsageIDArray = item->getReportSize() == 8 || item->getReportSize() == 16;
    if (isBitmap) {
      auto bitmap = extractBitsUnsigned(rawReport, item->getBitOffset(), item->getBitLength());
      for (size_t i = 0; i < item->getBitLength(); i++) {
        if (bitmap & (1 << i)) {
          pressedKeys.add(item->getUsageIDs()[i]);
        }
      }
    } else if (isUsageIDArray) {
      for (size_t i = 0; i < item->getReportCount(); i++) {
        auto bitOffset = item->getBitOffset() + i * item->getReportSize();
        auto usageID = extractBitsUnsigned(rawReport, bitOffset, item->getReportSize());
        if (usageID != 0) {  // 0 means no key pressed
          pressedKeys.add(usageID);
        }
      }
    }
  }
  return pressedKeys;
}
