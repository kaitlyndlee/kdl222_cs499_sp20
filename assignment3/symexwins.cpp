#include <deepstate/DeepState.hpp>

#include <stdio.h>
#include <string.h>
using namespace deepstate;


int checkValues(int num1, int num2) {
  if(num1 == 0 || num2 == 0) {
     return 0;
  }
  return (5 * num1 + 3 == num2);
}


TEST(Symexwins, StrTest) {
  int num1 = DeepState_Int();
  int num2 = DeepState_Int();
  LOG(TRACE) << "Numbers: " << num1 << ", " << num2;
  ASSERT_FALSE(checkValues(num1, num2));
}
