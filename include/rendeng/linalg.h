#ifndef LINALG
#define LINALG

#include "vector.h"

// 3d vect data type
typedef struct VECT3D {
  float values[3];
} vect3d;

// struct for storing root of a quadratic equation
// mainly used for sphere intersection
// if equation has no real roots then x1=-1 and x2=-1
typedef struct QUADRATIC_ROOTS {
  float x1;
  float x2;
} quadratic_roots;

// sets 3d vects values to v1, v2, v3
extern void set(vect3d* a, float v1, float v2, float v3);

// fills 3d vects values with v
extern void fill(vect3d* a, float v);

// prints a ginven 3d vector
extern void printv(vect3d a);

// adds to 3d vects returning the resulting vect
extern vect3d add(vect3d a, vect3d b);

// subtract 3d vectors b values from 3d vector a
extern vect3d sub(vect3d a, vect3d b);

// scale 3d vector a by value s
extern vect3d scale(vect3d a, float s);

// calculates the dot product value for 3d vectors a and b
extern float dot(vect3d a, vect3d b);

// calculates the ros product of vectors a and b (a x b)
extern vect3d cross3(vect3d a, vect3d b);

// calculates the lenght of a given 3d vector a
extern float vector_length(vect3d a);

// claculates a 3d unit vector given 3d vect a and 3d vect b. the direction is a
// to b.
extern vect3d make_unit_vect(vect3d a, vect3d b);

// normalize a given vector to unit lenght
extern vect3d normalize(vect3d a);

// returns the max value of values a and b
extern float maxv(float a, float b);

// returns the smalles value of a and b
extern float minv(float a, float b);

extern vect3d from_vector3(vector3 v);

#endif
