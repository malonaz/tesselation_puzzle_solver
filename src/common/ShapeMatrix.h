#ifndef APS_COMMON_SHAPEMATRIX_H_
#define APS_COMMON_SHAPEMATRIX_H_

class ShapeMatrix {
private:
  int width;
  int height;
  bool* shape;
  
public:
  ShapeMatrix(int width, int height);
  
  void set(int row, int col, bool value);
  
  bool get(int row, int col);
  
  virtual ~ShapeMatrix();
};

#endif