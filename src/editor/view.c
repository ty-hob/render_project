#include "editor/view.h"

#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>

// in renderer
// x depth red
// y horizontal green
// z vertical blue

// in scedit
// x horizontal red
// y depth green
// z vertical blue

// VARIABLE DEFINITIONS
window_data wd;
SDL_Event event;
Uint32 next_frame_time;
view vw;
editor ed;
camera tmpl_camera;
sphere tmpl_sphere;

object* objs      = NULL;
size_t objs_index = 0;

// FUNCTION DEFINITIONS

//     INITS AND QUITS
// creates a sdl window, renderer
void init_sdl() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("error: sdl couldnt init\n");
  }
  if (SDL_CreateWindowAndRenderer(
          WIN_WIDTH,
          WIN_HEIGHT,
          SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS,
          &wd.window,
          &wd.renderer
      )
      == -1) {
    printf("error: sdl create window and renderer failed\n");
  }
  next_frame_time = SDL_GetTicks();
}

// sets view default values
void init_view() {
  vw.current_view = TOP_VIEW;
  // position in sceen space
  vw.pos.v1 = 0;
  vw.pos.v2 = 0;

  vw.zoom          = 1;
  vw.zoom_in_fact  = 1.1;
  vw.zoom_out_fact = 0.9;

  // movement (not moving)
  vw.acceleration  = 1;
  vw.deaceleration = 1.5;
  for (size_t i = 0; i < 4; i++) {
    vw.is_accelerateing[i] = false;
    vw.velocities[i]       = 0;
  }
}

// sets default values of editor
void init_editor() {
  ed.edit_mode         = MOVE_EDIT_MODE;
  ed.event_left_click  = false;
  ed.event_right_click = false;
  ed.event_tab_pressed = false;
  ed.event_q_pressed   = false;

  ed.edp       = NULL;
  ed.edp_index = 0;

  ed.moveable_points       = NULL;
  ed.moveable_points_index = 0;
  ed.moveing_point         = -1;
  ed.prev_pos              = null_vect3d;

  ed.object_type = 0;

  // template objs
  tmpl_camera.focus           = null_vect3d;
  tmpl_camera.lense_center.v1 = 50;
  tmpl_camera.lense_center.v2 = 50;
  tmpl_camera.lense_center.v3 = 0;
  tmpl_camera.lense_width     = 200;
  tmpl_camera.lense_height    = 100;

  tmpl_sphere.center = null_vect3d;
  tmpl_sphere.radius = 100;
}

// inits everything. called before main loop
void init() {
  init_sdl();
  init_view();
  init_editor();
}

// quits sdl window and renderer
void quit_sdl() {
  SDL_DestroyRenderer(wd.renderer);
  SDL_DestroyWindow(wd.window);
  SDL_Quit();
}

// frees alocated memory by editor
void quit_editor() { free(ed.moveable_points); }

// frees all the memory alocated by sceene objects
void free_objs() {
  for (size_t i = 0; i < objs_index; i++) {
    if (objs[i].object_pointer != NULL) {
      free(objs[i].object_pointer);
    }
  }
  free(objs);
}

// quits everything called after main loop
void quit() {
  free_objs();
  quit_editor();
  quit_sdl();
}

// WINDOW IMPORTANT FUNCTIONS
// for window and events

