#ifndef VIEW
#define VIEW


#define WIN_WIDTH 800
#define WIN_HEIGHT 800
#define FRAME_RATE 30

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define TOP_VIEW 0
#define FRONT_VIEW 1
#define RIGHT_VIEW 2

#define NONE_EDIT_MODE 0
#define MOVE_EDIT_MODE 1
#define ADD_EDIT_MODE 2
#define REMOVE_EDIT_MODE 3

#define CAMERA_OBJECT 0
#define SPHERE_OBJECT 1


#include <stdbool.h>
#include <SDL.h>
#include "linalg2.h"


typedef struct SDL_WINDOW_DATA{
	SDL_Window* window;
	SDL_Renderer* renderer;

	bool quit_window;
} window_data;


// in renderer
	// x depth red 
	// y horizontal green
	// z vertical blue

// in scedit
	// x horizontal red
	// y depth green
	// z vertical blue


// OBJECT STRUCTS
// camera information
typedef struct CAMERA{
	vect3d focus;
	vect3d lense_center;
	double lense_width;
	double lense_height;
} camera;

// struct containing information about a sphere object
typedef struct SPHERE{
	vect3d center;
	double radius;
	//material* mat;
} sphere;

// structure to contain all scene object types
typedef struct OBJECT{
	void* object_pointer;
	size_t object_type;
} object;

// struc containing data about editable points
typedef struct EDITOR_POINT{
	vect3d* point;
	object* parent_obj;
	double* scalar;
} editor_point;

// sceene object editor
typedef struct EDITOR{
	// current edit mode, how input shoudl be parsed
	size_t edit_mode;
	bool event_left_click;
	bool event_right_click;
	bool event_tab_pressed;
	bool event_q_pressed;

	// alocated array with editable points
	editor_point* edp;
	size_t edp_index;

	// moveing edit mode
	vect3d** moveable_points;
	size_t moveable_points_index;
	// -1 if not moveing else index of moveing point
	int moveing_point;
	vect3d prev_pos;

	// add edit mode
	size_t object_type;



} editor;

// current view data.
typedef struct VIEW{
	// current visable view, (top, front, right)
	size_t current_view;

	// top conter position in scene space
	vect2d pos;
	// zoom
	double zoom;
	double zoom_in_fact;
	double zoom_out_fact;

	// view movment
	double acceleration;
	double deaceleration;
	double velocities[4];
	bool is_accelerateing[4];
}view;


// VARIABLES

// contains data about the sdl window.
extern window_data wd;
// event memory for event handler
extern SDL_Event event;
// time information about next frame
extern Uint32 next_frame_time;
// information about the current view
extern view vw;
// information about the editor (mode, editable objs, ...)
extern editor ed;
// template objects for adding to the sceene
extern camera tmpl_camera;
extern sphere tmpl_sphere;
// malloced array with objs in the sceene
extern object* objs;
// index of objs array
extern size_t objs_index;



// FUNCTIONS

// INITS n QUITS
// inits everything. called before main loop
extern void init();
// creates a sdl window, renderer
extern void init_sdl();
// sets view default variables
extern void init_view();
// sets editor to default settings
extern void init_editor();
// quits everything called after main loop
extern void quit();
// quits sdl window and renderer
extern void quit_sdl();
// frees editors memory
extern void quit_editor();

// WINDOW INPORTANT FUNCS
// handles the windows events. returns false on quit event. else true
extern bool event_handler();
// delays the update of next frame. to a set frame rate
extern void sdl_frame_delay();
// returns a vect 2d with the current mouse pos
extern vect2d get_mouse_pos();

// UPDATE FUNCS
// main loop update function
extern void update();
// updates the view movement
extern void update_view();

extern void update_editor();

// DRAW FUNCS
// draws and presents the next frame
extern void draw();

// ADD SMTHNG FUNCS
// ads and inits camera obj to view
extern void add_camera(camera c);
// adds sphere to sceene objects
extern void add_sphere(sphere s);

// VECTOR SPACE CONVERSION FUNCS
// converts scene position space point to window position space
extern vect2d scene_point_to_window_point(vect3d sp);
// converts a 2d vect window point to 3d vect sceene point dimension value not in current view is set to 0
extern vect3d window_point_to_scene_point(vect2d wp);
// returns a 2d vect form 3d vect with the values that are curently visiable on the view
extern vect2d get_cur_view_vals(vect3d v);
// converts 2d vect to 3d vect depending on curent view
extern vect3d set_cur_view_vals(vect2d v);

#endif