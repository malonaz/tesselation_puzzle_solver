#ifndef APS_COMMON_SHAPE_MATRIX_H_
#define APS_COMMON_SHAPE_MATRIX_H_

typedef unsigned int uint;

/**
  Describes the shape detected in the image as an discrete logocal matrix
*/
class ShapeMatrix {
private:
  /**
    The width of the matrix
  */
  uint width;

  /** 
    The height of the matrix
  */
  uint height;

  /**
    The area that the shape occupies out of the matrix
  */
  uint shapeArea;

  /**
    The logical matrix that represent
  */
  bool* shape;

public:
  ShapeMatrix(int width, int height);

  ShapeMatrix(const ShapeMatrix &copy);

  void set(uint index, bool value);

  bool get(uint index) const;

  void set(uint row, uint col, bool value);

  bool get(uint row, uint col) const;

  uint getWidth() const;

  uint getHeight() const;

  uint getShapeArea() const;

  uint getMatrixArea() const;

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
  
  /**
    Creates a copy of the shape matrix that is mirrored row-wise.
    
    \return The pointer to the new copy of the mirroed matrix.
  */
  ShapeMatrix* mirror() const;

  ShapeMatrix& operator=(const ShapeMatrix& shape);
  
  bool operator==(const ShapeMatrix& rhs) const;
  
  bool operator!=(const ShapeMatrix& rhs) const;

  friend bool operator< (const ShapeMatrix& a, const ShapeMatrix& b);

  virtual ~ShapeMatrix();

}; // ShapeMatrix

#endif