// handles the windows events. returns false on quit event. else true
bool event_handler() {
  while (SDL_PollEvent(&event)) {
    // quit event
    if (event.type == SDL_QUIT) {
      return false;
      // key pressed
    } else if (event.type == SDL_KEYDOWN) {
      // escape
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        // quit event
        return false;
        // up
      } else if (event.key.keysym.sym == SDLK_w) {
        vw.is_accelerateing[UP] = true;
        // down
      } else if (event.key.keysym.sym == SDLK_s) {
        vw.is_accelerateing[DOWN] = true;
        // left
      } else if (event.key.keysym.sym == SDLK_a) {
        vw.is_accelerateing[LEFT] = true;
        // right
      } else if (event.key.keysym.sym == SDLK_d) {
        vw.is_accelerateing[RIGHT] = true;
        // tab
      } else if (event.key.keysym.sym == SDLK_TAB) {
        ed.event_tab_pressed = true;
        // q
      } else if (event.key.keysym.sym == SDLK_q) {
        ed.event_q_pressed = true;
      }
      // key released
    } else if (event.type == SDL_KEYUP) {
      // up
      if (event.key.keysym.sym == SDLK_w) {
        vw.is_accelerateing[UP] = false;
        // down
      } else if (event.key.keysym.sym == SDLK_s) {
        vw.is_accelerateing[DOWN] = false;
        // left
      } else if (event.key.keysym.sym == SDLK_a) {
        vw.is_accelerateing[LEFT] = false;
        // right
      } else if (event.key.keysym.sym == SDLK_d) {
        vw.is_accelerateing[RIGHT] = false;
      } else if (event.key.keysym.sym == SDLK_1) {
        vw.current_view = TOP_VIEW;
      } else if (event.key.keysym.sym == SDLK_2) {
        vw.current_view = FRONT_VIEW;
      } else if (event.key.keysym.sym == SDLK_3) {
        vw.current_view = RIGHT_VIEW;
      }
      // mouse wheel scroll
    } else if (event.type == SDL_MOUSEWHEEL) {
      if (event.wheel.y > 0) { // scroll up
        // zoom in
        vw.zoom *= vw.zoom_in_fact;
      } else if (event.wheel.y < 0) { // scroll down
        // zoom out
        vw.zoom *= vw.zoom_out_fact;
      }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        ed.event_left_click = true;
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        ed.event_right_click = true;
      }
    }
  }
  return true;
}

// delays the update of next frame. to a set frame rate
void sdl_frame_delay() {
  Uint32 now = SDL_GetTicks();
  if (next_frame_time > now) {
    SDL_Delay(next_frame_time - now);
  }
  next_frame_time += FRAME_RATE;
}

// returns a vect 2d with the current mouse pos
vect2d get_mouse_pos() {
  int x, y;
  SDL_GetMouseState(&x, &y);
  vect2d p = {(int)x, (int)y};
  return p;
}

// VECTOR SPACE CONVERSION FUNCTIONS

// scene space to window(window drawing) space conversion function
vect2d scene_point_to_window_point(vect3d sp) {
  // window point
  vect2d wp = get_cur_view_vals(sp);

  wp.v1 = wp.v1 - vw.pos.v1;
  wp.v2 = wp.v2 - vw.pos.v2;

  wp.v1 *= vw.zoom;
  wp.v2 *= vw.zoom;

  wp.v1 += WIN_WIDTH / 2;
  wp.v2 = (WIN_HEIGHT / 2) - wp.v2;

  return wp;
}

// converts a 2d vect window point to 3d vect sceene point
// dimension value not in current view is set to 0
vect3d window_point_to_scene_point(vect2d wp) {
  wp.v1 -= WIN_WIDTH / 2;
  wp.v2 = (WIN_HEIGHT / 2) - wp.v2;

  wp = scale2d(wp, 1 / vw.zoom);

  wp = add2d(wp, vw.pos);

  return set_cur_view_vals(wp);
}

// returns a 2d vect form 3d vect with the values that are curently
// visiable on the view
vect2d get_cur_view_vals(vect3d v) {
  vect2d c;
  if (vw.current_view == TOP_VIEW) {
    c.v1 = v.v1;
    c.v2 = v.v2;
  } else if (vw.current_view == FRONT_VIEW) {
    c.v1 = v.v1;
    c.v2 = v.v3;
  } else if (vw.current_view == RIGHT_VIEW) {
    c.v1 = v.v2;
    c.v2 = v.v3;
  }
  return c;
}

// converts 2d vect to 3d vect depending on curent view
vect3d set_cur_view_vals(vect2d v) {
  vect3d c = {0, 0, 0};
  if (vw.current_view == TOP_VIEW) {
    c.v1 = v.v1;
    c.v2 = v.v2;
  } else if (vw.current_view == FRONT_VIEW) {
    c.v1 = v.v1;
    c.v3 = v.v2;
  } else if (vw.current_view == RIGHT_VIEW) {
    c.v2 = v.v1;
    c.v3 = v.v2;
  }
  return c;
}

