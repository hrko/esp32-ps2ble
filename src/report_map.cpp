// clang-format off
#include <fmt/core.h>
// clang-format on

#include "report_map.hpp"

#include <NimBLEDevice.h>

#include <cstdint>
#include <unordered_map>
#include <vector>

// global functions

ReportMapItemPrefixBase getReportMapItemPrefixBase(reportMapItemPrefix_t prefix) {
  prefix = prefix & 0xfc;  // 0b11111100
  return static_cast<ReportMapItemPrefixBase>(prefix);
}

ReportMapItemPrefixType getReportMapItemType(reportMapItemPrefix_t prefix) {
  prefix = prefix & 0x0c;  // 0b00001100
  return static_cast<ReportMapItemPrefixType>(prefix);
}

std::uint8_t getReportMapItemSize(reportMapItemPrefix_t prefix) {
  switch (prefix & 0x03) {  // 0b00000011
    case 0x00:
      return 0;
    case 0x01:
      return 1;
    case 0x02:
      return 2;
    case 0x03:
      return 4;
    default:  // should never happen, but prevents compiler warning
      return 0;
  }
}

// ReportItem functions

ReportItem::ReportItem(usagePage_t usagePage, std::vector<usageID_t> usageIDs, std::uint32_t reportSize, std::uint32_t reportCount,
                       std::int32_t logicalMin, std::int32_t logicalMax)
    : usagePage(usagePage),
      usageIDs(usageIDs),
      reportSize(reportSize),
      reportCount(reportCount),
      logicalMin(logicalMin),
      logicalMax(logicalMax) {}

std::string ReportItem::toString() {
  std::string usageIDsStr;
  for (auto usageID : usageIDs) {
    usageIDsStr += fmt::format("0x{:04x}, ", usageID);
  }
  return fmt::format("UsagePage: 0x{:04x}, UsageIDs: [{}], ReportSize: {}, ReportCount: {}, LogicalMin: {}, LogicalMax: {}", usagePage,
                     usageIDsStr, reportSize, reportCount, logicalMin, logicalMax);
}

// ReportFormat functions

ReportFormat::ReportFormat(usagePage_t usagePage, usageID_t usageID, ReportType reportType, reportID_t reportID)
    : usagePage(usagePage), usageID(usageID), reportType(reportType), reportID(reportID) {}
ReportFormat::~ReportFormat() {
  for (auto item : items) {
    delete item;
  }
}
void ReportFormat::addItem(ReportItem* item) { items.push_back(item); }
std::vector<ReportItem*> ReportFormat::getItems() { return items; }
ReportFormat::ReportType ReportFormat::getReportType() { return reportType; }
std::uint8_t ReportFormat::getReportID() { return reportID; }
void ReportFormat::setReportID(reportID_t reportID) { this->reportID = reportID; }
std::string ReportFormat::toString() {
  std::string reportTypeStr;
  switch (reportType) {
    case ReportType::INPUT_TYPE:
      reportTypeStr = "Input";
      break;
    case ReportType::OUTPUT_TYPE:
      reportTypeStr = "Output";
      break;
    case ReportType::FEATURE_TYPE:
      reportTypeStr = "Feature";
      break;
  }
  std::string itemsStr;
  for (auto item : items) {
    itemsStr += "\n" + item->toString();
  }
  return fmt::format("ReportType: {}, ReportID: {}, UsagePage: 0x{:04x}, UsageID: 0x{:04x}{}", reportTypeStr, reportID, usagePage, usageID,
                     itemsStr);
}

// ReportMap functions

