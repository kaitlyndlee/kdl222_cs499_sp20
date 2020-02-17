#include <stddef.h>
#include <stdint.h>

#include "cfunction.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size >= 1024) {
    is_binary((char *) Data);
  }
  return 0;  // Non-zero return values are reserved for future use.
}