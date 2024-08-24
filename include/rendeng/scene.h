#ifndef SCENE_H
#define SCENE_H


#include "rendeng/error.h"
#include "rendeng/objs.h"

extern error* load_scene(char* filename, object_manager* objm);

#endif
