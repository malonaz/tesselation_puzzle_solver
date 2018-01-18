#ifndef APS_COMMON_SHAPE_MATRIX_H_
#define APS_COMMON_SHAPE_MATRIX_H_

class ShapeMatrix {
private:
  int width;
  int height;
  bool* shape;
  
public:
  ShapeMatrix(int width, int height);
  
  void set(int index, bool value);
  
  bool get(int index);
  
  void set(int row, int col, bool value);
  
  bool get(int row, int col);
  
  virtual ~ShapeMatrix();
};

#endif