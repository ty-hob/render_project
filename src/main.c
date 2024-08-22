#include "imgexp.h"
#include "linalg.h"
#include "objs.h"
#include "sceene.h"

#include <stdbool.h>
#include <stdio.h>

rgb_color image_buffer[MAX_IMAGE_WIDTH * MAX_IMAGE_HEIGHT];

int main(int argc, char *argv[]) {
  int image_width = 1000;
  int image_height = 1000;

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
  objm.sceene_objects = &sceene_objects[0];
  objm.sceene_object_count = 0;

  objm.sphere_objects = &sphere_objects[0];
  objm.sphere_object_count = 0;

  objm.plane_objects = &plane_objects[0];
  objm.plane_object_count = 0;

  objm.triangle_objects = &triangle_objects[0];
  objm.triangle_object_count = 0;

  objm.light_objects = &light_objects[0];
  objm.light_object_count = 0;

  objm.materials = &materials[0];
  objm.material_count = 0;

  if (argc == 2) {
    load_sceene(argv[1], &objm);
  } else {
    printf("invalid usage: rendeng [scene file name]\n");
    return 0;
  }

  // add_material(&objm, 1, 0, 0, 100, 0, 0, 5);

  // material mat_plane1 = {{ 0,  0,  1}, 100,  0, 0, 5};
  // material mat_plane2 = {{ 0,  1, .5}, 100, .1, 0, 1};
  // material mat_plane3 = {{.8, .8,  0}, 100,  0, 0, 1};

  // material mat_ball1 =  {{ 1,  0,  0}, 100,   0, 1, 3};
  // material mat_ball2 =  {{ 0.1,  0.1,  0.1}, 300,   0.5, 0.5, 3};
  // material mat_ball3 =  {{.1, .1, .3}, 100,  .6, 0, 1};
  // material mat_ball4 =  {{.4, .7, .7}, 100,  .6, 0, 1};

  // material mat_trig1 =  {{.0,  .5, .0}, 10,  0, 0, 0};

  // add_light(&objm, -200, 0, 0, 1, 1, 1, 1, 1, 1);
  // add_light(&objm, -200, 500, 1900, 1, 1, 1, 1, 1, 1);
  //  add_light(&objm, 3000, 500, 1000, 1, 1, 1, 0, 0, 1);
  // add_light(&objm, 200, -1000, 1000);
  // add_triangle(&objm, 400, -50, 0, 400, 50, 0, 400, 0, 50, &mat_trig1,
  // false);

  // add_sphere(&objm,  900,  100,   -100, 100, &mat_ball1); // red
  //  add_sphere(&objm,  400,  0, -100,  50, &mat_ball2); // green
  //  add_sphere(&objm,  600,  150,  150, 100, &mat_ball3); // dark blue
  //  add_sphere(&objm, 1000, -200,    0, 200, &mat_ball4); // light blue
  //  add_sphere(&objm, 1200, 300,    0, 200, &mat_ball4);

  // add_sphere(&objm,  -210,  0,   0, 200, &mat_ball1);

  // add_plane(&objm, 0, 0, -500, 0, 0, 1, &objm.materials[0]);
  // add_plane(&objm, 4000, 0, 0, -1, 0, 0, &mat_plane2);
  // add_plane(&objm, 0, 0, 2200, 0, 0, -1, &mat_plane3);

  // add_triangle(&objm, 498.11217, 154.53818, -2.38847,
  // 565.80329, 57.33352, 50.22591, 425.8098, 57.33352, 48.87036, &mat_trig1,
  // 0); add_triangle(&objm, 498.11217, 154.53818, -2.38847,
  // 425.8098, 57.33352, 48.87036, 501.46356, 57.57082, -73.54529, &mat_trig1,
  // 0); add_triangle(&objm, 497.82237, -32.33669, -2.88491,
  // 565.80329, 57.33352, 50.22591, 425.8098, 57.33352, 48.87036, &mat_trig1,
  // 0); add_triangle(&objm, 425.8098, 57.33352, 48.87036, 501.46356, 57.57082,
  // -73.54529, 497.82237, -32.33669, -2.88491, &mat_trig1, 0);
  // add_triangle(&objm, 497.82237, -32.33669, -2.88491, 501.46356, 57.57082,
  // -73.54529, 565.80329, 57.33352, 50.22591, &mat_trig1, 0);
  // add_triangle(&objm, 498.11217, 154.53818, -2.38847, 501.46356, 57.57082,
  // -73.54529, 565.80329, 57.33352, 50.22591, &mat_trig1, 0);

  // x depth
  // y horizontal
  // z vertical

  // camera object containing information about the camera
  camera camera_object;
  set(&camera_object.camera_pos, 0, 0, 0);
  camera_object.distance_to_lense = 200;
  camera_object.lense_width = 200;
  camera_object.lense_height = 200;
  camera_object.sub_ray_count = 3;

  // setiing up ray directon calculation neede values
  vect3d thro_lense_iter_w;
  set(&thro_lense_iter_w, 0, camera_object.lense_width / image_width, 0);
  vect3d thro_lense_iter_h;
  set(&thro_lense_iter_h, 0, 0, -camera_object.lense_height / image_height);

  vect3d thro_lense_iter_start_pos;
  set(&thro_lense_iter_start_pos,
      camera_object.camera_pos.values[0] + camera_object.distance_to_lense,
      camera_object.camera_pos.values[1] - (camera_object.lense_width / 2),
      camera_object.camera_pos.values[2] + (camera_object.lense_height / 2));

  vect3d sub_ray_iter_w, sub_ray_iter_h;
  fill(&sub_ray_iter_w, 0);
  fill(&sub_ray_iter_h, 0);
  if (camera_object.sub_ray_count > 1) {
    sub_ray_iter_w
        = scale(thro_lense_iter_w, 1.0 / ((float)camera_object.sub_ray_count));
    sub_ray_iter_h
        = scale(thro_lense_iter_h, 1.0 / ((float)camera_object.sub_ray_count));
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

  // untit vect calculate for every image pixel, contains the directon of ray
  vect3d ray_direction;
  vect3d intersection_point;

  closest_object cobj;
  rgb_color pixel_color_sum, point_color;

  // buffer of struct RGB_COLOR containing the data of render images pixels

  // iterative point on lense
  vect3d lense_point;
  for (int height_image_index = 0; height_image_index < image_height;
       height_image_index++) {
    for (int width_image_index = 0; width_image_index < image_width;
         width_image_index++) {

      // reset color
      pixel_color_sum.r = 0;
      pixel_color_sum.g = 0;
      pixel_color_sum.b = 0;

      // point on lense for calculateing main rays
      lense_point = add(
          thro_lense_iter_start_pos,
          add(scale(thro_lense_iter_w, width_image_index),
              scale(thro_lense_iter_h, height_image_index))
      );

      for (int sub_ray_h_index = 0;
           sub_ray_h_index < camera_object.sub_ray_count;
           sub_ray_h_index++) {
        for (int sub_ray_w_index = 0;
             sub_ray_w_index < camera_object.sub_ray_count;
             sub_ray_w_index++) {

          // calculate ray direction
          ray_direction = make_unit_vect(
              camera_object.camera_pos,
              add(lense_point,
                  add(scale(sub_ray_iter_w, sub_ray_w_index),
                      scale(sub_ray_iter_h, sub_ray_h_index)))
          );
          // find distance to closes object if it exists
          cobj = get_closest_object(
              &objm, camera_object.camera_pos, ray_direction
          );

          // set current color
          if (cobj.distance != -1) {

            intersection_point = scale(ray_direction, cobj.distance);

            point_color = get_color_of_point(
                ray_direction,
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
