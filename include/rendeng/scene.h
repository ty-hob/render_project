#ifndef SCENE_H
#define SCENE_H

#include "error.h"
#include "rendeng/objects.h"

error* load_scene(char* filename, object_manager* objm);

error* load_from_json(scene_object* scene, char* filename);

#endif
