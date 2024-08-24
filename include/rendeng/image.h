#ifndef IMAGE_H
#define IMAGE_H

#include "vector.h"

// RGB color
typedef struct COLOR {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} color;

// a 2d array of pixels with a given width and height
typedef struct IMAGE {
  int width;
  int height;
  color* pixels; // array of size width * height
} image;

// creates a new image with the given width and height. the caller must free
// the image with free_image
extern image* new_image(int width, int height);

// frees the memory of the given image
extern void free_image(image* img);

// sets the pixel at the given x and y coordinates to the given color
extern void set_image_pixel(image* img, int x, int y, color c);

// gets the pixel at the given x and y coordinates
extern color get_image_pixel(image* img, int x, int y);

// converts a vector3 to a color
// the vector3 must have values between 0 and 1
// x = r, y = g, z = b
extern color color_from_vector3(vector3 v);

// writes the given image to the given file in the PPM format
extern void save_image(image* img, const char* filename);

#endif
