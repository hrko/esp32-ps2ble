#ifndef E8C27331_0DFE_481C_878A_FAEC7EA8B6B9
#define E8C27331_0DFE_481C_878A_FAEC7EA8B6B9

#include <cstdint>

#include "report_map.hpp"

class MouseReport {
 public:
  bool isButtonPressed[8] = {false};
  std::int32_t x = 0;
  std::int32_t y = 0;
  std::int32_t wheelVertical = 0;
  std::int32_t wheelHorizontal = 0;
  std::string toString();
};

MouseReport decodeMouseInputReport(const std::uint8_t* rawReport, const ReportItemList& inputReportItemList);

#endif /* E8C27331_0DFE_481C_878A_FAEC7EA8B6B9 */
