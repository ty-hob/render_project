#ifndef VECTOR_H
#define VECTOR_H

// 2d vector
typedef struct VECTOR2 {
  double x;
  double y;
} vector2;

// 3d vector
typedef struct VECTOR3 {
  double x;
  double y;
  double z;
} vector3;

// // 2d vector with values x=0, y=0
// extern const vector2 null2 = {0, 0};
//
// // 3d vector with values x=0, y=0, z=0
// extern const vector3 null3 = {0, 0, 0};

vector2 scale2(vector2 v, double s);

vector3 scale3(vector3 v, double s);

vector2 add2(vector2 a, vector2 b);

vector3 add3(vector3 a, vector3 b);

vector2 sub2(vector2 a, vector2 b);

vector3 sub3(vector3 a, vector3 b);

vector2 unit2(vector2 v);

vector3 unit3(vector3 v);

double length2(vector2 v);

double length3(vector3 v);

#endif
