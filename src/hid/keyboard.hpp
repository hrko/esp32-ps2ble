#ifndef A3258D52_3303_43EF_8FDE_4621626B6EEF
#define A3258D52_3303_43EF_8FDE_4621626B6EEF

#include "report_map.hpp"

class KeyboardPressedKeys {
 private:
  std::vector<usageID_t> pressedKeys;

 public:
  void add(usageID_t usageID);
  void remove(usageID_t usageID);
  std::vector<usageID_t> get();
};

KeyboardPressedKeys decodeKeyboardInputReport(const std::uint8_t* report, ReportItemList* inputReportItemList);

#endif /* A3258D52_3303_43EF_8FDE_4621626B6EEF */
