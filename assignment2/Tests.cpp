// #include <deepstate/DeepState.hpp>

// using namespace deepstate;

#include "openpose/utilities/string.hpp"
#include <iostream>

using namespace op;
// int main() {
  
//   std::string input("tester12");
//   unsigned long num = op::getLastNumber(input);
//   std::cout<< num<< std::endl;
//   return num;
// }

TEST(Test, Stuff) {
  char* inputChar = DeepState_CStrUpToLen(30);
  String inputString(inputChar);
  unsigned long num = op::getLastNumber(input);
}