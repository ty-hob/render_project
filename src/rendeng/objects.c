#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "rendeng/objects.h"

void free_scene(scene_object* scene) {
  free(scene->name);
  free(scene->materials);
  free(scene);
}

vector3 get_lense_top_left_corner(const camera_object* camera) {
  return add3(
      add3(
          camera->lense_center,
          scale3(-camera->lense_height / 2, get_lense_vertical_unit(camera))
      ),
      scale3(-camera->lense_width / 2, get_lense_horizontal_unit(camera))
  );
}

vector3 get_lense_horizontal_unit(const camera_object* camera) {
  return scale3(
      -1, unit3(cross(sub3(camera->lense_center, camera->focus), k_hat3))
  );
}

vector3 get_lense_vertical_unit(const camera_object* camera) {
  return scale3(
      -1, unit3(cross(sub3(camera->lense_center, camera->focus), j_hat3))
  );
}

vector3 get_lense_horizontal_iter(const camera_object* camera) {
  return scale3(
      1 / camera->lense_pixel_density, get_lense_horizontal_unit(camera)
  );
}

vector3 get_lense_vertical_iter(const camera_object* camera) {
  return scale3(
      1 / camera->lense_pixel_density, get_lense_vertical_unit(camera)
  );
}

// does the parsing of object types sphere, triangle, plane and returns the
// requested pointer of an objects parameter.
void* get_object_parameter(char pram_type, object obj) {
  // material           - m
  // get_normal         - n
  // check_intersection - i
  if (pram_type == 'm') {
    if (obj.type == 's') {
      return ((sphere*)obj.object_pointer)->mat;
    } else if (obj.type == 't') {
      return ((triangle*)obj.object_pointer)->mat;
    } else if (obj.type == 'p') {
      return ((plane*)obj.object_pointer)->mat;
    } else {
      puts("unrecognized object type");
      return NULL;
    }
  } else if (pram_type == 'n') {
    if (obj.type == 's') {
      return ((sphere*)obj.object_pointer)->get_normal;
    } else if (obj.type == 't') {
      return ((triangle*)obj.object_pointer)->get_normal;
    } else if (obj.type == 'p') {
      return ((plane*)obj.object_pointer)->get_normal;
    } else {
      puts("unrecognized object type\n");
      return NULL;
    }
  } else if (pram_type == 'i') {
    if (obj.type == 's') {
      return ((sphere*)obj.object_pointer)->check_intersection;
    } else if (obj.type == 't') {
      return ((triangle*)obj.object_pointer)->check_intersection;
    } else if (obj.type == 'p') {
      return ((plane*)obj.object_pointer)->check_intersection;
    } else {
      puts("unrecognized object type\n");
      return NULL;
    }
  } else {
    puts("unrecognized object parameeter type \n");
    return NULL;
  }
}

// function for finding the closes object given all the objects in sceene and
// ray star pos and direction if the closest_object distance value is -1 the
// pointer to object value is garbage and should be ignored
closest_object get_closest_object(
    object_manager* objm, vect3d start_pos, vect3d ray_direction
) {
  float distance_to_object;
  closest_object cobj; // closest object
  float (*check_intersection)(vect3d, vect3d, void*);

  // if any object exist set the intersection wiht the first one as the closest
  if (objm->scene_object_count != 0) {

    check_intersection = get_object_parameter('i', objm->scene_objects[0]);
    cobj.distance      = check_intersection(
        ray_direction, start_pos, objm->scene_objects[0].object_pointer
    );
    cobj.c_object.type           = objm->scene_objects[0].type;
    cobj.c_object.object_pointer = objm->scene_objects[0].object_pointer;

  } else {
    cobj.distance = -1;
  }
  // if more than one objects exists find the closest one by checking them all
  // and updating the closes obj value if a closer object is found to the
  // starting point
  for (int i = 1; i < objm->scene_object_count; i++) {
    check_intersection = get_object_parameter('i', objm->scene_objects[i]);
    distance_to_object = check_intersection(
        ray_direction, start_pos, objm->scene_objects[i].object_pointer
    );

    if (distance_to_object != -1) {
      if (cobj.distance == -1) {
        cobj.distance                = distance_to_object;
        cobj.c_object.type           = objm->scene_objects[i].type;
        cobj.c_object.object_pointer = objm->scene_objects[i].object_pointer;
      } else if (distance_to_object < cobj.distance) {
        cobj.distance                = distance_to_object;
        cobj.c_object.type           = objm->scene_objects[i].type;
        cobj.c_object.object_pointer = objm->scene_objects[i].object_pointer;
      }
    }
  }
  return cobj;
}

