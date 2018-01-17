#ifndef APS_COMMON_SHAPEMATRIX_H_
#define APS_COMMON_SHAPEMATRIX_H_

typedef struct {
  int width;
  int height;
  bool shape[];
} ShapeMatrix;

#endif