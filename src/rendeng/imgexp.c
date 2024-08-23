#include "rendeng/imgexp.h"
#include <stdio.h>

rgb_color normalize_color(rgb_color a) {
  if (a.r > a.g) {
    if (a.r > a.b) {
      a.r /= a.r;
      a.g /= a.r;
      a.b /= a.r;
    } else {
      a.r /= a.b;
      a.g /= a.b;
      a.b /= a.b;
    }
  } else {
    if (a.g > a.b) {
      a.r /= a.g;
      a.g /= a.g;
      a.b /= a.g;
    } else {
      a.r /= a.b;
      a.g /= a.b;
      a.b /= a.b;
    }
  }

  return a;
}

rgb_color merge_colors(rgb_color a, rgb_color b) {
  a.r = (a.r + b.r) / 2;
  a.g = (a.g + b.g) / 2;
  a.b = (a.b + b.b) / 2;

  return a;
}

// writes the data from struct RGB_COLOR buffer to a image file
// render_result.ppm
void save_image(int image_width, int image_height, rgb_color image[]) {
  FILE* image_file = fopen("render_result.ppm", "w");
  if (image_file == NULL) {
    puts("error while trying to create/open render_result.ppm\n");
    return;
  }

  // writeing file header
  fputs("P3\n", image_file);
  fprintf(image_file, "%i %i\n", image_width, image_height);
  fputs("255\n", image_file);

  // writeing image data to image_file
  for (int y = 0; y < image_height; y++) {
    for (int x = 0; x < image_width; x++) {
      fprintf(
          image_file,
          "%i %i %i ",
          (int)(image[y * image_width + x].r * 255),
          (int)(image[y * image_width + x].g * 255),
          (int)(image[y * image_width + x].b * 255)
      );
    }
    fputc('\n', image_file);
  }

  fflush(image_file);
  fclose(image_file);
}