// given a point in scene and a object that that point is on calculate the
// shadeing of that given point.
vector3 get_color_of_point(
    vect3d ray_direction,
    vect3d intersection_point,
    closest_object cobj,
    object_manager* objm,
    camera_object* camera_object,
    vector3 ambient_light_color,
    int reflection_depth,
    int refraction_depth
) {
  // get the material of the object
  material_object* mat = get_object_parameter('m', cobj.c_object);

  // get suface normal
  vect3d (*get_normal)(vect3d, void*)
      = get_object_parameter('n', cobj.c_object);
  vect3d suface_normal
      = get_normal(intersection_point, cobj.c_object.object_pointer);

  // apply the ambient term
  vector3 color = hadamard3(ambient_light_color, mat->diffuse_color);

  // iterate over light sources
  vect3d to_light_direction;
  for (int i = 0; i < objm->light_object_count; i++) {
    // normal form suface to light source
    to_light_direction
        = make_unit_vect(intersection_point, objm->light_objects[i].pos);

    // add diffuse shadeing
    // max(0, dot(n, r)) * diffuse_color * light_color_or_intensity
    color = add3(
        color,
        scale3(
            // cos_theta
            maxv(0, dot(to_light_direction, suface_normal)),
            hadamard3(
                mat->diffuse_color, objm->light_objects[i].diffuse_light_color
            )
        )
    );

    // add specular shading
    // normalized ray form intersection point to view point(camera)
    // normalize(sub(camera_object.camera_pos, intersection_point));
    // normalized reflection of light ray
    // scale(reflect_ray(to_light_direction, suface_normal), -1);
    double cos_alpha = pow(
        maxv(
            0,
            dot(make_unit_vect(
                    intersection_point, from_vector3(camera_object->focus)
                ),
                scale(reflect_ray(to_light_direction, suface_normal), -1))
        ),
        mat->shininess_const
    );

    // pow(dot(light_reflection, to_view), shinines_cont) *
    // specular_light_color * specular_reflection_const
    // specular_reflection_const = 1 here
    color = add3(
        color, scale3(cos_alpha, objm->light_objects[i].specular_light_color)
    );
  }

  // apply reflection
  if (reflection_depth > 0) {
    if (mat->reflectiveness_const > 0) {
      // get the reflection ray
      ray_direction = reflect_ray(ray_direction, suface_normal);

      // find the closes object
      closest_object reflect_cobj
          = get_closest_object(objm, intersection_point, ray_direction);
      // check if an object was hit by traced ray
      if (reflect_cobj.distance != -1) {
        // get the color of that point
        vector3 reflection_color = get_color_of_point(
            ray_direction,
            add(intersection_point,
                scale(ray_direction, reflect_cobj.distance)),
            reflect_cobj,
            objm,
            camera_object,
            ambient_light_color,
            reflection_depth - 1,
            refraction_depth - 1
        );

        // apply the color
        color
            = add3(color, scale3(mat->reflectiveness_const, reflection_color));
      }
    }
  }

  // apply refraction
  if (refraction_depth > 0) {
    if (mat->refractiveness_const > 0) {
      // get rfractive ray
      ray_direction = refract_ray(cobj, ray_direction, &intersection_point);

      closest_object refract_cobj = get_closest_object(
          objm,
          add(intersection_point, scale(ray_direction, 0.1)),
          ray_direction
      );

      if (refract_cobj.distance != -1) {
        vector3 refraction_color = get_color_of_point(
            ray_direction,
            add(intersection_point,
                scale(ray_direction, refract_cobj.distance + .1)),
            refract_cobj,
            objm,
            camera_object,
            ambient_light_color,
            reflection_depth - 1,
            refraction_depth - 1
        );

        color
            = add3(color, scale3(mat->refractiveness_const, refraction_color));
      }
    }
  }

  // apply shadows to shadeing
  int obscursions = trace_shadows(objm, cobj, intersection_point);
  if (obscursions > 0) {
    color = scale3(1.0 / (4.0 * (double)obscursions), color);
  }

  // normalizeing the color
  if (color.x > 1) {
    color.x = 1;
  }
  if (color.y > 1) {
    color.y = 1;
  }
  if (color.z > 1) {
    color.z = 1;
  }

  return color;
}

