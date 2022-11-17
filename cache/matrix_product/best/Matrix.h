class Matrix {
  // define it in ram twice
  double *data_row;
  double *data_col; // double ram foootprint but optimising our data layout
  int dimension;

  public:
  // constructor, creates square matrix sizeof(dimension*dimension)
  Matrix(int dimension) {
    this->dimension = dimension;
    data_col = new double[dimension * dimension];
    data_row = new double[dimension * dimension];
  }

  // destructor
  ~Matrix() {
    delete[] data_col;
    delete[] data_row;
  }

  // set the value of an element
  void Set(int col, int row, double v);
  double GetCol(int col, int row);
  double GetRow(int row, int col);

  // moves the value of src into dest
  static void MOV(Matrix& dest, Matrix& src);

  // stores multiple of src in dest
  static void MUL(Matrix& dest, Matrix& src1, Matrix& src2);
};
