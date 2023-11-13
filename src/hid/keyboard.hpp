#ifndef A3258D52_3303_43EF_8FDE_4621626B6EEF
#define A3258D52_3303_43EF_8FDE_4621626B6EEF

#include "report_map.hpp"

class KeyboardReport {
 private:
  std::vector<usageID_t> pressedKeys;

 public:
  void add(usageID_t usageID);
  std::vector<usageID_t> getPressedKeys();
};

KeyboardReport decodeKeyboardInputReport(const std::uint8_t* rawReport, const ReportItemList& inputReportItemList);

#endif /* A3258D52_3303_43EF_8FDE_4621626B6EEF */
