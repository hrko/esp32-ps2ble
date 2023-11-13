#include "mouse.hpp"

#include <fmt/core.h>

#include <cstdint>

#include "common.hpp"
#include "util.hpp"

std::string MouseReport::toString() {
  return fmt::format(
      "MouseReport {{x: {}, y: {}, wheelVertical: {}, wheelHorizontal: {}, isButtonPressed: [{}, {}, {}, {}, {}, {}, {}, {}]}}", x, y,
      wheelVertical, wheelHorizontal, isButtonPressed[0], isButtonPressed[1], isButtonPressed[2], isButtonPressed[3], isButtonPressed[4],
      isButtonPressed[5], isButtonPressed[6], isButtonPressed[7]);
}

MouseReport decodeMouseInputReport(const std::uint8_t* rawReport, ReportItemList* inputReportItemList) {
  MouseReport mouseReport;
  for (auto item : inputReportItemList->getItems()) {
    auto usagePage = item->getUsagePage();
    auto reportSize = item->getReportSize();
    // X, Y and Wheel are in the Generic Desktop usage page
    if (usagePage == static_cast<usagePage_t>(UsagePage::GENERIC_DESKTOP)) {
      // loop for usageIDs
      auto bitOffset = item->getBitOffset();
      for (auto usageID : item->getUsageIDs()) {
        if (usageID == static_cast<usageID_t>(UsageIDGenericDesktop::X)) {
          mouseReport.x = extractBitsSigned(rawReport, bitOffset, reportSize);
        } else if (usageID == static_cast<usageID_t>(UsageIDGenericDesktop::Y)) {
          mouseReport.y = extractBitsSigned(rawReport, bitOffset, reportSize);
        } else if (usageID == static_cast<usageID_t>(UsageIDGenericDesktop::WHEEL)) {
          mouseReport.wheelVertical = extractBitsSigned(rawReport, bitOffset, reportSize);
        }
        bitOffset += reportSize;
      }
      continue;
    }
    // Horizontal wheel is in the Consumer usage page (oddly enough)
    if (usagePage == static_cast<usagePage_t>(UsagePage::CONSUMER)) {
      auto bitOffset = item->getBitOffset();
      for (auto usageID : item->getUsageIDs()) {
        if (usageID == static_cast<usageID_t>(UsageIDConsumer::AC_PAN)) {
          mouseReport.wheelHorizontal = extractBitsSigned(rawReport, bitOffset, reportSize);
        }
        bitOffset += reportSize;
      }
      continue;
    }
    // Buttons are in the Button usage page
    if (usagePage == static_cast<usagePage_t>(UsagePage::BUTTON)) {
      auto isBitmap = item->getReportSize() == 1 && item->getReportCount() > 1;
      if (!isBitmap) {
        // Only support bitmap for now
        continue;
      }
      if (item->getUsageIDs().empty()) {
        // Ignore buttons without usageIDs (e.g. padding)
        continue;
      }
      auto firstUsageID = item->getUsageIDs()[0];
      auto data = extractBitsUnsigned(rawReport, item->getBitOffset(), reportSize);
      for (size_t i = 0; i < item->getReportCount(); i++) {
        auto buttonIndex = firstUsageID + i - 1;
        if (buttonIndex >= 0 && buttonIndex < 8) {
          mouseReport.isButtonPressed[buttonIndex] = data & (1 << i);
        }
      }
      continue;
    }
  }
  return mouseReport;
}
