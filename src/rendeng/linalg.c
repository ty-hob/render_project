#include "rendeng/linalg.h"
#include <math.h>
#include <stdio.h>

// sets 3d vects values to v1, v2, v3
void set(vect3d* a, float v1, float v2, float v3) {
  a->values[0] = v1;
  a->values[1] = v2;
  a->values[2] = v3;
}

// fills 3d vects values with v
void fill(vect3d* a, float v) {
  a->values[0] = v;
  a->values[1] = v;
  a->values[2] = v;
}

// prints a ginven 3d vector
void printv(vect3d a) {
  printf("[%f %f %f]\n", a.values[0], a.values[1], a.values[2]);
}

// adds to 3d vects returning the resulting vect
vect3d add(vect3d a, vect3d b) {
  a.values[0] += b.values[0];
  a.values[1] += b.values[1];
  a.values[2] += b.values[2];
  return a;
}

// subtract 3d vectors b values from 3d vector a
vect3d sub(vect3d a, vect3d b) {
  a.values[0] -= b.values[0];
  a.values[1] -= b.values[1];
  a.values[2] -= b.values[2];
  return a;
}

// scale 3d vector a by value s
vect3d scale(vect3d a, float s) {
  a.values[0] *= s;
  a.values[1] *= s;
  a.values[2] *= s;
  return a;
}

// calculates the dot product value for 3d vectors a and b
float dot(vect3d a, vect3d b) {
  return (a.values[0] * b.values[0]) + (a.values[1] * b.values[1])
       + (a.values[2] * b.values[2]);
}

// calculates the ros porduct of vectors a and b (a x b)
vect3d cross(vect3d a, vect3d b) {
  vect3d c;
  c.values[0] = a.values[1] * b.values[2] - a.values[2] * b.values[1];
  c.values[1] = a.values[2] * b.values[0] - a.values[0] * b.values[2];
  c.values[2] = a.values[0] * b.values[1] - a.values[1] * b.values[0];
  return c;
}

// calculates the lenght of a given 3d vector a
float vector_length(vect3d a) {
  return sqrt(
      (a.values[0] * a.values[0]) + (a.values[1] * a.values[1])
      + (a.values[2] * a.values[2])
  );
}

// claculates a 3d unit vector given 3d vect a and 3d vect b. the direction is a
// to b.
vect3d make_unit_vect(vect3d a, vect3d b) {
  return scale(sub(b, a), 1.0 / vector_length(sub(b, a)));
  ;
}

// normalize a given vector to unit lenght
vect3d normalize(vect3d a) {
  float sq = sqrt(
      (a.values[0] * a.values[0]) + (a.values[1] * a.values[1])
      + (a.values[2] * a.values[2])
  );
  a.values[0] /= sq;
  a.values[1] /= sq;
  a.values[2] /= sq;
  return a;
}

// returns the max value of values a and b
float maxv(float a, float b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

// returns the smalles value of a and b
float minv(float a, float b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

vect3d from_vector3(vector3 v) {
  vect3d a;
  a.values[0] = v.x;
  a.values[1] = v.y;
  a.values[2] = v.z;
  return a;
}