// returns how many times a point on an object is in shadow of a light by other
// objects (not it self)
int trace_shadows(
    object_manager* objm, closest_object cobj, vect3d intersection_point
) {
  int obscursions = 0;

  vect3d ray_direction;
  float distance_to_object, distance_to_closest_objscursion;

  float (*check_intersection)(vect3d, vect3d, void*);

  // iterate over lights
  for (int l = 0; l < objm->light_object_count; l++) {

    ray_direction
        = make_unit_vect(intersection_point, objm->light_objects[l].pos);

    // find distance to closest object that is not cobj
    distance_to_closest_objscursion = -1;
    // iterate over objects
    for (int i = 0; i < objm->scene_object_count; i++) {
      if (cobj.c_object.object_pointer
          != objm->scene_objects[i].object_pointer) {
        check_intersection = get_object_parameter('i', objm->scene_objects[i]);
        distance_to_object = check_intersection(
            ray_direction,
            intersection_point,
            objm->scene_objects[i].object_pointer
        );

        if (distance_to_object != -1) {
          if (distance_to_closest_objscursion == -1) {
            distance_to_closest_objscursion = distance_to_object;
          } else if (distance_to_object < distance_to_closest_objscursion) {
            distance_to_closest_objscursion = distance_to_object;
          }
        }
      }
    }

    if (distance_to_closest_objscursion
            < vector_length(sub(intersection_point, objm->light_objects[l].pos))
        && distance_to_closest_objscursion > 0) {
      obscursions += 1;
    }
  }

  return obscursions;
}

// finds the reflected ray of an given object
vect3d refract_ray(
    closest_object cobj, vect3d ray_direction, vect3d* intersection_point
) {
  // get surface normal of cobj
  vect3d (*get_normal)(vect3d, void*)
      = get_object_parameter('n', cobj.c_object);
  vect3d suface_normal
      = get_normal(*intersection_point, cobj.c_object.object_pointer);

  // get material of cobj
  material_object* mat = get_object_parameter('m', cobj.c_object);

  float n     = 1 / mat->refractive_index;
  float cosI  = -dot(suface_normal, ray_direction);
  float sinT2 = n * n * (1.0 - (cosI * cosI));
  if (sinT2 > 1) {
    puts("invalid refraction indices");
  }
  float cosT = sqrt(1.0 - sinT2);

  if (cobj.c_object.type == 's') {
    ray_direction
        = add(scale(ray_direction, n), scale(suface_normal, (n * cosI) - cosT));
    float d = sphere_intersection(
        ray_direction,
        add(*intersection_point, scale(ray_direction, 0.1)),
        cobj.c_object.object_pointer
    );

    if (d != -1) {
      d += .0;

      *intersection_point = add(*intersection_point, scale(ray_direction, d));

      suface_normal
          = get_normal(*intersection_point, cobj.c_object.object_pointer);

      n     = mat->refractive_index;
      cosI  = -dot(suface_normal, ray_direction);
      sinT2 = n * n * (1.0 - (cosI * cosI));
      if (sinT2 > 1) {
        puts("invalid sphere refraction indices");
      }
      cosT = sqrt(1.0 - sinT2);

      return add(
          scale(ray_direction, n), scale(suface_normal, (n * cosI) - cosT)
      );

    } else {
      return ray_direction;
    }

  } else {
    return add(
        scale(ray_direction, n), scale(suface_normal, (n * cosI) - cosT)
    );
  }
  // n1 = 1
  // n2 = cobj.c_object.mat->refraction_index
}

// reflect a given ray against a suface normal
vect3d reflect_ray(vect3d ray_direction, vect3d suface_normal) {
  return sub(
      ray_direction, scale(suface_normal, 2 * dot(ray_direction, suface_normal))
  );
}

// return the smallest distance to intersecton point from starting point
float sphere_intersection(
    vect3d ray_direction, vect3d start_pos, void* obj_prt
) {
  sphere* self = (sphere*)obj_prt;
  // sqrt
  vect3d temp_sub = sub(start_pos, self->center);
  float t         = dot(ray_direction, temp_sub);

  float d = t * t - (dot(temp_sub, temp_sub) - (self->radius * self->radius));
  if (d >= 0) {
    float d1, d2;
    d1 = -t + sqrt(d);
    d2 = -t - sqrt(d);
    if (d1 > 0) {
      if (d1 < d2) {
        return d1;
      } else if (d2 > 0) {
        return d2;
      } else {
        return -1;
      }
    } else {
      if (d2 > 0) {
        return d2;
      } else {
        return -1;
      }
    }
  } else {
    return -1;
  }
}

