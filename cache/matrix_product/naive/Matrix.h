class Matrix {
  double *data;
  int dimension;

  public:
  // constructor, creates square matrix sizeof(dimension*dimension)
  Matrix(int dimension) {
    this->dimension = dimension;
    data = new double[dimension * dimension];
  }

  // destructor
  ~Matrix() {
    delete[] data;
  }

  // set the value of an element
  void Set(int col, int row, double v);
  double Get(int col, int row);

  // moves the value of src into dest
  static void MOV(Matrix& dest, Matrix& src);

  // stores multiple of src in dest
  static void MUL(Matrix& dest, Matrix& src1, Matrix& src2);
};
