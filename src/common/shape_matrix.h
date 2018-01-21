#ifndef APS_COMMON_SHAPE_MATRIX_H_
#define APS_COMMON_SHAPE_MATRIX_H_

/*
  ShapeMatrix

  describes the shape detected in the image as an discrete logocal matrix
*/
class ShapeMatrix {
private:
  /**
    The width of the matrix
  */
  int width;

  /** 
    The height of the matrix
  */
  int height;

  /**
    The area that the shape occupies out of the matrix
  */
  int shapeArea;

  /**
    The logical matrix that represent
  */
  bool* shape;

public:
  ShapeMatrix(int width, int height);

  ShapeMatrix(const ShapeMatrix &copy);

  void set(int index, bool value);

  bool get(int index) const;

  void set(int row, int col, bool value);

  bool get(int row, int col) const;

  int getWidth() const;

  int getHeight() const;

  int getShapeArea() const;

  int getMatrixArea() const;

  /**
    Creates a new copy of the shape matrix that is rotated clockwise by
    90 degrees.
    
    \return The pointer to the new copy of the rotated matrix.
  */
  ShapeMatrix* rotate() const;

  /**
    Creates a new copy of the shape matrix that is rotated clockwise by
    n * 90 degrees.
    
    \param n The number of 90 degree transpose to rotate
    \return The pointer to the new copy of the rotated matrix.
  */
  ShapeMatrix* rotate(int n) const;

  ShapeMatrix& operator=(const ShapeMatrix& shape);

  virtual ~ShapeMatrix();

}; // ShapeMatrix

#endif
