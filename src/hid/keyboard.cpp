#include "keyboard.hpp"

#include <fmt/core.h>

#include "util.hpp"

void KeyboardReport::add(usageID_t usageID) { pressedKeys.push_back(usageID); }

std::vector<usageID_t> KeyboardReport::getPressedKeys() { return pressedKeys; }

usagePage_t KeyboardReport::getUsagePage() { return usagePage; }

void KeyboardReport::setUsagePage(usagePage_t usagePage) { this->usagePage = usagePage; }

std::string KeyboardReport::toString() {
  std::string result = "KeyboardReport {";
  for (auto usageID : pressedKeys) {
    result += fmt::format("0x{:04x}, ", usageID);
  }
  result += "}";
  return result;
}

KeyboardReport decodeKeyboardInputReport(const std::uint8_t* rawReport, const ReportItemList& inputReportItemList) {
  KeyboardReport pressedKeys;
  if (inputReportItemList.getItems().empty()) {
    return pressedKeys;
  }
  // Assume all items in the report have the same usage page
  pressedKeys.setUsagePage(inputReportItemList.getItems()[0]->getUsagePage());
  for (auto item : inputReportItemList.getItems()) {
    // If different usage pages are used in the same report, we ignore the later ones
    if (item->getUsagePage() != pressedKeys.getUsagePage()) {
      continue;
    }
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
