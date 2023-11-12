#ifndef D81310A6_C4D4_4BD1_A83E_DB6CD32E413C
#define D81310A6_C4D4_4BD1_A83E_DB6CD32E413C

#include <cstdint>
#include <unordered_map>
#include <vector>

using usagePage_t = std::uint16_t;
using usageID_t = std::uint16_t;
using reportID_t = std::uint8_t;
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

 public:
  ReportItem(usagePage_t usagePage, std::vector<usageID_t> usageIDs, std::uint32_t reportSize, std::uint32_t reportCount,
             std::int32_t logicalMin, std::int32_t logicalMax);
  std::string toString();
};

class ReportFormat {
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
  ReportFormat(usagePage_t usagePage, usageID_t usageID, ReportType reportType, reportID_t reportID);
  ~ReportFormat();
  void addItem(ReportItem* item);
  std::vector<ReportItem*> getItems();
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
  std::unordered_map<reportID_t, ReportFormat*> inputReportFormats;
  std::unordered_map<reportID_t, ReportFormat*> outputReportFormats;
  std::unordered_map<reportID_t, ReportFormat*> featureReportFormats;

 public:
  void addFormat(ReportFormat* format);

  ReportMap(const std::uint8_t* raw_map, std::size_t raw_map_len);
  ~ReportMap();
  ReportFormat* getInputReportFormat(reportID_t reportID);
  ReportFormat* getOutputReportFormat(reportID_t reportID);
  ReportFormat* getFeatureReportFormat(reportID_t reportID);
  ReportFormat* getReportFormat(ReportFormat::ReportType reportType, reportID_t reportID);
  std::string toString();
};

#endif /* D81310A6_C4D4_4BD1_A83E_DB6CD32E413C */