// UPDATE FUNCTIONS

void update_view() {
  for (size_t i = 0; i < 4; i++) {
    if (vw.is_accelerateing[i]) {
      vw.velocities[i] += vw.acceleration;
    } else {
      if (vw.velocities[i] > 0) {
        vw.velocities[i] -= vw.deaceleration;
      } else {
        vw.velocities[i] = 0;
      }
    }
  }
  vw.pos.v1 += (vw.velocities[RIGHT] - vw.velocities[LEFT]) / vw.zoom;
  vw.pos.v2 += (vw.velocities[UP] - vw.velocities[DOWN]) / vw.zoom;
}

void update_editor() {
  // switch edit modes
  if (ed.event_tab_pressed == true) {
    ed.edit_mode         = (ed.edit_mode + 1) % 4;
    ed.event_tab_pressed = false;
  }
  // parse input acordint to current edit mode
  if (ed.edit_mode == MOVE_EDIT_MODE) {
    if (ed.event_left_click == true) {
      // start moveing point, select it
      if (ed.moveing_point == -1) {
        vect2d mp = get_mouse_pos();
        for (size_t i = 0; i < ed.edp_index; i++) {
          if (ed.edp[i].point != NULL) {
            if (length2d(
                    sub2d(mp, scene_point_to_window_point(*(ed.edp[i].point)))
                )
                < 5) {
              ed.prev_pos      = *(ed.edp[i].point);
              ed.moveing_point = i;
              break;
            }
          }
        }
        // end moveing point
      } else {
        ed.moveing_point = -1;
      }

      ed.event_left_click = false;
    } else {
      if (ed.moveing_point != -1) {
        *(ed.edp[ed.moveing_point].point) = add3d(
            window_point_to_scene_point(get_mouse_pos()),
            sub3d(
                ed.prev_pos, set_cur_view_vals(get_cur_view_vals(ed.prev_pos))
            )
        );
      }
    }
    if (ed.event_right_click == true) {

      if (ed.moveing_point != -1) {
        *(ed.edp[ed.moveing_point].point) = ed.prev_pos;
        ed.moveing_point                  = -1;
      }

      ed.event_right_click = false;
    }
  } else if (ed.edit_mode == ADD_EDIT_MODE) {
    if (ed.event_q_pressed == true) {
      ed.object_type     = (ed.object_type + 1) % 2;
      ed.event_q_pressed = false;
    }
    if (ed.object_type == CAMERA_OBJECT) {
      tmpl_camera.focus = window_point_to_scene_point(get_mouse_pos());
      tmpl_camera.lense_center
          = add3d(tmpl_camera.focus, scale3d(add3d(i_hat, j_hat), 50));
    } else if (ed.object_type == SPHERE_OBJECT) {
      tmpl_sphere.center = window_point_to_scene_point(get_mouse_pos());
    }
    // add object to sceene
    if (ed.event_left_click) {
      if (ed.object_type == CAMERA_OBJECT) {
        add_camera(tmpl_camera);
      } else if (ed.object_type == SPHERE_OBJECT) {
        add_sphere(tmpl_sphere);
      }
      ed.event_left_click = false;
    }
  } else if (ed.edit_mode == REMOVE_EDIT_MODE) {
    if (ed.event_left_click == true) {
      vect2d mp = get_mouse_pos();
      for (size_t i = 0; i < ed.edp_index; i++) {
        if (ed.edp[i].point != NULL) {
          if (length2d(
                  sub2d(mp, scene_point_to_window_point(*(ed.edp[i].point)))
              )
              < 5) {

            // delete point parent
            printf("free\n");

            print_vect3d(
                ((sphere*)((ed.edp[i].parent_obj)->object_pointer))->center
            );

            // free((ed.edp[i].parent_obj)->object_pointer);
            (ed.edp[i].parent_obj)->object_pointer = NULL;

            // delete editor point
            ed.edp[i].point      = NULL;
            ed.edp[i].parent_obj = NULL;
            ed.edp[i].scalar     = NULL;

            break;
          }
        }
      }

      ed.event_left_click = false;
    }
  }
}

