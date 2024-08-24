#include <stdbool.h>
#include <stdio.h>

#include "rendeng/error.h"
#include "rendeng/imgexp.h"
#include "rendeng/linalg.h"
#include "rendeng/objs.h"
#include "rendeng/scene.h"
#include "vector.h"

rgb_color image_buffer[MAX_IMAGE_WIDTH * MAX_IMAGE_HEIGHT];

int main(int argc, char* argv[]) {
  int image_width  = 200;
  int image_height = 200;

  // error check if the image size is not to big
  if (image_width > MAX_IMAGE_WIDTH) {
    puts("chosen image width is larger than maximum allowed value");
    return 0;
  }

  if (image_height > MAX_IMAGE_HEIGHT) {
    puts("chosen image height is larger than maximum allowed value");
    return 0;
  }
  // random use defined colors
  rgb_color background_color = {.5, .5, .5};

  // scene object storage
  sphere sphere_objects[MAX_SPHERE_OBJECTS];

  plane plane_objects[MAX_PLANE_OBJECTS];

  triangle triangle_objects[MAX_TRIANGLE_OBJECTS];

  light light_objects[MAX_LIGHT_OBJECTS];

  material materials[MAX_MATERIALS];

  object sceene_objects
      [MAX_SPHERE_OBJECTS + MAX_TRIANGLE_OBJECTS + MAX_PLANE_OBJECTS
       + MAX_TRIANGLE_OBJECTS];

  // object manager setup
  object_manager objm;
  objm.sceene_objects      = &sceene_objects[0];
  objm.sceene_object_count = 0;

  objm.sphere_objects      = &sphere_objects[0];
  objm.sphere_object_count = 0;

  objm.plane_objects      = &plane_objects[0];
  objm.plane_object_count = 0;

  objm.triangle_objects      = &triangle_objects[0];
  objm.triangle_object_count = 0;

  objm.light_objects      = &light_objects[0];
  objm.light_object_count = 0;

  objm.materials      = &materials[0];
  objm.material_count = 0;

  if (argc != 2) {
    printf("invalid usage: rendeng [scene file name]\n");

    return 0;
  }

  error* err = load_scene(argv[1], &objm);
  if (err != NULL) {
    printf("error: %s\n", err->message);

    free_error(err);
    return 0;
  }

  // camera object containing information about the camera
  camera camera_object = {//
                          .camera_pos        = {0, 0, 0},
                          .distance_to_lense = 200,
                          .lense_width       = 200,
                          .lense_height      = 200,
                          .sub_ray_count     = 3
  };
  // set(&camera_object.camera_pos, 0, 0, 0);
  // camera_object.distance_to_lense = 200;
  // camera_object.lense_width       = 200;
  // camera_object.lense_height      = 200;
  // camera_object.sub_ray_count     = 3;

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

  // vect3d thro_lense_iter_w;
  // set(&thro_lense_iter_w, 0, camera_object.lense_width / image_width, 0);
  // vect3d thro_lense_iter_h;
  // set(&thro_lense_iter_h, 0, 0, -camera_object.lense_height / image_height);
  //
  // vect3d thro_lense_iter_start_pos;
  // set(&thro_lense_iter_start_pos,
  //     camera_object.camera_pos.values[0] + camera_object.distance_to_lense,
  //     camera_object.camera_pos.values[1] - (camera_object.lense_width / 2),
  //     camera_object.camera_pos.values[2] + (camera_object.lense_height / 2));

  vector3 sub_ray_iter_horizontal = {0, 0, 0};
  vector3 sub_ray_iter_vertical   = {0, 0, 0};

  // vect3d sub_ray_iter_w, sub_ray_iter_h;
  // fill(&sub_ray_iter_w, 0);
  // fill(&sub_ray_iter_h, 0);

  if (camera_object.sub_ray_count > 1) {
    // sub_ray_iter_w
    //     = scale(thro_lense_iter_w, 1.0 /
    //     ((float)camera_object.sub_ray_count));
    // sub_ray_iter_h
    //     = scale(thro_lense_iter_h, 1.0 /
    //     ((float)camera_object.sub_ray_count));

    sub_ray_iter_horizontal = scale3(
        lense_iter_horizontal, 1.0 / ((double)camera_object.sub_ray_count)
    );
    sub_ray_iter_vertical = scale3(
        lense_iter_vertical, 1.0 / ((double)camera_object.sub_ray_count)
    );
  }

  // random use variable used for loops
  int i;

  // calculate ambient light
  rgb_color ambient_light;
  if (objm.light_object_count > 0) {
    ambient_light.r = objm.light_objects[0].diffuse_light_color.r;
    ambient_light.g = objm.light_objects[0].diffuse_light_color.g;
    ambient_light.b = objm.light_objects[0].diffuse_light_color.b;
  }
  for (i = 1; i < objm.light_object_count; i++) {
    ambient_light.r += objm.light_objects[i].diffuse_light_color.r;
    ambient_light.g += objm.light_objects[i].diffuse_light_color.g;
    ambient_light.b += objm.light_objects[i].diffuse_light_color.b;
  }
  ambient_light.r *= 0.25;
  ambient_light.g *= 0.25;
  ambient_light.b *= 0.25;

  vect3d intersection_point;

  closest_object cobj;
  rgb_color pixel_color_sum, point_color;

  // buffer of struct RGB_COLOR containing the data of render images pixels

  for (int height_image_index = 0; height_image_index < image_height;
       height_image_index++) {
    for (int width_image_index = 0; width_image_index < image_width;
         width_image_index++) {

      // reset color
      pixel_color_sum.r = 0;
      pixel_color_sum.g = 0;
      pixel_color_sum.b = 0;

      // point on lense for calculateing main rays
      vector3 lense_point = add3(
          lense_iter_start_pos,
          add3(
              scale3(lense_iter_horizontal, (double)width_image_index),
              scale3(lense_iter_vertical, (double)height_image_index)
          )
      );

      // lense_point = add(
      //     thro_lense_iter_start_pos,
      //     add(scale(thro_lense_iter_w, width_image_index),
      //         scale(thro_lense_iter_h, height_image_index))
      // );
      //
      for (int sub_ray_h_index = 0;
           sub_ray_h_index < camera_object.sub_ray_count;
           sub_ray_h_index++) {
        for (int sub_ray_w_index = 0;
             sub_ray_w_index < camera_object.sub_ray_count;
             sub_ray_w_index++) {
          // calculate ray direction
          //
          // unit vector calculate for every image pixel, contains the direction
          // of ray
          vector3 ray_direction = unit3(sub3(
              add3(
                  lense_point,
                  add3(
                      scale3(sub_ray_iter_horizontal, sub_ray_w_index),
                      scale3(sub_ray_iter_vertical, sub_ray_h_index)
                  )
              ),
              camera_object.camera_pos
          ));

          //  = make_unit_vect(
          //     camera_object.camera_pos,
          //     add(lense_point,
          //         add(scale(sub_ray_iter_w, sub_ray_w_index),
          //             scale(sub_ray_iter_h, sub_ray_h_index)))
          // );

          // find distance to closes object if it exists
          cobj = get_closest_object(
              &objm,
              from_vector3(camera_object.camera_pos),
              from_vector3(ray_direction)
          );

          // set current color
          if (cobj.distance != -1) {

            intersection_point
                = scale(from_vector3(ray_direction), cobj.distance);

            point_color = get_color_of_point(
                from_vector3(ray_direction),
                intersection_point,
                cobj,
                &objm,
                &camera_object,
                ambient_light,
                0,
                0
            );

            pixel_color_sum.r += point_color.r;
            pixel_color_sum.g += point_color.g;
            pixel_color_sum.b += point_color.b;

          } else {
            // set pixle to background color and continue to next pixel
            pixel_color_sum.r = background_color.r * camera_object.sub_ray_count
                              * camera_object.sub_ray_count;
            pixel_color_sum.g = background_color.g * camera_object.sub_ray_count
                              * camera_object.sub_ray_count;
            pixel_color_sum.b = background_color.b * camera_object.sub_ray_count
                              * camera_object.sub_ray_count;
            sub_ray_h_index = camera_object.sub_ray_count;
            sub_ray_w_index = camera_object.sub_ray_count;
          }
        }
      }

      image_buffer[height_image_index * image_width + width_image_index].r
          = pixel_color_sum.r
          / (camera_object.sub_ray_count * camera_object.sub_ray_count);
      image_buffer[height_image_index * image_width + width_image_index].g
          = pixel_color_sum.g
          / (camera_object.sub_ray_count * camera_object.sub_ray_count);
      image_buffer[height_image_index * image_width + width_image_index].b
          = pixel_color_sum.b
          / (camera_object.sub_ray_count * camera_object.sub_ray_count);
    }
  }

  // write image buffer to ppm file named: render_result.ppm
  save_image(image_width, image_height, image_buffer);

  return 0;
}
