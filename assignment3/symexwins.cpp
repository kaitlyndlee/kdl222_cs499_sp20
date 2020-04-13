#include <deepstate/DeepState.hpp>

#include <iostream>
#include <string>

using namespace deepstate;


int isPrime(unsigned int number) {
  int flag = 1;
  // Iterate from 2 to n/2 
  for (i = 2; i <= sqrt(n); i++) { 

    // If n is divisible by any number between 
    // 2 and n/2, it is not prime 
    if (n % i == 0) { 
      flag = 0; 
      break; 
    } 
  } 
  return flag;
}

int main()

TEST(StrUtil, StrManipulation) {
  int inputNum =DeepState_UIntInRange(0, 100000);
  LOG(TRACE)<< printf("Input number: %f", inputNum);
  ASSERT_EQ(isPrime(inputNum), 1);
}
