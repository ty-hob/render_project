#include "rendeng/image.h"

#include <stdio.h>
#include <stdlib.h>

image* new_image(int width, int height) {
  image* img = malloc(sizeof(image));

  img->width  = width;
  img->height = height;
  img->pixels = calloc(width * height, sizeof(color));
  if (img->pixels == NULL) {
    free(img);
    return NULL;
  }

  return img;
}

void free_image(image* img) {
  free(img->pixels);
  free(img);
}

void set_image_pixel(image* img, int x, int y, color c) {
  img->pixels[y * img->width + x] = c;
}

color get_image_pixel(image* img, int x, int y) {
  return img->pixels[y * img->width + x];
}

void save_image(image* img, const char* filename) {
  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    puts("error while trying to create/open render_result.ppm\n");
    return;
  }

  // writeing file header
  fputs("P3\n", file);
  fprintf(file, "%i %i\n", img->width, img->height);
  fputs("255\n", file);

  // writeing image data to image_file
  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {
      color c = get_image_pixel(img, x, y);

      fprintf(file, "%u %u %u ", c.r, c.g, c.b);
    }
    fputc('\n', file);
  }

  fflush(file);
  fclose(file);
}