ReportMap::ReportMap(const std::uint8_t* raw_map, std::size_t raw_map_len) {
  class GlobalItemState {
   public:
    // some global items are omitted since they are not mandatory
    usagePage_t usagePage;
    std::uint8_t reportSize;
    std::uint8_t reportCount;
    std::int32_t logicalMin;
    std::int32_t logicalMax;
    reportID_t reportID;
  };
  class LocalItemState {
   public:
    std::vector<usageID_t> usageIDs;
  };
  class CollectionProperty {
   public:
    usagePage_t usagePage;
    usageID_t usageID;
    std::uint8_t collectionType;
    bool isApplicationCollection() { return collectionType == 0x01; }
  };
  GlobalItemState globalItemState;
  LocalItemState localItemState;
  std::vector<GlobalItemState> globalItemStateStack;  // only used for PUSH and POP
  std::vector<CollectionProperty> collectionStack;
  ReportFormat* formatInput = nullptr;
  ReportFormat* formatOutput = nullptr;
  ReportFormat* formatFeature = nullptr;

  // parse the raw report map
  for (size_t i = 0; i < raw_map_len;) {
    // read the prefix
    auto prefix = raw_map[i];
    auto prefixBase = getReportMapItemPrefixBase(prefix);
    auto itemType = getReportMapItemType(prefix);
    auto itemSize = getReportMapItemSize(prefix);

    // read the item value
    std::uint32_t itemValueUnsigned = 0;
    std::int32_t itemValueSigned = 0;
    switch (itemSize) {
      case 0:
        break;
      case 1:
        std::uint8_t itemValue8;
        std::memcpy(&itemValue8, raw_map + i + 1, 1);
        itemValueUnsigned = itemValue8;
        itemValueSigned = std::int8_t(itemValue8);
        break;
      case 2:
        std::uint16_t itemValue16;
        std::memcpy(&itemValue16, raw_map + i + 1, 2);
        itemValueUnsigned = itemValue16;
        itemValueSigned = std::int16_t(itemValue16);
        break;
      case 4:
        std::uint32_t itemValue32;
        std::memcpy(&itemValue32, raw_map + i + 1, 4);
        itemValueUnsigned = itemValue32;
        itemValueSigned = std::int32_t(itemValue32);
        break;
    }

    // handle the item
    switch (prefixBase) {
      // Main items
      case ReportMapItemPrefixBase::COLLECTION: {
        usagePage_t usagePage = globalItemState.usagePage;
        usageID_t usageID = localItemState.usageIDs.size() > 0 ? localItemState.usageIDs.back() : 0;
        auto collectionType = (std::uint8_t)itemValueUnsigned;
        auto collectionProperty = CollectionProperty{usagePage, usageID, collectionType};
        collectionStack.push_back(collectionProperty);
        if (collectionProperty.isApplicationCollection()) {
          formatInput = new ReportFormat(collectionProperty.usagePage, collectionProperty.usageID, ReportFormat::ReportType::INPUT_TYPE, 0);
          formatOutput =
              new ReportFormat(collectionProperty.usagePage, collectionProperty.usageID, ReportFormat::ReportType::OUTPUT_TYPE, 0);
          formatFeature =
              new ReportFormat(collectionProperty.usagePage, collectionProperty.usageID, ReportFormat::ReportType::FEATURE_TYPE, 0);
        }
        break;
      }

      case ReportMapItemPrefixBase::END_COLLECTION: {
        auto collectionProperty = collectionStack.back();
        collectionStack.pop_back();
        if (collectionProperty.isApplicationCollection()) {
          formatInput->setReportID(globalItemState.reportID);
          formatOutput->setReportID(globalItemState.reportID);
          formatFeature->setReportID(globalItemState.reportID);
          addFormat(formatInput);
          addFormat(formatOutput);
          addFormat(formatFeature);
        }
        break;
      }

      case ReportMapItemPrefixBase::INPUT_:
        if (formatInput != nullptr) {
          auto reportItem = new ReportItem(globalItemState.usagePage, localItemState.usageIDs, globalItemState.reportSize,
                                           globalItemState.reportCount, globalItemState.logicalMin, globalItemState.logicalMax);
          formatInput->addItem(reportItem);
        }
        break;

      case ReportMapItemPrefixBase::OUTPUT_:
        if (formatOutput != nullptr) {
          auto reportItem = new ReportItem(globalItemState.usagePage, localItemState.usageIDs, globalItemState.reportSize,
                                           globalItemState.reportCount, globalItemState.logicalMin, globalItemState.logicalMax);
          formatOutput->addItem(reportItem);
        }
        break;

      case ReportMapItemPrefixBase::FEATURE:
        if (formatFeature != nullptr) {
          auto reportItem = new ReportItem(globalItemState.usagePage, localItemState.usageIDs, globalItemState.reportSize,
                                           globalItemState.reportCount, globalItemState.logicalMin, globalItemState.logicalMax);
          formatFeature->addItem(reportItem);
        }
        break;

      // Global items
      case ReportMapItemPrefixBase::USAGE_PAGE:
        globalItemState.usagePage = itemValueUnsigned;
        break;
      case ReportMapItemPrefixBase::REPORT_SIZE:
        globalItemState.reportSize = itemValueUnsigned;
        break;
      case ReportMapItemPrefixBase::REPORT_COUNT:
        globalItemState.reportCount = itemValueUnsigned;
        break;
      case ReportMapItemPrefixBase::LOGICAL_MIN:
        globalItemState.logicalMin = itemValueSigned;
        break;
      case ReportMapItemPrefixBase::LOGICAL_MAX:
        globalItemState.logicalMax = itemValueSigned;
        break;
      case ReportMapItemPrefixBase::REPORT_ID:
        globalItemState.reportID = itemValueUnsigned;
        break;
      case ReportMapItemPrefixBase::PUSH:
        globalItemStateStack.push_back(globalItemState);
        break;
      case ReportMapItemPrefixBase::POP:
        globalItemState = globalItemStateStack.back();
        globalItemStateStack.pop_back();
        break;

      // Local items
      case ReportMapItemPrefixBase::USAGE:
      case ReportMapItemPrefixBase::USAGE_MIN:
        localItemState.usageIDs.push_back(itemValueUnsigned);
        break;
      case ReportMapItemPrefixBase::USAGE_MAX:
        // add all usageIDs between USAGE_MIN and USAGE_MAX
        for (auto usageID = localItemState.usageIDs.back() + 1; usageID <= itemValueUnsigned; usageID++) {
          localItemState.usageIDs.push_back(usageID);
        }
        break;
    }

    // if main item is found, clear the local item state
    if (itemType == ReportMapItemPrefixType::MAIN) {
      localItemState.usageIDs.clear();
    }

    // seek to the next item
    i += 1 + itemSize;
  }
}

