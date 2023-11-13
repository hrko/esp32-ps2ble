#ifndef DBF45B03_C1FB_4527_9315_D80C7852E0BE
#define DBF45B03_C1FB_4527_9315_D80C7852E0BE

#include <cstdint>

std::uint32_t extractBitsUnsigned(const std::uint8_t* array, std::uint8_t bitOffset, std::uint8_t bitSize);
std::int32_t extractBitsSigned(const std::uint8_t* array, std::uint8_t bitOffset, std::uint8_t bitSize);

#endif /* DBF45B03_C1FB_4527_9315_D80C7852E0BE */
