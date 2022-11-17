#include <iostream>
#include <ctime>
#include "Matrix.h"

int main(void) {
  // set cout precision
  std::cout.precision(6);

  // define some matrices
  int dimension = 1000;

  Matrix* a = new Matrix(dimension); // elements to multiply
  Matrix* b = new Matrix(dimension); // ...
  Matrix* c = new Matrix(dimension); // result
  
  // init the matrices with random values
  for(int row=0; row<dimension; row++) {
    for(int col=0; col<dimension; col++) {
      // random calues from  0.0-1.0
      double va = (double)rand() / (double)RAND_MAX;
      double vb = (double)rand() / (double)RAND_MAX;
      a->Set(col, row, va);
      b->Set(col, row, vb);
    }
  }

  for(int run=0; run<10; run++) {
    long start_time = clock();
    Matrix::MUL(*c, *a, *b);
    long finish_time = clock();

    std::cout << "[+] Product computed in " << (double) (finish_time - start_time) / CLOCKS_PER_SEC << "s" << std::endl;
  }
  std::cout << "[>] Output test: " << std::endl;
  for(int row=0; row<10; row++) {
    for(int col=0; col<10; col++) {
      std::cout << " " << c->GetCol(col, row);

    }
    std::cout << std::endl;
  }
  std::cout << "[>] Return." << std::endl;
  return 0;
}
