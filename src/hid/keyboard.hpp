#ifndef A3258D52_3303_43EF_8FDE_4621626B6EEF
#define A3258D52_3303_43EF_8FDE_4621626B6EEF

#include "report_map.hpp"

class KeyboardReport {
 private:
  std::vector<usageID_t> pressedKeys;
  usagePage_t usagePage;

 public:
  void add(usageID_t usageID);
  std::vector<usageID_t> getPressedKeys();
  usagePage_t getUsagePage();
  void setUsagePage(usagePage_t usagePage);
  std::string toString();
};

KeyboardReport decodeKeyboardInputReport(const std::uint8_t* rawReport, const ReportItemList& inputReportItemList);

#endif /* A3258D52_3303_43EF_8FDE_4621626B6EEF */
