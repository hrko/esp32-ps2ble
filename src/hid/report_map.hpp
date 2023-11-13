#ifndef D81310A6_C4D4_4BD1_A83E_DB6CD32E413C
#define D81310A6_C4D4_4BD1_A83E_DB6CD32E413C

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "common.hpp"

using reportMapItemPrefix_t = std::uint8_t;

enum class ReportMapItemType {
  MAIN,
  GLOBAL,
  LOCAL,
};

// Each item in HID report map has a prefix byte.
// The prefix byte is composed of 3 parts:
// 0b<BIT7><BIT6><BIT5><BIT4><BIT3><BIT2><BIT1><BIT0>
// BIT7-4: Tag
// BIT3-2: Type (0b00: Main, 0b01: Global, 0b10: Local)
// BIT1-0: Size (0b00: 0, 0b01: 1, 0b10: 2, 0b11: 4)
enum class ReportMapItemPrefixBase : reportMapItemPrefix_t {
  // Main items
  INPUT_ = 0x80,
  OUTPUT_ = 0x90,
  COLLECTION = 0xa0,
  FEATURE = 0xb0,
  END_COLLECTION = 0xc0,
  // Global items
  USAGE_PAGE = 0x04,
  LOGICAL_MIN = 0x14,
  LOGICAL_MAX = 0x24,
  PHYSICAL_MIN = 0x34,
  PHYSICAL_MAX = 0x44,
  UNIT_EXPONENT = 0x54,
  UNIT = 0x64,
  REPORT_SIZE = 0x74,
  REPORT_ID = 0x84,
  REPORT_COUNT = 0x94,
  PUSH = 0xa4,
  POP = 0xb4,
  // Local items
  USAGE = 0x08,
  USAGE_MIN = 0x18,
  USAGE_MAX = 0x28,
  DESIGNATOR_INDEX = 0x38,
  DESIGNATOR_MIN = 0x48,
  DESIGNATOR_MAX = 0x58,
  STRING_INDEX = 0x78,
  STRING_MIN = 0x88,
  STRING_MAX = 0x98,
  DELIMITER = 0xa8,
  // _LONG_ITEM = 0xfe,
  // _INVALID = 0xff,
};

enum class ReportMapItemPrefixType : reportMapItemPrefix_t {
  MAIN = 0x00,
  GLOBAL = 0x04,
  LOCAL = 0x08,
};

class ReportItem {
 private:
  usagePage_t usagePage;
  std::vector<usageID_t> usageIDs;
  std::uint32_t reportSize;
  std::uint32_t reportCount;
  std::int32_t logicalMin;
  std::int32_t logicalMax;
  // these are generated from the reportSize and reportCount
  std::uint32_t bitOffset = 0;
  std::uint32_t bitLength = 0;

 public:
  ReportItem(usagePage_t usagePage, std::vector<usageID_t> usageIDs, std::uint32_t reportSize, std::uint32_t reportCount,
             std::int32_t logicalMin, std::int32_t logicalMax);
  usagePage_t getUsagePage();
  std::vector<usageID_t> getUsageIDs();
  std::uint32_t getReportSize();
  std::uint32_t getReportCount();
  std::int32_t getLogicalMin();
  std::int32_t getLogicalMax();
  std::uint32_t getBitOffset();
  void setBitOffset(std::uint32_t bitOffset);
  std::uint32_t getBitLength();
  void setBitLength(std::uint32_t bitLength);
  std::string toString();
};

class ReportItemList {
 public:
  enum class ReportType {
    INPUT_TYPE,
    OUTPUT_TYPE,
    FEATURE_TYPE,
  };

 private:
  std::vector<ReportItem*> items;
  usagePage_t usagePage;
  usageID_t usageID;
  ReportType reportType;
  reportID_t reportID;

 public:
  ReportItemList(usagePage_t usagePage, usageID_t usageID, ReportType reportType, reportID_t reportID);
  ~ReportItemList();
  void addItem(ReportItem* item);
  std::vector<ReportItem*> getItems();
  usagePage_t getUsagePage();
  usageID_t getUsageID();
  ReportType getReportType();
  std::uint8_t getReportID();
  void setReportID(reportID_t reportID);
  std::string toString();
};

ReportMapItemPrefixBase getReportMapItemPrefixBase(reportMapItemPrefix_t prefix);
ReportMapItemPrefixType getReportMapItemType(reportMapItemPrefix_t prefix);
std::uint8_t getReportMapItemSize(reportMapItemPrefix_t prefix);

class ReportMap {
 private:
  std::unordered_map<reportID_t, ReportItemList*> input;
  std::unordered_map<reportID_t, ReportItemList*> output;
  std::unordered_map<reportID_t, ReportItemList*> feature;

 public:
  void addItemList(ReportItemList* itemList);

  ReportMap(const std::uint8_t* raw_map, std::size_t raw_map_len);
  ~ReportMap();
  ReportItemList* getInputReportItemList(reportID_t reportID);
  ReportItemList* getOutputReportItemList(reportID_t reportID);
  ReportItemList* getFeatureReportItemList(reportID_t reportID);
  ReportItemList* getItemList(ReportItemList::ReportType reportType, reportID_t reportID);
  std::unordered_map<reportID_t, ReportItemList*> getInputReportItemLists();
  std::unordered_map<reportID_t, ReportItemList*> getOutputReportItemLists();
  std::unordered_map<reportID_t, ReportItemList*> getFeatureReportItemLists();
  std::string toString();
};

#endif /* D81310A6_C4D4_4BD1_A83E_DB6CD32E413C */
