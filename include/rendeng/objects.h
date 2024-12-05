#ifndef OBJECTS_H
#define OBJECTS_H

#define MAX_SPHERE_OBJECTS 10
#define MAX_PLANE_OBJECTS 10
#define MAX_TRIANGLE_OBJECTS 80
#define MAX_LIGHT_OBJECTS 10
#define MAX_MATERIALS 10

#include <stdbool.h>

#include "rendeng/image.h"
#include "rendeng/linalg.h"
#include "vector.h"

// struct containing information about the camera
typedef struct CAMERA_OBJECT {
  vector3 focus;
  vector3 lense_center;

  double lense_width;
  double lense_height;
  // the amount of pixels per unit of lense width/height
  double lense_pixel_density;

  int rays_per_pixel;

  int reflection_depth;
  int refraction_depth;
} camera_object;

typedef struct MATERIAL_OBJECT {
  char* name;
  vector3 diffuse_color;
  int shininess_const;
  double reflectiveness_const;
  double refractiveness_const;
  double refractive_index;
} material_object;

// struct containing information about a sphere object
typedef struct SPHERE {
  vect3d center;
  float radius;
  material_object* mat;
  float (*check_intersection)(vect3d, vect3d, void*);
  vect3d (*get_normal)(vect3d, void*); // change to get normal of point of
                                       // intersection and make get reflection
                                       // public
} sphere;

// struct containing information about a scene object of type triangle
// the most basic polygon
typedef struct TRIANGLE {
  vect3d p1;
  vect3d p2;
  vect3d p3;
  material_object* mat;
  bool invert_normal;
  float (*check_intersection)(vect3d, vect3d, void*);
  vect3d (*get_normal)(vect3d, void*);
} triangle;

typedef struct PLANE {
  vect3d point;
  vect3d normal;
  material_object* mat;
  float (*check_intersection)(vect3d, vect3d, void*);
  vect3d (*get_normal)(vect3d, void*);
} plane;

// struct containing information about light object
typedef struct LIGHT {
  vect3d pos;
  vector3 diffuse_light_color;
  vector3 specular_light_color;
} light;

// defines a scene
typedef struct SCENE_OBJECT {
  char* name;
  camera_object camera;

  material_object* materials;
  int materials_size;

} scene_object;

// struct for dealing with different object types
// created for use in the future when i add polygons
// and maybe someting else idk.
typedef struct OBJECT {
  char type;
  void* object_pointer;
} object;

// struct containing pointers to object type buffers like sphere[] and
// triangle[] and light[]
typedef struct OBJECT_MANAGER {
  object* scene_objects;
  int scene_object_count;

  sphere* sphere_objects;
  int sphere_object_count;

  plane* plane_objects;
  int plane_object_count;

  triangle* triangle_objects;
  int triangle_object_count;

  light* light_objects;
  int light_object_count;

  material_object* materials;
  int material_count;
} object_manager;

// return type of function get_coloset_object
// contains the distance and a pointer to and object
typedef struct CLOSEST_OBJECT {
  float distance;
  object c_object; // no good name for this shit // sphere for no later struct -
                   // object pointer
} closest_object;

void free_scene(scene_object* scene);

// returns the top left corner of the lense.
vector3 get_lense_top_left_corner(const camera_object* camera);

// returns a unit vector horizontal to the lense square plane
// pointing from left to right
vector3 get_lense_horizontal_unit(const camera_object* camera);

// returns a unit vector vertical to the lense square plane
// pointing from top to bottom
vector3 get_lense_vertical_unit(const camera_object* camera);

// returns the iterator vector that passes through the lense square plane
// horizontally
// for single ray
// pointing from left to right
vector3 get_lense_horizontal_iter(const camera_object* camera);

// returns the iterator vector that passes through the lense square plane
// vertically
// for single ray
// pointing from left to right
vector3 get_lense_vertical_iter(const camera_object* camera);

// does the parsing of object types sphere, triangle, plane and returns the
// requested pointer of an objects parameter.
extern void* get_object_parameter(char pram_type, object obj);

// function for finding the closes object given all the objects in scene and
// ray star pos and direction
extern closest_object get_closest_object(
    object_manager* objm, vect3d start_pos, vect3d ray_direction
);

// given a point in scene and a object that that point is on calculate the
// shadeing of that given point.
vector3 get_color_of_point(
    vect3d ray_direction,
    vect3d intersection_point,
    closest_object cobj,
    object_manager* objm,
    camera_object* camera_object,
    vector3 ambient_light_color,
    int reflection_count,
    int refraction_count
);

// returns how many times a point on an object is in shadow of a light by other
// objects (not it self)
extern int trace_shadows(
    object_manager* objm, closest_object cobj, vect3d intersection_point
);

// finds the reflected ray of an given object
extern vect3d refract_ray(
    closest_object cobj, vect3d ray_direction, vect3d* intersection_point
);

// reflect a given ray against a suface normal
extern vect3d reflect_ray(vect3d ray_direction, vect3d suface_normal);

// adding objects to scene code
// adds material to for use in objects
extern void add_material(
    object_manager* objm,
    float dr,
    float dg,
    float db,
    int sc,
    float reflc,
    float refrc,
    float refri
);

// adds a sphere to scene
extern void add_sphere(
    object_manager* objm,
    float cx,
    float cy,
    float cz,
    float radius,
    material_object* mat
);

// adds triangle object to scene with coners in points p1, p2, p3. with a
// given pointer to a meterial struct
extern void add_triangle(
    object_manager* objm,
    float p1x,
    float p1y,
    float p1z,
    float p2x,
    float p2y,
    float p2z,
    float p3x,
    float p3y,
    float p3z,
    material_object* mat,
    bool invert_normal
);

// adds plane to scene. nx, ny, nz are surface normal values. when setting
// surface normal it can be set with a length larger or smaller than 1. the
// suface normal is normalized on initalisation
extern void add_plane(
    object_manager* objm,
    float px,
    float py,
    float pz,
    float nx,
    float ny,
    float nz,
    material_object* mat
);

// adds a light object to sceene
extern void add_light(
    object_manager* objm,
    float px,
    float py,
    float pz,
    float dr,
    float dg,
    float db,
    float sr,
    float sg,
    float sb
);

// intersecton calculation funcitons
// return the smallest distance to intersecton point from starting point
extern float
sphere_intersection(vect3d ray_direction, vect3d start_pos, void* obj_prt);

// returns the distance to a triangle object. if ray has not intersected a
// triangle then returns -1
extern float
triangle_intersection(vect3d ray_direction, vect3d start_pos, void* obj_prt);

// returns the distance to intersiction point to a plane. if ray and plane are
// paralel then returns -1 if distance is negative returns -1
extern float
plane_intersection(vect3d ray_direction, vect3d start_pos, void* obj_prt);

// getting normals code
// claculates the surface normal (unit vect) for an given sphere object and
// point of intersection.
extern vect3d sphere_normal(vect3d intersection_point, void* obj_prt);

// returns the suface normal of an triangle object. function has parameeter
// intersecton for compatability with sphere object
extern vect3d triangle_normal(vect3d intersection_point, void* obj_prt);

// returns the surface normal of plane object. function has unused variables for
// compatability for object manager
extern vect3d plane_normal(vect3d intersection_point, void* obj_prt);

#endif
