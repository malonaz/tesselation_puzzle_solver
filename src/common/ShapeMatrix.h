#ifndef APS_COMMON_SHAPEMATRIX_H_
#define APS_COMMON_SHAPEMATRIX_H_

class ShapeMatrix {
private:
  int width;
  int height;
  bool* shape;
  
public:
  ShapeMatrix(int width, int height) {
    this->width = width;
    this->height = height;
    int area = width * height;
    this->shape = new bool[area];
  }
  
  void set(int row, int col, bool value) {
    this->shape[row * this->width + col] = value;
  }
  
  bool get(int row, int col) {
    return this->shape[row * this->width + col];
  }
  
  virtual ~ShapeMatrix() {
    delete[] this->shape;
  }
};

#endif