// all incompasing update. called by main loop
void update() {
  update_view();
  update_editor();
}

// ADD SOMETHING FUNCTIONS

object* add_obj(void* obj_prt, size_t obj_type) {
  size_t i;
  for (i = 0; i < objs_index; i++) {
    if (objs[i].object_pointer == NULL) {
      break;
    }
  }
  if (i == objs_index) {
    objs = realloc(objs, (objs_index + 1) * sizeof(object));
    if (objs == NULL) {
      printf("realloc failed\n");
      return NULL;
    }
    objs[objs_index].object_pointer = obj_prt;
    objs[objs_index].object_type    = obj_type;
    objs_index += 1;
    return objs + (objs_index - 1);
  } else {
    // found empty space in array
    objs[i].object_pointer = obj_prt;
    objs[i].object_type    = obj_type;
    return objs + i;
  }
}

// adds new point to editor
void add_editor_point(object* parent, vect3d* p, double* s) {
  // check for empty spaces in edp array
  size_t i;
  for (i = 0; i < ed.edp_index; i++) {
    if (ed.edp[i].point == NULL) {
      // found empty space
      break;
    }
  }
  if (i == ed.edp_index) {
    // alocate new space
    ed.edp = realloc(ed.edp, (ed.edp_index + 1) * sizeof(editor_point));
    if (ed.edp == NULL) {
      printf("realloc failed\n");
      return;
    }
    ed.edp[ed.edp_index].parent_obj = parent;
    ed.edp[ed.edp_index].point      = p;
    ed.edp[ed.edp_index].scalar     = s;
    ed.edp_index += 1;
  } else {
    // fill space with index i
    ed.edp[i].parent_obj = parent;
    ed.edp[i].point      = p;
    ed.edp[i].scalar     = s;
  }
}

// ads and inits camera obj to view
void add_camera(camera c) {
  camera* cam_prt = malloc(sizeof(camera));
  if (cam_prt == NULL) {
    printf("malloc failed\n");
    return;
  }
  *cam_prt        = c;
  object* obj_prt = add_obj(cam_prt, CAMERA_OBJECT);
  add_editor_point(obj_prt, &(cam_prt->focus), NULL);
  add_editor_point(obj_prt, &(cam_prt->lense_center), NULL);
}

// adds sphere to scene objects
void add_sphere(sphere s) {
  sphere* sph_prt = malloc(sizeof(sphere));
  if (sph_prt == NULL) {
    printf("malloc failed\n");
    return;
  }
  *sph_prt        = s;
  object* obj_prt = add_obj(sph_prt, SPHERE_OBJECT);
  add_editor_point(obj_prt, &(sph_prt->center), NULL);
}

// DRAWING CODE functions for drawing things on the screen
// only draw() should be called in main loop. everything else is in
// smaller functions that are called by draw()

// draws a line in current view form vect 3d v1 to vect3d v2
void draw_line(vect3d v1, vect3d v2) {
  vect2d p1 = scene_point_to_window_point(v1);
  vect2d p2 = scene_point_to_window_point(v2);
  lineRGBA(
      wd.renderer,
      (int)p1.v1,
      (int)p1.v2,
      (int)p2.v1,
      (int)p2.v2,
      255,
      255,
      255,
      255
  );
}

