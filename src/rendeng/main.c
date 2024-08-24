#include <stdbool.h>
#include <stdio.h>

#include "rendeng/error.h"
#include "rendeng/image.h"
#include "rendeng/linalg.h"
#include "rendeng/objs.h"
#include "rendeng/scene.h"
#include "vector.h"

int main(int argc, char* argv[]) {
  int image_width  = 200;
  int image_height = 200;

  // random use defined colors
  vector3 background_color = {.5, .5, .5};

  // scene object storage
  sphere sphere_objects[MAX_SPHERE_OBJECTS];

  plane plane_objects[MAX_PLANE_OBJECTS];

  triangle triangle_objects[MAX_TRIANGLE_OBJECTS];

  light light_objects[MAX_LIGHT_OBJECTS];

  material materials[MAX_MATERIALS];

  object scene_objects
      [MAX_SPHERE_OBJECTS + MAX_TRIANGLE_OBJECTS + MAX_PLANE_OBJECTS
       + MAX_TRIANGLE_OBJECTS];

  // object manager setup
  object_manager obj_manager;
  obj_manager.scene_objects      = &scene_objects[0];
  obj_manager.scene_object_count = 0;

  obj_manager.sphere_objects      = &sphere_objects[0];
  obj_manager.sphere_object_count = 0;

  obj_manager.plane_objects      = &plane_objects[0];
  obj_manager.plane_object_count = 0;

  obj_manager.triangle_objects      = &triangle_objects[0];
  obj_manager.triangle_object_count = 0;

  obj_manager.light_objects      = &light_objects[0];
  obj_manager.light_object_count = 0;

  obj_manager.materials      = &materials[0];
  obj_manager.material_count = 0;

  if (argc != 2) {
    printf("invalid usage: rendeng [scene file name]\n");

    return 0;
  }

  error* err = load_scene(argv[1], &obj_manager);
  if (err != NULL) {
    printf("error: %s\n", err->message);

    free_error(err);
    return 1;
  }

  image* img = new_image(image_width, image_height);
  if (img == NULL) {
    puts("failed to create new image\n");

    return 1;
  }

  // camera object containing information about the camera
  camera camera_object = {//
                          .camera_pos        = null3,
                          .distance_to_lense = 200,
                          .lense_width       = 200,
                          .lense_height      = 200,
                          .rays_per_pixel    = 3
  };
  float total_rays_per_pixel
      = camera_object.rays_per_pixel * camera_object.rays_per_pixel;

  // setting up ray direction calculation needed values
  vector3 lense_iter_horizontal = {//
                                   .x = 0,
                                   .y = camera_object.lense_width / image_width,
                                   .z = 0
  };

  vector3 lense_iter_vertical = {//
                                 .x = 0,
                                 .y = 0,
                                 .z = -camera_object.lense_height / image_height
  };

  vector3 lense_iter_start_pos
      = {.x = camera_object.camera_pos.x + camera_object.distance_to_lense,
         .y = camera_object.camera_pos.y - (camera_object.lense_width / 2),
         .z = camera_object.camera_pos.z + (camera_object.lense_height / 2)};

  vector3 sub_ray_iter_horizontal = null3;
  vector3 sub_ray_iter_vertical   = null3;

  if (camera_object.rays_per_pixel > 1) {
    sub_ray_iter_horizontal = scale3(
        1.0 / (double)camera_object.rays_per_pixel, lense_iter_horizontal
    );
    sub_ray_iter_vertical = scale3(
        1.0 / (double)camera_object.rays_per_pixel, lense_iter_vertical
    );
  }

  // calculate ambient light
  vector3 ambient_light_color = {0, 0, 0};

  for (int i = 0; i < obj_manager.light_object_count; i++) {
    ambient_light_color = add3(
        ambient_light_color, obj_manager.light_objects[i].diffuse_light_color
    );
  }

  ambient_light_color = scale3(0.25, ambient_light_color);

  for (int pixel_pos_y = 0; pixel_pos_y < image_height; pixel_pos_y++) {
    for (int pixel_pos_x = 0; pixel_pos_x < image_width; pixel_pos_x++) {
      // color of the pixel currently being calculated
      // is a vector to simplify calculations and color manipulations
      vector3 pixel_color = {0, 0, 0};

      // point on lense where a ray will pass through
      vector3 lense_point = add3(
          lense_iter_start_pos,
          add3(
              scale3((double)pixel_pos_x, lense_iter_horizontal),
              scale3((double)pixel_pos_y, lense_iter_vertical)
          )
      );

      // calculate multiple rays for each pixel
      for (int ray_y = 0; ray_y < camera_object.rays_per_pixel; ray_y++) {
        for (int ray_x = 0; ray_x < camera_object.rays_per_pixel; ray_x++) {
          // calculate ray direction
          //
          // unit vector calculated for every pixels sub ray.
          // the direction of ray from cameras focus point through lense to
          // objects
          vector3 ray_direction = unit3(sub3(
              add3(
                  lense_point,
                  add3(
                      scale3(ray_x, sub_ray_iter_horizontal),
                      scale3(ray_y, sub_ray_iter_vertical)
                  )
              ),
              camera_object.camera_pos
          ));

          // find distance to closes object if it exists
          closest_object closest_obj = get_closest_object(
              &obj_manager,
              from_vector3(camera_object.camera_pos),
              from_vector3(ray_direction)
          );

          // set current color
          if (closest_obj.distance == -1) {
            pixel_color = add3(pixel_color, background_color);

            continue;
          }

          pixel_color = add3(
              pixel_color,
              get_color_of_point(
                  from_vector3(ray_direction),
                  from_vector3(scale3(closest_obj.distance, ray_direction)),
                  closest_obj,
                  &obj_manager,
                  &camera_object,
                  ambient_light_color,
                  0,
                  0
              )
          );
        }
      }

      set_image_pixel(
          img,
          pixel_pos_x,
          pixel_pos_y,
          color_from_vector3(scale3(1.0 / total_rays_per_pixel, pixel_color))
      );
    }
  }

  save_image(img, "render_result.ppm");
  free_image(img);

  return 0;
}
