#ifndef IMGEXP
#define IMGEXP


#define MAX_IMAGE_WIDTH 1000
#define MAX_IMAGE_HEIGHT 1000


typedef struct RGB_COLOR{
	float r;
	float g;
	float b;
} rgb_color;


// if rgb color gets out of bounds [0, 1] then find the higes chanel
// and divide every chanel whith it
extern rgb_color normalize_color(rgb_color a);

// simple average between color valuse a and b. change later :)
extern rgb_color merge_colors(rgb_color a, rgb_color b);

// writes the data from struct RGB_COLOR buffer to a image file render_result.ppm
extern void save_image(int image_width, int image_height, rgb_color image[]);



#endif
