#ifndef OBJS
#define OBJS

#define MAX_SPHERE_OBJECTS 10
#define MAX_PLANE_OBJECTS 10
#define MAX_TRIANGLE_OBJECTS 80
#define MAX_LIGHT_OBJECTS 10
#define MAX_MATERIALS 10

#define MAX_REFLECTIONS 4
#define MAX_REFRACTIONS 4

#include <stdbool.h>

#include "rendeng/image.h"
#include "rendeng/linalg.h"
#include "vector.h"

// struct containing information about the camera
typedef struct CAMERA {
  vector3 camera_pos;
  float distance_to_lense;
  float lense_width;
  float lense_height;
  int sub_ray_count;
} camera;

typedef struct MATERIAL {
  rgb_color diffuse_color;
  int shininess_const;
  float reflectivenes_const;
  float refractivenes_const;
  float refractive_index;
} material;

// struct containing information about a sphere object
typedef struct SPHERE {
  vect3d center;
  float radius;
  material* mat;
  float (*check_intersection)(vect3d, vect3d, void*);
  vect3d (*get_normal)(vect3d, void*); // change to get normal of point of
                                       // intersection and make get reflection
                                       // public
} sphere;

// struct containing information about a sceene object of type tirangle
// the most basic poligon
typedef struct TRIANGLE {
  vect3d p1;
  vect3d p2;
  vect3d p3;
  material* mat;
  bool invert_normal;
  float (*check_intersection)(vect3d, vect3d, void*);
  vect3d (*get_normal)(vect3d, void*);
} triangle;

typedef struct PLANE {
  vect3d point;
  vect3d normal;
  material* mat;
  float (*check_intersection)(vect3d, vect3d, void*);
  vect3d (*get_normal)(vect3d, void*);
} plane;

// struct containing information about light object
typedef struct LIGHT {
  vect3d pos;
  rgb_color diffuse_light_color;
  rgb_color specular_light_color;
} light;

// struct for dealing with different object types
// created for use in the future when i add poligons
// and maby someting else idn.
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

  material* materials;
  int material_count;
} object_manager;

// return type of function get_coloset_object
// contains the distance and a pointer to and object
typedef struct CLOSEST_OBJECT {
  float distance;
  object c_object; // no good name for this shit // sphere for no later struct -
                   // object pointer
} closest_object;

// does the parsing of object types sphere, triangle, plane and returns the
// requested pointer of an objects parameter.
extern void* get_object_parameter(char pram_type, object obj);

// function for finding the closes object given all the objects in sceene and
// ray star pos and direction
extern closest_object get_closest_object(
    object_manager* objm, vect3d start_pos, vect3d ray_direction
);

// given a point in scene and a object that that point is on calculate the
// shadeing of that given point.
extern rgb_color get_color_of_point(
    vect3d ray_direction,
    vect3d intersection_point,
    closest_object cobj,
    object_manager* objm,
    camera* camera_object,
    rgb_color ambient_light,
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

// adding objects to sceene code
// adds materal to for use in objects
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

// adds a sphere to sceene
extern void add_sphere(
    object_manager* objm,
    float cx,
    float cy,
    float cz,
    float radius,
    material* mat
);

// adds triangle object to sceene withs coners in points p1, p2, p3. with a
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
    material* mat,
    bool invert_normal
);

// adds plane to sceene. nx, ny, nz are surface normal values. when setting
// surface normal it can be set with a length larger or smaller than 1. the
// suface normal is normalized on initalization
extern void add_plane(
    object_manager* objm,
    float px,
    float py,
    float pz,
    float nx,
    float ny,
    float nz,
    material* mat
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
