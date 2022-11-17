#include "Matrix.h"

void Matrix::Set(int col, int row, double v) {
  data[col + row * dimension] = v;
}

double Matrix::Get(int col, int row) {
  return data[col + row * dimension];
}

void Matrix::MOV(Matrix& dest, Matrix& src) {
  for(int i=0; i < dest.dimension * dest.dimension; i++) dest.data[i] = src.data[i];
}

void Matrix::MUL(Matrix& dest, Matrix& src1, Matrix& src2) {
  int dim = dest.dimension;
  Matrix* tmp = new Matrix(dest.dimension);
  // compute the matrix product
  for(int row=0; row<dim; row++) {
    for(int col=0; col<dim; col++) {
      double result = 0.0;
      // compute the dot product of row src1 and col src2
      for (int dot=0; dot<dim; dot++) result += src1.Get(dot, row) * src2.Get(col, dot);
      tmp->Set(col, row, result);
    }
  }
  Matrix::MOV(dest, *tmp);
}


