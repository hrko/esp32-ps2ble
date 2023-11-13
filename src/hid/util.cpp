#include "util.hpp"

// Function to get value from byte array.
std::uint32_t extractBitsUnsigned(const std::uint8_t* array, std::uint8_t bitOffset, std::uint8_t bitSize) {
  std::uint32_t result = 0;
  std::uint8_t bitIdx = 0;
  std::uint8_t byteIdx = bitOffset / 8;
  std::uint8_t bitIdxInByte = bitOffset % 8;
  while (bitIdx < bitSize) {
    // Extract current byte
    std::uint8_t byte = array[byteIdx];
    // Extract bits from current byte
    while (bitIdxInByte < 8 && bitIdx < bitSize) {
      if (byte & (1 << bitIdxInByte)) {
        result |= (1 << bitIdx);
      }
      // Move to next bit
      bitIdx++;
      bitIdxInByte++;
    }
    // Move to next byte
    byteIdx++;
    bitIdxInByte = 0;
  }
  return result;
}

// Function to get value from byte array, signed version.
std::int32_t extractBitsSigned(const std::uint8_t* array, std::uint8_t bitOffset, std::uint8_t bitSize) {
  std::int32_t result = 0;
  std::uint32_t unsignedResult = extractBitsUnsigned(array, bitOffset, bitSize);
  bool isNegative = unsignedResult & (1 << (bitSize - 1));
  if (isNegative) {
    // Set all bits after bitSize to 1
    unsignedResult |= (0xFFFFFFFF << bitSize);
  }
  result = unsignedResult;  // If variables are the same size, they are copied bit by bit as is.
  return result;
}
