#include <stdio.h>

#include "cfunction.h"

int main() {
  int size = 1024;
  unsigned char buffer[size];
  for(int i = 0; i < size; i++) {
    buffer[i] = (unsigned char) getchar();
    printf("buffer[%u] = %u\n", i, buffer[i]);
  }
  is_binary(buffer);
}