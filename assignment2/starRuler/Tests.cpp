#include <deepstate/DeepState.hpp>

using namespace deepstate;

#include "str_util.h"
#include <iostream>


TEST(Test, toLower_toUpper) {
  char* inputChar = DeepState_CStrUpToLen(30);

  std::string originalStr(inputChar);
  std::string inputStr = originalStr;
  toLowercase(inputStr);
  ASSERT_STREQ(toUppercase(inputStr), originalStr);
}