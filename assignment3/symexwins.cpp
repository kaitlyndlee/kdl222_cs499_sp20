#include <deepstate/DeepState.hpp>

#include <iostream>
#include <string>

using namespace deepstate;

float **gauss_jordan(int sizeOfMatrix, float **input, float **inverse) {
    float localVariable;
    int i,j,k;

    for(i=0;i<sizeOfMatrix;i++) {                                    
      for(j=0;j<sizeOfMatrix;j++) {                          
        if(i==j) {                                      
          inverse[i][j]=1;  
        }                                  
        else {                                           
           inverse[i][j]=0;
        }                               
      }
    } 


    for(k=0;k<sizeOfMatrix;k++) {                                                        
        localVariable=input[k][k];                                        
        for(j=0;j<sizeOfMatrix;j++) {
            input[k][j]/=localVariable;                                    
            inverse[k][j]/=localVariable;                                    

        }                                                    
        for(i=0;i<sizeOfMatrix;i++) {
            localVariable = input[i][k];                                    
            for(j=0;j<sizeOfMatrix;j++) {                                                
                if(i==k)
                    break;                                    
                input[i][j] -= input[k][j]*localVariable;                        
                inverse[i][j] -= inverse[k][j]*localVariable;                        
            }
        }

    printf("\nThe solution is:\n");
    for(i=1; i<=n; i++)
    {
        x[i]=A[i][n+1]/A[i][i];
        printf("\n x%d=%f\n",i,x[i]);
    }
  }
}




TEST(StrUtil, StrManipulation) {
  float **input;
  float **inverse;
  int i,j,k;
  int sizeOfMatrix = 4;

  input = (float **)malloc(sizeOfMatrix*sizeof(float *)); 
  for(i=0;i<sizeOfMatrix;i++) {
    input[i]=(float *)malloc(sizeOfMatrix*sizeof(float));
  }

  inverse=(float **)malloc(sizeOfMatrix*sizeof(float *));        
  for(i=0;i<sizeOfMatrix;i++) {

    inverse[i]=(float *)malloc(sizeOfMatrix*sizeof(float));
  }

  for(i=0;i<sizeOfMatrix;i++) {
    for(j=0;j<sizeOfMatrix;j++) {
      input[i][j] = DeepState_UIntInRange(1, 10000);
    }
  }
  gauss_jordan(sizeOfMatrix, input, inverse);


  // ASSERT_TRUE(isLower(originalStr, inputStr));
}
