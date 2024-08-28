#include "vector.h"
#include <math.h>
#include <stdio.h>

const vector2 null2 = {0, 0};

const vector3 null3 = {0, 0, 0};

// 3d unit vector in x direction
const vector3 i_hat3 = {1, 0, 0};

// 3d unit vector in y direction
const vector3 j_hat3 = {0, 1, 0};

// 3d unit vector in z direction
const vector3 k_hat3 = {0, 0, 1};

// 3d unit vector in x direction
const vector2 i_hat2 = {1, 0};

// 3d unit vector in y direction
const vector2 j_hat2 = {0, 1};

vector2 scale2(double s, vector2 v) {
  return (vector2){
      .x = v.x * s,
      .y = v.y * s,
  };
}

vector3 scale3(double s, vector3 v) {
  return (vector3){
      .x = v.x * s,
      .y = v.y * s,
      .z = v.z * s,
  };
}

vector2 add2(vector2 a, vector2 b) {
  return (vector2){
      .x = a.x + b.x,
      .y = a.y + b.y,
  };
}

vector3 add3(vector3 a, vector3 b) {
  return (vector3){
      .x = a.x + b.x,
      .y = a.y + b.y,
      .z = a.z + b.z,
  };
}

vector2 sub2(vector2 a, vector2 b) {
  return (vector2){
      .x = a.x - b.x,
      .y = a.y - b.y,
  };
}

vector3 sub3(vector3 a, vector3 b) {
  return (vector3){
      .x = a.x - b.x,
      .y = a.y - b.y,
      .z = a.z - b.z,
  };
}

vector2 unit2(vector2 v) { return scale2(1 / length2(v), v); }

vector3 unit3(vector3 v) { return scale3(1 / length3(v), v); }

double length2(vector2 v) { return sqrt((v.x * v.x) + (v.y * v.y)); }

double length3(vector3 v) {
  return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vector2 hadamard2(vector2 a, vector2 b) {
  return (vector2){
      .x = a.x * b.x,
      .y = a.y * b.y,
  };
}

vector3 hadamard3(vector3 a, vector3 b) {
  return (vector3){
      .x = a.x * b.x,
      .y = a.y * b.y,
      .z = a.z * b.z,
  };
}

vector3 cross(vector3 a, vector3 b) {
  return (vector3){
      .x = a.y * b.z - a.z * b.y,
      .y = a.z * b.x - a.x * b.z,
      .z = a.x * b.y - a.y * b.x,
  };
}

void print2(vector2 v) { printf("%f %f\n", v.x, v.y); }

void print3(vector3 v) { printf("%f %f %f\n", v.x, v.y, v.z); }

bool equal2(vector2 a, vector2 b) { return a.x == b.x && a.y == b.y; }

bool equal3(vector3 a, vector3 b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}
