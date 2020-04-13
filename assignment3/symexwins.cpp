#include <deepstate/DeepState.hpp>

#include <iostream>
#include <string>

using namespace deepstate;


int isHadamard(int sizeOfMatrix, float inputMatrix[2][2]) {
  float transpose[2][2];
  for(int i = 0; i < sizeOfMatrix; i++) {                                    
    for(int j = 0; j < sizeOfMatrix; j++) {                          
      transpose[j][i] = inputMatrix[i][j];
    }
  }

  int i, j, k; 
  float result[2][2];
  for (i = 0; i < sizeOfMatrix; i++) { 
    for (j = 0; j < sizeOfMatrix; j++) { 
      result[i][j] = 0; 
      for (k = 0; k < sizeOfMatrix; k++) {
        result[i][j] += inputMatrix[i][k] * transpose[k][j]; 
      } 
    }
  } 

  return ( (result[0][0] == 1) && (result[0][1] == 0) && (result[1][0] == 0) && (result[1][1] == 1));
}

int main()

TEST(StrUtil, StrManipulation) {
  float input[2][2];
  int i,j,k;

  for(i = 0; i < 2; i++) {
    for(j = 0; j < 2; j++) {
      input[i][j] =  DeepState_IntInRange(-1, 1);;
    }
  }
  isHadamard(2, input);
  LOG(TRACE)<< printf("Input matrix: [[%f, %f], [%f, %f]]\n", input[0][0], input[0][1], input[1][0], input[1][1]);
  ASSERT_EQ(isHadamard(2, input), 1);
}
