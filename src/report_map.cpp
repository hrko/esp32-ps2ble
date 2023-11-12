#include "report_map.hpp"

#include <fmt/core.h>

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

// ReportItemList functions

ReportItemList::ReportItemList(usagePage_t usagePage, usageID_t usageID, ReportType reportType, reportID_t reportID)
    : usagePage(usagePage), usageID(usageID), reportType(reportType), reportID(reportID) {}
ReportItemList::~ReportItemList() {
  for (auto item : items) {
    delete item;
  }
}
void ReportItemList::addItem(ReportItem* item) { items.push_back(item); }
std::vector<ReportItem*> ReportItemList::getItems() { return items; }
ReportItemList::ReportType ReportItemList::getReportType() { return reportType; }
std::uint8_t ReportItemList::getReportID() { return reportID; }
void ReportItemList::setReportID(reportID_t reportID) { this->reportID = reportID; }
std::string ReportItemList::toString() {
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
  ReportItemList* itemListInput = nullptr;
  ReportItemList* itemListOutput = nullptr;
  ReportItemList* itemListFeature = nullptr;

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
          itemListInput = new ReportItemList(collectionProperty.usagePage, collectionProperty.usageID, ReportItemList::ReportType::INPUT_TYPE, 0);
          itemListOutput =
              new ReportItemList(collectionProperty.usagePage, collectionProperty.usageID, ReportItemList::ReportType::OUTPUT_TYPE, 0);
          itemListFeature =
              new ReportItemList(collectionProperty.usagePage, collectionProperty.usageID, ReportItemList::ReportType::FEATURE_TYPE, 0);
        }
        break;
      }

      case ReportMapItemPrefixBase::END_COLLECTION: {
        auto collectionProperty = collectionStack.back();
        collectionStack.pop_back();
        if (collectionProperty.isApplicationCollection()) {
          itemListInput->setReportID(globalItemState.reportID);
          itemListOutput->setReportID(globalItemState.reportID);
          itemListFeature->setReportID(globalItemState.reportID);
          addItemList(itemListInput);
          addItemList(itemListOutput);
          addItemList(itemListFeature);
        }
        break;
      }

      case ReportMapItemPrefixBase::INPUT_:
        if (itemListInput != nullptr) {
          auto reportItem = new ReportItem(globalItemState.usagePage, localItemState.usageIDs, globalItemState.reportSize,
                                           globalItemState.reportCount, globalItemState.logicalMin, globalItemState.logicalMax);
          itemListInput->addItem(reportItem);
        }
        break;

      case ReportMapItemPrefixBase::OUTPUT_:
        if (itemListOutput != nullptr) {
          auto reportItem = new ReportItem(globalItemState.usagePage, localItemState.usageIDs, globalItemState.reportSize,
                                           globalItemState.reportCount, globalItemState.logicalMin, globalItemState.logicalMax);
          itemListOutput->addItem(reportItem);
        }
        break;

      case ReportMapItemPrefixBase::FEATURE:
        if (itemListFeature != nullptr) {
          auto reportItem = new ReportItem(globalItemState.usagePage, localItemState.usageIDs, globalItemState.reportSize,
                                           globalItemState.reportCount, globalItemState.logicalMin, globalItemState.logicalMax);
          itemListFeature->addItem(reportItem);
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
  for (auto itemList : input) {
    delete itemList.second;
  }
  for (auto itemList : output) {
    delete itemList.second;
  }
  for (auto itemList : feature) {
    delete itemList.second;
  }
}

void ReportMap::addItemList(ReportItemList* itemList) {
  switch (itemList->getReportType()) {
    case ReportItemList::ReportType::INPUT_TYPE:
      input[itemList->getReportID()] = itemList;
      break;
    case ReportItemList::ReportType::OUTPUT_TYPE:
      output[itemList->getReportID()] = itemList;
      break;
    case ReportItemList::ReportType::FEATURE_TYPE:
      feature[itemList->getReportID()] = itemList;
      break;
  }
}

ReportItemList* ReportMap::getInputItemList(reportID_t reportID) { return input[reportID]; }

ReportItemList* ReportMap::getOutputItemList(reportID_t reportID) { return output[reportID]; }

ReportItemList* ReportMap::getFeatureItemList(reportID_t reportID) { return feature[reportID]; }

ReportItemList* ReportMap::getItemList(ReportItemList::ReportType reportType, reportID_t reportID) {
  switch (reportType) {
    case ReportItemList::ReportType::INPUT_TYPE:
      return getInputItemList(reportID);
    case ReportItemList::ReportType::OUTPUT_TYPE:
      return getOutputItemList(reportID);
    case ReportItemList::ReportType::FEATURE_TYPE:
      return getFeatureItemList(reportID);
    default:
      return nullptr;
  }
}

std::string ReportMap::toString() {
  std::string inputItemListsStr;
  for (auto itemList : input) {
    inputItemListsStr += "\n" + itemList.second->toString();
  }
  std::string outputItemListsStr;
  for (auto itemList : output) {
    outputItemListsStr += "\n" + itemList.second->toString();
  }
  std::string featureItemListsStr;
  for (auto itemList : feature) {
    featureItemListsStr += "\n" + itemList.second->toString();
  }
  return fmt::format("InputItemLists:{}\nOutputItemLists:{}\nFeatureItemLists:{}", inputItemListsStr,
                     outputItemListsStr, featureItemListsStr);
}
