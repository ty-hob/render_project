#include "vector.h"
#include <math.h>

const vector2 null2 = {0, 0};

const vector3 null3 = {0, 0, 0};

vector2 scale2(vector2 v, double s) {
  return (vector2){//
                   .x = v.x * s,
                   .y = v.y * s
  };
}

vector3 scale3(vector3 v, double s) {
  return (vector3){//
                   .x = v.x * s,
                   .y = v.y * s,
                   .z = v.z * s
  };
}

vector2 add2(vector2 a, vector2 b) {
  return (vector2){//
                   .x = a.x + b.x,
                   .y = a.y + b.y
  };
}

vector3 add3(vector3 a, vector3 b) {
  return (vector3){//
                   .x = a.x + b.x,
                   .y = a.y + b.y,
                   .z = a.z + b.z
  };
}

vector2 sub2(vector2 a, vector2 b) {
  return (vector2){//
                   .x = a.x - b.x,
                   .y = a.y - b.y
  };
}

vector3 sub3(vector3 a, vector3 b) {
  return (vector3){//
                   .x = a.x - b.x,
                   .y = a.y - b.y,
                   .z = a.z - b.z
  };
}

vector2 unit2(vector2 v) { return scale2(v, 1 / length2(v)); }

vector3 unit3(vector3 v) { return scale3(v, 1 / length3(v)); }

double length2(vector2 v) { return sqrt((v.x * v.x) + (v.y * v.y)); }

double length3(vector3 v) {
  return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}