// draws the camera
void draw_camera(camera c) {
  draw_line(c.focus, c.lense_center);

  vect3d hh = scale3d(
      direction3d(cross_prod3d(sub3d(c.lense_center, c.focus), j_hat)),
      c.lense_height / 2
  );
  vect3d hw = scale3d(
      direction3d(cross_prod3d(sub3d(c.lense_center, c.focus), k_hat)),
      c.lense_width / 2
  );

  vect3d coner1 = add3d(add3d(c.lense_center, hh), scale3d(hw, -1));
  vect3d coner2 = add3d(add3d(c.lense_center, hh), hw);
  vect3d coner3
      = add3d(add3d(c.lense_center, scale3d(hh, -1)), scale3d(hw, -1));
  vect3d coner4 = add3d(add3d(c.lense_center, scale3d(hh, -1)), hw);

  draw_line(coner1, coner2);
  draw_line(coner1, coner3);
  draw_line(coner4, coner3);
  draw_line(coner4, coner2);

  draw_line(c.focus, coner1);
  draw_line(c.focus, coner2);
  draw_line(c.focus, coner3);
  draw_line(c.focus, coner4);
}

// draws sphere in the current view
void draw_sphere(sphere s) {
  vect2d c;
  c = scene_point_to_window_point(s.center);
  circleRGBA(
      wd.renderer,
      (int)c.v1,
      (int)c.v2,
      (int)s.radius * vw.zoom,
      255,
      255,
      255,
      255
  );
}

// draws all the objects in curent sceene
void draw_objs() {
  for (size_t i = 0; i < objs_index; i++) {
    if (objs[i].object_pointer != NULL) {
      if (objs[i].object_type == CAMERA_OBJECT) {
        draw_camera(*((camera*)objs[i].object_pointer));
      } else if (objs[i].object_type == SPHERE_OBJECT) {
        draw_sphere(*((sphere*)objs[i].object_pointer));
      }
    }
  }
}

