#ifndef APS_COMMON_SHAPE_MATRIX_H_
#define APS_COMMON_SHAPE_MATRIX_H_

/*
  ShapeMatrix

  describes the shape detected in the image as an discrete logocal matrix
*/
class ShapeMatrix {
private:
  int width;
  int height;
  int shapeArea;
  bool* shape;

public:
  ShapeMatrix(int width, int height);

  void set(int index, bool value);

  bool get(int index);

  void set(int row, int col, bool value);

  bool get(int row, int col);

  int getWidth();

  int getHeight();

  int getShapeArea();

  int getMatrixArea();

  virtual ~ShapeMatrix();
};

#endif
