#ifndef LINALG2
#define LINALG2





// second version of linalg lib
// now with doubles not floats
// rendeng should migrate to this lib latter


// MATH STRUCTS
// 3d point / vect data
typedef struct VECT3D{
	double v1; // x
	double v2; // y
	double v3; // z
} vect3d;

// 2d point / vect data
typedef struct VECT2D{
	double v1; // horizontal
	double v2; // vertical
} vect2d;




// VARIABLES
// vect2d with values 0, 0 for use in calculations. (dont change)
extern const vect2d null_vect2d;
// 3d vector with values 0 0 0 for calc
extern const vect3d null_vect3d;
// 3d unit vect in x direction
extern const vect3d i_hat;
// 3d unit vect in y direction
extern const vect3d j_hat;
// 3d unit vect in z direction
extern const vect3d k_hat;

// FUNCTIONS
// 2D
// prints 3d vect values to stdout
extern void print_vect2d(vect2d v);
// adds two 2d vectors
extern vect2d add2d(vect2d v1, vect2d v2);
// subtracts form v1, v2
extern vect2d sub2d(vect2d v1, vect2d v2);
// scales 2d vect v by value s
extern vect2d scale2d(vect2d v, double s);
// calcs the lenght of 2d vect
extern double length2d(vect2d v);

// 3D
// prints 3d vect values to stdout
extern void print_vect3d(vect3d v);
// adds two 3d vectors
extern vect3d add3d(vect3d v1, vect3d v2);

// subracts form 3d vector v1, vector v2
extern vect3d sub3d(vect3d v1, vect3d v2);

// scales 3d vect v by value s
extern vect3d scale3d(vect3d v, double s);

// cross product between 3d vects v1 x v2
extern vect3d cross_prod3d(vect3d v1, vect3d v2);

// returns the scalar lenght of 3d vect
extern double length3d(vect3d v);

// returns a 3d unit vector in the direction of v
extern vect3d direction3d(vect3d v);

#endif