// draws the x and y axis
void draw_axis() {
  char dist_marker[10];
  vect3d tick_pos;
  vect2d c, d;
  c = scene_point_to_window_point(null_vect3d);
  if (vw.current_view == TOP_VIEW) {
    stringRGBA(wd.renderer, 5, 5, "top view", 255, 255, 255, 255); // view name

    // x axis, horizontal, red
    lineRGBA(
        wd.renderer, 0, (int)c.v2, WIN_WIDTH, (int)c.v2, 255, 0, 0, 255
    ); // axis
    // arrow
    lineRGBA(
        wd.renderer,
        WIN_WIDTH - 10,
        (int)c.v2 - 5,
        WIN_WIDTH,
        (int)c.v2,
        255,
        0,
        0,
        255
    ); // arrow
    lineRGBA(
        wd.renderer,
        WIN_WIDTH - 10,
        (int)c.v2 + 5,
        WIN_WIDTH,
        (int)c.v2,
        255,
        0,
        0,
        255
    );
    // dist marker tick and value
    lineRGBA(
        wd.renderer,
        WIN_WIDTH - 47,
        (int)c.v2 - 5,
        WIN_WIDTH - 47,
        (int)c.v2 + 5,
        255,
        0,
        0,
        255
    );
    d.v1     = WIN_WIDTH - 47;
    d.v2     = (int)c.v2;
    tick_pos = window_point_to_scene_point(d);
    sprintf(dist_marker, "%d ", (int)tick_pos.v1);
    stringRGBA(
        wd.renderer, WIN_WIDTH - 45, (int)c.v2 - 12, dist_marker, 255, 0, 0, 255
    );

    // y axis, vertical, green
    lineRGBA(wd.renderer, (int)c.v1, 0, (int)c.v1, WIN_HEIGHT, 0, 255, 0, 255);
    // arrow
    lineRGBA(wd.renderer, (int)c.v1 - 5, 10, (int)c.v1, 0, 0, 255, 0, 255);
    lineRGBA(wd.renderer, (int)c.v1 + 5, 10, (int)c.v1, 0, 0, 255, 0, 255);
    // dist marker and val
    lineRGBA(wd.renderer, (int)c.v1 - 5, 47, (int)c.v1 + 5, 47, 0, 255, 0, 255);
    d.v1     = (int)c.v1;
    d.v2     = 47;
    tick_pos = window_point_to_scene_point(d);
    sprintf(dist_marker, "%d ", (int)tick_pos.v2);
    stringRGBA(wd.renderer, (int)c.v1 + 6, 39, dist_marker, 0, 255, 0, 255);
  } else if (vw.current_view == FRONT_VIEW) {
    stringRGBA(wd.renderer, 5, 5, "front view", 255, 255, 255, 255);

    // x axis, horizontal, red
    lineRGBA(wd.renderer, 0, (int)c.v2, WIN_WIDTH, (int)c.v2, 255, 0, 0, 255);
    // arrow
    lineRGBA(
        wd.renderer,
        WIN_WIDTH - 10,
        (int)c.v2 - 5,
        WIN_WIDTH,
        (int)c.v2,
        255,
        0,
        0,
        255
    );
    lineRGBA(
        wd.renderer,
        WIN_WIDTH - 10,
        (int)c.v2 + 5,
        WIN_WIDTH,
        (int)c.v2,
        255,
        0,
        0,
        255
    );
    // marker
    lineRGBA(
        wd.renderer,
        WIN_WIDTH - 47,
        (int)c.v2 - 5,
        WIN_WIDTH - 47,
        (int)c.v2 + 5,
        255,
        0,
        0,
        255
    );
    d.v1     = WIN_WIDTH - 47;
    d.v2     = (int)c.v2;
    tick_pos = window_point_to_scene_point(d);
    sprintf(dist_marker, "%d ", (int)tick_pos.v1);
    stringRGBA(
        wd.renderer, WIN_WIDTH - 45, (int)c.v2 - 12, dist_marker, 255, 0, 0, 255
    );

    // z axis, vertical, blue
    lineRGBA(wd.renderer, (int)c.v1, 0, (int)c.v1, WIN_HEIGHT, 0, 0, 255, 255);

    lineRGBA(wd.renderer, (int)c.v1 - 5, 10, (int)c.v1, 0, 0, 0, 255, 255);
    lineRGBA(wd.renderer, (int)c.v1 + 5, 10, (int)c.v1, 0, 0, 0, 255, 255);
    // dist marker and val
    lineRGBA(wd.renderer, (int)c.v1 - 5, 47, (int)c.v1 + 5, 47, 0, 0, 255, 255);
    d.v1     = (int)c.v1;
    d.v2     = 47;
    tick_pos = window_point_to_scene_point(d);
    sprintf(dist_marker, "%d ", (int)tick_pos.v3);
    stringRGBA(wd.renderer, (int)c.v1 + 6, 39, dist_marker, 0, 0, 255, 255);

  } else if (vw.current_view == RIGHT_VIEW) {
    stringRGBA(wd.renderer, 5, 5, "right view", 255, 255, 255, 255);
    // y axis, horizontal, green
    lineRGBA(wd.renderer, 0, (int)c.v2, WIN_WIDTH, (int)c.v2, 0, 255, 0, 255);
    // arrow
    lineRGBA(
        wd.renderer,
        WIN_WIDTH - 10,
        (int)c.v2 - 5,
        WIN_WIDTH,
        (int)c.v2,
        0,
        255,
        0,
        255
    );
    lineRGBA(
        wd.renderer,
        WIN_WIDTH - 10,
        (int)c.v2 + 5,
        WIN_WIDTH,
        (int)c.v2,
        0,
        255,
        0,
        255
    );
    // marker
    lineRGBA(
        wd.renderer,
        WIN_WIDTH - 47,
        (int)c.v2 - 5,
        WIN_WIDTH - 47,
        (int)c.v2 + 5,
        0,
        255,
        0,
        255
    );
    d.v1     = WIN_WIDTH - 47;
    d.v2     = (int)c.v2;
    tick_pos = window_point_to_scene_point(d);
    sprintf(dist_marker, "%d ", (int)tick_pos.v2);
    stringRGBA(
        wd.renderer, WIN_WIDTH - 45, (int)c.v2 - 12, dist_marker, 0, 255, 0, 255
    );

    // z axis, vertical, blue
    lineRGBA(wd.renderer, (int)c.v1, 0, (int)c.v1, WIN_HEIGHT, 0, 0, 255, 255);
    // arrow
    lineRGBA(wd.renderer, (int)c.v1 - 5, 10, (int)c.v1, 0, 0, 0, 255, 255);
    lineRGBA(wd.renderer, (int)c.v1 + 5, 10, (int)c.v1, 0, 0, 0, 255, 255);
    // dist marker and val
    lineRGBA(wd.renderer, (int)c.v1 - 5, 47, (int)c.v1 + 5, 47, 0, 0, 255, 255);
    d.v1     = (int)c.v1;
    d.v2     = 47;
    tick_pos = window_point_to_scene_point(d);
    sprintf(dist_marker, "%d ", (int)tick_pos.v3);
    stringRGBA(wd.renderer, (int)c.v1 + 6, 39, dist_marker, 0, 0, 255, 255);
  }
  // d.v1 = WIN_WIDTH-47;
  // d.v2 = (int)c.v2;
  // print_vect3d(window_point_to_scene_point(d));
  // sprintf(dist_marker, "%d ", (int)length3d(window_point_to_scene_point(d)));

  // stringRGBA(wd.renderer, WIN_WIDTH-45, (int)c.v2-12, dist_marker, 255, 255,
  // 255, 255); black pixel in axis crosing
  // pixelRGBA(wd.renderer, (int)c.v1, (int)c.v2, 0, 0, 0, 255);
}

