#include "linalg2.h"
#include <math.h>
#include <stdio.h>


// VARIABLES
// vect2d with values 0, 0 for use in calculations. (dont change)
const vect2d null_vect2d = {0, 0};
// 3d vector with values 0 0 0 for calc
const vect3d null_vect3d = {0, 0, 0};
// 3d unit vect in x direction
const vect3d i_hat = {1, 0, 0};
// 3d unit vect in y direction
const vect3d j_hat = {0, 1, 0};
// 3d unit vect in z direction
const vect3d k_hat = {0, 0, 1};


// FUNCTIONS

// 2D
// prints 3d vect values to stdout
void print_vect2d(vect2d v){
	printf("%f %f\n", v.v1, v.v2);
}

// adds two 2d vectors
vect2d add2d(vect2d v1, vect2d v2){
	v1.v1 += v2.v1;
	v1.v2 += v2.v2;
	return v1;
}

// subtracts form v1, v2
vect2d sub2d(vect2d v1, vect2d v2){
	v1.v1 -= v2.v1;
	v1.v2 -= v2.v2;
	return v1;
}

// scales 2d vect v by value s
vect2d scale2d(vect2d v, double s){
	v.v1 *= s;
	v.v2 *= s;
	return v;
}

// calcs the lenght of 2d vect
double length2d(vect2d v){
	double l = 0;
	l += v.v1*v.v1;
	l += v.v2*v.v2;
	return sqrt(l);
}


// 3D
// prints 3d vect values to stdout
void print_vect3d(vect3d v){
	printf("%f %f %f\n", v.v1, v.v2, v.v3);
}

// adds two 3d vectors
vect3d add3d(vect3d v1, vect3d v2){
	v1.v1 += v2.v1;
	v1.v2 += v2.v2;
	v1.v3 += v2.v3;
	return v1;
}

// subracts form 3d vector v1, vector v2
vect3d sub3d(vect3d v1, vect3d v2){
	v1.v1 -= v2.v1;
	v1.v2 -= v2.v2;
	v1.v3 -= v2.v3;
	return v1;
}

// scales 3d vect v by value s
vect3d scale3d(vect3d v, double s){
	v.v1 *= s;
	v.v2 *= s;
	v.v3 *= s;
	return v;
}

// cross product between 3d vects v1 x v2
vect3d cross_prod3d(vect3d v1, vect3d v2){
	vect3d r;
	r.v1 = (v1.v2*v2.v3) - (v1.v3*v2.v2);
	r.v2 = (v1.v3*v2.v1) - (v1.v1*v2.v3);
	r.v3 = (v1.v1*v2.v2) - (v1.v2*v2.v1);
	return r;
}

// returns the scalar lenght of 3d vect
double length3d(vect3d v){
	double l = 0;
	l += v.v1*v.v1;
	l += v.v2*v.v2;
	l += v.v3*v.v3;
	return sqrt(l);
}

// returns a 3d unit vector in the direction of v
vect3d direction3d(vect3d v){
	double l = length3d(v);
	v.v1 /= l;
	v.v2 /= l;
	v.v3 /= l;
	return v;
}