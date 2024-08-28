#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

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

// 2d vector with values x=0, y=0
extern const vector2 null2;

// 2d unit vector in x direction
const vector2 i_hat2;

// 2d unit vector in y direction
const vector2 j_hat2;

// 3d vector with values x=0, y=0, z=0
extern const vector3 null3;

// 3d unit vector in x direction
const vector3 i_hat3;

// 3d unit vector in y direction
const vector3 j_hat3;

// 3d unit vector in z direction
const vector3 k_hat3;

vector2 scale2(double s, vector2 v);

vector3 scale3(double s, vector3 v);

vector2 add2(vector2 a, vector2 b);

vector3 add3(vector3 a, vector3 b);

vector2 sub2(vector2 a, vector2 b);

vector3 sub3(vector3 a, vector3 b);

vector2 unit2(vector2 v);

vector3 unit3(vector3 v);

double length2(vector2 v);

double length3(vector3 v);

// hadamard product - element wise multiplication product of two vectors
vector2 hadamard2(vector2 a, vector2 b);

// hadamard product - element wise multiplication product of two vectors
vector3 hadamard3(vector3 a, vector3 b);

// cross product of two 3d vectors a x b
vector3 cross(vector3 a, vector3 b);

void print2(vector2 v);

void print3(vector3 v);

bool equal2(vector2 a, vector2 b);

bool equal3(vector3 a, vector3 b);

#endif