// returns the distance to a triangle object. if ray has not intersected a
// triangle then returns -1
float triangle_intersection(
    vect3d ray_direction, vect3d start_pos, void* obj_prt
) {
  triangle* self = (triangle*)obj_prt;
  // get normal
  vect3d normal = self->get_normal(
      ray_direction, self
  ); // ray direction just forr filler value. is not used in calculation

  float d = dot(ray_direction, normal);
  if (d != 0) {
    d = dot(sub(self->p1, start_pos), normal) / d;
    if (d <= 0) {
      return -1;
    } else {
      vect3d p = add(start_pos, scale(ray_direction, d));
      if (dot(normal, cross3(sub(self->p2, self->p1), sub(p, self->p1))) > 0) {
        if (dot(normal, cross3(sub(self->p3, self->p2), sub(p, self->p2)))
            > 0) {
          if (dot(normal, cross3(sub(self->p1, self->p3), sub(p, self->p3)))
              > 0) {
            return d;
          }
        }
      }
      return -1;
    }
  } else {
    return -1;
  }
}

// returns the distance to intersiction point to a plane. if ray and plane are
// paralel then returns -1 if distance is negative returns -1
float plane_intersection(
    vect3d ray_direction, vect3d start_pos, void* obj_prt
) {
  plane* self = (plane*)obj_prt;
  float div   = dot(ray_direction, self->normal);
  if (div != 0) {
    div = dot(sub(self->point, start_pos), self->normal) / div;
    if (div <= 0) {
      return -1;
    } else {
      return div;
    }
  } else {
    return -1;
  }
}

// object normals code

// claculates the surface normal (unit vect) for an given sphere object and
// point of intersection.
vect3d sphere_normal(vect3d intersection_point, void* obj_prt) {
  sphere* self       = (sphere*)obj_prt;
  intersection_point = make_unit_vect(self->center, intersection_point);
  return intersection_point;
}

// returns the suface normal of an triangle object. function has parameeter
// intersecton for compatability with sphere object
vect3d triangle_normal(vect3d intersection_point, void* obj_prt) {
  triangle* self = (triangle*)obj_prt;
  if (self->invert_normal) {
    return scale(
        normalize(cross3(sub(self->p2, self->p1), sub(self->p3, self->p1))), -1
    );
  } else {
    return normalize(cross3(sub(self->p2, self->p1), sub(self->p3, self->p1)));
  }
}

// returns the surface normal of plane object. function has unused variables for
// compatability for object manager
vect3d plane_normal(vect3d intersection_point, void* obj_prt) {
  plane* self = (plane*)obj_prt;
  return self->normal;
}

// adding object code

// adds materal to for use in objects
void add_material(
    object_manager* objm,
    float dr,
    float dg,
    float db,
    int sc,
    float reflc,
    float refrc,
    float refri
) {
  if (objm->material_count >= MAX_MATERIALS) {
    printf("max material count reached\n");
    return;
  }
  objm->materials[objm->material_count].diffuse_color.x      = dr;
  objm->materials[objm->material_count].diffuse_color.y      = dg;
  objm->materials[objm->material_count].diffuse_color.z      = db;
  objm->materials[objm->material_count].shininess_const      = sc;
  objm->materials[objm->material_count].reflectiveness_const = reflc;
  objm->materials[objm->material_count].refractiveness_const = refrc;
  objm->materials[objm->material_count].refractive_index     = refri;

  objm->material_count += 1;
}

// adds a sphere to scene
void add_sphere(
    object_manager* objm,
    float cx,
    float cy,
    float cz,
    float radius,
    material_object* mat
) {
  if (objm->sphere_object_count >= MAX_SPHERE_OBJECTS) {
    puts("max sphere object count in scene exeded\n");
    return;
  }
  objm->sphere_objects[objm->sphere_object_count].center.values[0] = cx;
  objm->sphere_objects[objm->sphere_object_count].center.values[1] = cy;
  objm->sphere_objects[objm->sphere_object_count].center.values[2] = cz;
  objm->sphere_objects[objm->sphere_object_count].radius           = radius;
  objm->sphere_objects[objm->sphere_object_count].mat              = mat;
  objm->sphere_objects[objm->sphere_object_count].check_intersection
      = &sphere_intersection;
  objm->sphere_objects[objm->sphere_object_count].get_normal = &sphere_normal;

  objm->scene_objects[objm->scene_object_count].type = 's';
  objm->scene_objects[objm->scene_object_count].object_pointer
      = &objm->sphere_objects[objm->sphere_object_count];

  objm->sphere_object_count += 1;
  objm->scene_object_count += 1;
}