ReportMap::~ReportMap() {
  for (auto format : inputReportFormats) {
    delete format.second;
  }
  for (auto format : outputReportFormats) {
    delete format.second;
  }
  for (auto format : featureReportFormats) {
    delete format.second;
  }
}

void ReportMap::addFormat(ReportFormat* format) {
  switch (format->getReportType()) {
    case ReportFormat::ReportType::INPUT_TYPE:
      inputReportFormats[format->getReportID()] = format;
      break;
    case ReportFormat::ReportType::OUTPUT_TYPE:
      outputReportFormats[format->getReportID()] = format;
      break;
    case ReportFormat::ReportType::FEATURE_TYPE:
      featureReportFormats[format->getReportID()] = format;
      break;
  }
}

ReportFormat* ReportMap::getInputReportFormat(reportID_t reportID) { return inputReportFormats[reportID]; }

ReportFormat* ReportMap::getOutputReportFormat(reportID_t reportID) { return outputReportFormats[reportID]; }

ReportFormat* ReportMap::getFeatureReportFormat(reportID_t reportID) { return featureReportFormats[reportID]; }

ReportFormat* ReportMap::getReportFormat(ReportFormat::ReportType reportType, reportID_t reportID) {
  switch (reportType) {
    case ReportFormat::ReportType::INPUT_TYPE:
      return getInputReportFormat(reportID);
    case ReportFormat::ReportType::OUTPUT_TYPE:
      return getOutputReportFormat(reportID);
    case ReportFormat::ReportType::FEATURE_TYPE:
      return getFeatureReportFormat(reportID);
    default:
      return nullptr;
  }
}

std::string ReportMap::toString() {
  std::string inputReportFormatsStr;
  for (auto format : inputReportFormats) {
    inputReportFormatsStr += "\n" + format.second->toString();
  }
  std::string outputReportFormatsStr;
  for (auto format : outputReportFormats) {
    outputReportFormatsStr += "\n" + format.second->toString();
  }
  std::string featureReportFormatsStr;
  for (auto format : featureReportFormats) {
    featureReportFormatsStr += "\n" + format.second->toString();
  }
  return fmt::format("InputReportFormats:{}\nOutputReportFormats:{}\nFeatureReportFormats:{}", inputReportFormatsStr,
                     outputReportFormatsStr, featureReportFormatsStr);
}