void draw_editor() {
  // mode
  if (ed.edit_mode == NONE_EDIT_MODE) {
    stringRGBA(wd.renderer, 5, 20, "mode: none", 255, 255, 255, 255);
  } else if (ed.edit_mode == MOVE_EDIT_MODE) {
    stringRGBA(wd.renderer, 5, 20, "mode: move", 255, 255, 255, 255);
  } else if (ed.edit_mode == ADD_EDIT_MODE) {
    if (ed.object_type == CAMERA_OBJECT) {
      stringRGBA(wd.renderer, 5, 20, "mode: add - camera", 255, 255, 255, 255);
    } else if (ed.object_type == SPHERE_OBJECT) {
      stringRGBA(wd.renderer, 5, 20, "mode: add - sphere", 255, 255, 255, 255);
    }
  } else if (ed.edit_mode == REMOVE_EDIT_MODE) {
    stringRGBA(wd.renderer, 5, 20, "mode: remove", 255, 255, 255, 255);
  }
  // draw movable points
  if (ed.edit_mode == MOVE_EDIT_MODE) {
    vect2d wp;
    for (size_t i = 0; i < ed.edp_index; i++) {
      if (ed.edp[i].point != NULL) {
        wp = scene_point_to_window_point(*(ed.edp[i].point));
        circleRGBA(wd.renderer, (int)wp.v1, (int)wp.v2, 5, 0, 230, 230, 255);
      }
    }
  } else if (ed.edit_mode == ADD_EDIT_MODE) {
    // draw the selected object
    if (ed.object_type == CAMERA_OBJECT) {
      draw_camera(tmpl_camera);
    } else if (ed.object_type == SPHERE_OBJECT) {
      draw_sphere(tmpl_sphere);
    }
  } else if (ed.edit_mode == REMOVE_EDIT_MODE) {
    // draw removable object points

    vect2d wp;
    for (size_t i = 0; i < ed.edp_index; i++) {
      if (ed.edp[i].point != NULL) {
        wp = scene_point_to_window_point(*(ed.edp[i].point));
        circleRGBA(wd.renderer, (int)wp.v1, (int)wp.v2, 5, 200, 0, 0, 255);
      }
    }

    // draw x on mouse
    int x, y;
    SDL_GetMouseState(&x, &y);
    lineRGBA(wd.renderer, x - 6, y - 6, x + 6, y + 6, 200, 20, 20, 255);
    lineRGBA(wd.renderer, x + 6, y - 6, x - 6, y + 6, 200, 20, 20, 255);
  }
}

// draws and presents the next frame
void draw() {
  // background
  SDL_SetRenderDrawColor(wd.renderer, 40, 40, 40, 255);
  SDL_RenderFillRect(wd.renderer, NULL);

  draw_axis();
  draw_editor();

  draw_objs();

  SDL_RenderPresent(wd.renderer);
}
