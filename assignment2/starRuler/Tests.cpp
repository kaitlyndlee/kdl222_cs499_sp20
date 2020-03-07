#include <deepstate/DeepState.hpp>

#include "str_util.h"

#include <iostream>
#include <string>

using namespace deepstate;

TEST(Test, toLower_toUpper) {
  char* inputChar = DeepState_CStrUpToLen(30);

  std::string originalStr(inputChar);
  std::string inputStr = originalStr;
  toLowercase(inputStr);

for (std::string::size_type i = 0; i < originalStr.length(); ++i) {
    std::cout << std::tolower(originalStr[i]);
}
ASSERT_TRUE(originalStr == inputStr);
  // ASSERT_STREQ(toUppercase(inputStr), originalStr);
  ASSERT_EQ(1, 1);
}
