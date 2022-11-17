#include "Matrix.h"

void Matrix::Set(int col, int row, double v) {
  data_row[col + row * dimension] = v;
  data_col[row + col * dimension] = v; // make 90 degree copy
}

double Matrix::GetRow(int col, int row) {
  return data_row[row + col * dimension];
}

double Matrix::GetCol(int col, int row) {
  return data_col[col + row * dimension];
}

void Matrix::MOV(Matrix& dest, Matrix& src) {
  for(int i=0; i < dest.dimension * dest.dimension; i++) {
    dest.data_col[i] = src.data_col[i];
    dest.data_row[i] = src.data_row[i];
  }
}

void Matrix::MUL(Matrix& dest, Matrix& src1, Matrix& src2) {
  int dim = dest.dimension;
  Matrix* tmp = new Matrix(dest.dimension);
  // compute the matrix product
  for(int row=0; row<dim; row++) {
    for(int col=0; col<dim; col++) {
      double result = 0.0;

      // compute the dot product of row srxcc1 and col src2
      for (int dot=0; dot<dim; dot++) result += src1.GetCol(dot, row) * src2.GetRow(col, dot);
      tmp->Set(col, row, result);
    }
  }
  Matrix::MOV(dest, *tmp);
}