// adds triangle object to scene with coners in points p1, p2, p3. with a
// given pointer to a meterial struct
void add_triangle(
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
) {
  if (objm->triangle_object_count >= MAX_TRIANGLE_OBJECTS) {
    puts("max triangle object count in scene exeded\n");
    return;
  }
  objm->triangle_objects[objm->triangle_object_count].p1.values[0] = p1x;
  objm->triangle_objects[objm->triangle_object_count].p1.values[1] = p1y;
  objm->triangle_objects[objm->triangle_object_count].p1.values[2] = p1z;

  objm->triangle_objects[objm->triangle_object_count].p2.values[0] = p2x;
  objm->triangle_objects[objm->triangle_object_count].p2.values[1] = p2y;
  objm->triangle_objects[objm->triangle_object_count].p2.values[2] = p2z;

  objm->triangle_objects[objm->triangle_object_count].p3.values[0] = p3x;
  objm->triangle_objects[objm->triangle_object_count].p3.values[1] = p3y;
  objm->triangle_objects[objm->triangle_object_count].p3.values[2] = p3z;

  objm->triangle_objects[objm->triangle_object_count].mat = mat;

  objm->triangle_objects[objm->triangle_object_count].invert_normal
      = invert_normal;

  objm->triangle_objects[objm->triangle_object_count].check_intersection
      = &triangle_intersection;
  objm->triangle_objects[objm->triangle_object_count].get_normal
      = &triangle_normal;

  objm->scene_objects[objm->scene_object_count].type = 't';
  objm->scene_objects[objm->scene_object_count].object_pointer
      = &objm->triangle_objects[objm->triangle_object_count];

  objm->triangle_object_count += 1;
  objm->scene_object_count += 1;
}

// adds plane to scene. nx, ny, nz are surface normal values. when setting
// surface normal it can be set with a length larger or smaller than 1. the
// suface normal is normalized on initalisation
void add_plane(
    object_manager* objm,
    float px,
    float py,
    float pz,
    float nx,
    float ny,
    float nz,
    material_object* mat
) {
  if (objm->plane_object_count >= MAX_PLANE_OBJECTS) {
    puts("max plane object count in scene exeded\n");
    return;
  }
  objm->plane_objects[objm->plane_object_count].point.values[0] = px;
  objm->plane_objects[objm->plane_object_count].point.values[1] = py;
  objm->plane_objects[objm->plane_object_count].point.values[2] = pz;
  objm->plane_objects[objm->plane_object_count].mat             = mat;
  objm->plane_objects[objm->plane_object_count].check_intersection
      = &plane_intersection;
  objm->plane_objects[objm->plane_object_count].get_normal = &plane_normal;
  vect3d normal;
  normal.values[0]                                     = nx;
  normal.values[1]                                     = ny;
  normal.values[2]                                     = nz;
  normal                                               = normalize(normal);
  objm->plane_objects[objm->plane_object_count].normal = normal;

  objm->scene_objects[objm->scene_object_count].type = 'p';
  objm->scene_objects[objm->scene_object_count].object_pointer
      = &objm->plane_objects[objm->plane_object_count];

  objm->plane_object_count += 1;
  objm->scene_object_count += 1;
}

// adds a light object to scene
void add_light(
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
) {
  if (objm->light_object_count >= MAX_LIGHT_OBJECTS) {
    puts("max light object count in scene exeded\n");
    return;
  }

  objm->light_objects[objm->light_object_count].pos.values[0] = px;
  objm->light_objects[objm->light_object_count].pos.values[1] = py;
  objm->light_objects[objm->light_object_count].pos.values[2] = pz;

  objm->light_objects[objm->light_object_count].diffuse_light_color.x = dr;
  objm->light_objects[objm->light_object_count].diffuse_light_color.y = dg;
  objm->light_objects[objm->light_object_count].diffuse_light_color.z = db;

  objm->light_objects[objm->light_object_count].specular_light_color.x = sr;
  objm->light_objects[objm->light_object_count].specular_light_color.y = sg;
  objm->light_objects[objm->light_object_count].specular_light_color.z = sb;

  objm->light_object_count += 1;
}
