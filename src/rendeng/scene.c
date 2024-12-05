#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs/cJSON.h"
#include "rendeng/objects.h"
#include "rendeng/scene.h"

char* read_file(char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    fclose(file);
    return NULL;
  }

  long length = ftell(file);

  if (fseek(file, 0, SEEK_SET) != 0) {
    fclose(file);
    return NULL;
  }

  char* buffer = calloc(length + 1, sizeof(char));
  if (buffer == NULL) {
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, length, file);
  buffer[length] = '\0';

  fclose(file);
  return buffer;
}

error* parse_vector3(cJSON* root, vector3* v) {
  cJSON* x = cJSON_GetObjectItemCaseSensitive(root, "x");
  if (!cJSON_IsNumber(x)) {
    return new_error("x not found in vector3 object (.x)");
  }

  cJSON* y = cJSON_GetObjectItemCaseSensitive(root, "y");
  if (!cJSON_IsNumber(y)) {
    return new_error("y not found in vector3 object (.y)");
  }

  cJSON* z = cJSON_GetObjectItemCaseSensitive(root, "z");
  if (!cJSON_IsNumber(z)) {
    return new_error("z not found in vector3 object (.z)");
  }

  v->x = x->valuedouble;
  v->y = y->valuedouble;
  v->z = z->valuedouble;

  return NULL;
}

error* parse_camera(cJSON* root, camera_object* camera) {
  error* err;

  cJSON* focus = cJSON_GetObjectItemCaseSensitive(root, "focus");
  if (!cJSON_IsObject(focus)) {
    return new_error("focus not found in camera object (.camera.focus)");
  }

  err = parse_vector3(focus, &camera->focus);
  if (err != NULL) {
    return err;
  }

  cJSON* lense_center = cJSON_GetObjectItemCaseSensitive(root, "lense_center");
  if (!cJSON_IsObject(lense_center)) {
    return new_error(
        "lense center not found in camera object (.camera.lense_center)"
    );
  }

  err = parse_vector3(lense_center, &camera->lense_center);
  if (err != NULL) {
    return err;
  }

  cJSON* lense_width = cJSON_GetObjectItemCaseSensitive(root, "lense_width");
  if (!cJSON_IsNumber(lense_width)) {
    return new_error(
        "lense width not found in camera object (.camera.lense_width)"
    );
  }

  camera->lense_width = lense_width->valuedouble;

  cJSON* lense_height = cJSON_GetObjectItemCaseSensitive(root, "lense_height");
  if (!cJSON_IsNumber(lense_height)) {
    return new_error(
        "lense height not found in camera object (.camera.lense_height)"
    );
  }

  camera->lense_height = lense_height->valuedouble;

  cJSON* lense_pixel_density
      = cJSON_GetObjectItemCaseSensitive(root, "lense_pixel_density");
  if (!cJSON_IsNumber(lense_pixel_density)) {
    return new_error("lense pixel density not found in camera object "
                     "(.camera.lense_pixel_density)");
  }

  camera->lense_pixel_density = lense_pixel_density->valuedouble;

  cJSON* rays_per_pixel
      = cJSON_GetObjectItemCaseSensitive(root, "rays_per_pixel");
  if (!cJSON_IsNumber(rays_per_pixel)) {
    return new_error(
        "rays per pixel not found in camera object (.camera.rays_per_pixel)"
    );
  }

  camera->rays_per_pixel = (int)rays_per_pixel->valuedouble;

  cJSON* reflection_depth
      = cJSON_GetObjectItemCaseSensitive(root, "reflection_depth");
  if (!cJSON_IsNumber(reflection_depth)) {
    return new_error(
        "reflection depth not found in camera object (.camera.reflection_depth)"
    );
  }

  camera->reflection_depth = (int)reflection_depth->valuedouble;

  cJSON* refraction_depth
      = cJSON_GetObjectItemCaseSensitive(root, "refraction_depth");
  if (!cJSON_IsNumber(refraction_depth)) {
    return new_error(
        "refraction depth not found in camera object (.camera.refraction_depth)"
    );
  }

  camera->refraction_depth = (int)refraction_depth->valuedouble;

  return NULL;
}

error* parse_material(cJSON* root, material_object* material) {
  error* err;

  cJSON* name = cJSON_GetObjectItemCaseSensitive(root, "name");
  if (!cJSON_IsString(name) || name->valuestring == NULL) {
    return new_error("material name not found in material object (.name)");
  }

  material->name = strdup(name->valuestring);

  cJSON* diffuse_color
      = cJSON_GetObjectItemCaseSensitive(root, "diffuse_color");
  if (!cJSON_IsObject(diffuse_color)) {
    return new_error(
        "diffuse color not found in material object (.material.diffuse_color)"
    );
  }
  err = parse_vector3(diffuse_color, &material->diffuse_color);
  if (err != NULL) {
    return err;
  }

  cJSON* shininess_const
      = cJSON_GetObjectItemCaseSensitive(root, "shininess_const");
  if (!cJSON_IsNumber(shininess_const)) {
    return new_error("shininess const not found in material object "
                     "(.material.shininess_const)");
  }
  material->shininess_const = (int)shininess_const->valuedouble;

  cJSON* reflectiveness_const
      = cJSON_GetObjectItemCaseSensitive(root, "reflectiveness_const");
  if (!cJSON_IsNumber(reflectiveness_const)) {
    return new_error("reflectiveness const not found in material object "
                     "(.material.reflectiveness_const)");
  }
  material->reflectiveness_const = reflectiveness_const->valuedouble;

  cJSON* refractiveness_const
      = cJSON_GetObjectItemCaseSensitive(root, "refractiveness_const");
  if (!cJSON_IsNumber(refractiveness_const)) {
    return new_error("refractiveness const not found in material object "
                     "(.material.refractiveness_const)");
  }
  material->refractiveness_const = refractiveness_const->valuedouble;

  cJSON* refractive_index
      = cJSON_GetObjectItemCaseSensitive(root, "refractive_index");
  if (!cJSON_IsNumber(refractive_index)) {
    return new_error("refractive index not found in material object "
                     "(.material.refractive_index)");
  }
  material->refractive_index = refractive_index->valuedouble;

  return NULL;
}

error* parse_scene(cJSON* root, scene_object* scene) {
  error* err;

  cJSON* name = cJSON_GetObjectItemCaseSensitive(root, "name");
  if (!cJSON_IsString(name) || name->valuestring == NULL) {
    return new_error("scene name not found in scene JSON file (.name)");
  }

  scene->name = strdup(name->valuestring);

  cJSON* camera = cJSON_GetObjectItemCaseSensitive(root, "camera");
  if (!cJSON_IsObject(camera)) {
    return new_error("camera not found in scene JSON file (.camera)");
  }

  err = parse_camera(camera, &scene->camera);
  if (err != NULL) {
    return err;
  }

  cJSON* materials = cJSON_GetObjectItemCaseSensitive(root, "materials");
  if (!cJSON_IsArray(materials)) {
    return new_error("materials not found in scene JSON file (.materials)");
  }

  scene->materials_size = cJSON_GetArraySize(materials);
  scene->materials = calloc(scene->materials_size, sizeof(material_object));
  if (scene->materials == NULL) {
    return new_error("failed to allocate memory for materials");
  }

  for (int i = 0; i < scene->materials_size; i++) {
    cJSON* material = cJSON_GetArrayItem(materials, i);
    if (!cJSON_IsObject(material)) {
      return new_error("material is not an JSON object");
    }

    err = parse_material(material, &scene->materials[i]);
    if (err != NULL) {
      return err;
    }
  }

  return NULL;
}

error* load_from_json(scene_object* scene, char* filename) {
  char* file = read_file(filename);
  if (file == NULL) {
    return new_error("failed to read scene JSON file");
  }

  cJSON* root = cJSON_Parse(file);
  if (root == NULL) {
    free(file);
    return new_error("failed to parse scene JSON file");
  }

  error* err = parse_scene(root, scene);
  if (err != NULL) {
    cJSON_Delete(root);
    free(file);
    return err;
  }

  cJSON_Delete(root);
  free(file);
  return NULL;
}

// --------------------------------------------

// reads a line from a file. the caller is responsible for freeing the memory.
// returns NULL if an error occurred.
char* read_line(FILE* file) {
  char* line = calloc(256, sizeof(char));
  if (line == NULL) {
    return NULL;
  }

  size_t size   = 255; // lower than allocated, to allow for null terminator
  size_t length = 0;

  while (1) {
    int c = fgetc(file);
    if (c == EOF) {
      break;
    }

    line[length] = c;
    length += 1;

    if (c == '\n') {
      break;
    }

    if (length >= size) {
      size *= 2;
      line = realloc(line, size);
      if (line == NULL) {
        return NULL;
      }
    }
  }

  line[length] = '\0';

  return line;
}

error* load_scene(char* filename, object_manager* obj_manager) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return new_error("failed to open scene file");
  }

  char object_type;

  while (1) {
    char* line = read_line(file);
    if (line == NULL) {
      return new_error("failed to read line");
    }

    if (strlen(line) == 0) {
      free(line);
      break;
    }

    if (line[0] == 'l') { // add light
      float x, y, z;
      float dr, dg, db;
      float sr, sg, sb;

      sscanf(
          line,
          "%c %f %f %f %f %f %f %f %f %f",
          &object_type,
          &x,
          &y,
          &z,
          &dr,
          &dg,
          &db,
          &sr,
          &sg,
          &sb
      );

      add_light(obj_manager, x, y, z, dr, dg, db, sr, sg, sb);
    } else if (line[0] == 'm') { // add material
      float dr, dg, db;
      int sc;
      float reflc, refrc, refri;

      sscanf(
          line,
          "%c %f %f %f %d %f %f %f",
          &object_type,
          &dr,
          &dg,
          &db,
          &sc,
          &reflc,
          &refrc,
          &refri
      );

      add_material(obj_manager, dr, dg, db, sc, reflc, refrc, refri);
    } else if (line[0] == 'p') { // add plane
      float px, py, pz, nx, ny, nz;
      int mat;

      sscanf(
          line,
          "%c %f %f %f %f %f %f %d",
          &object_type,
          &px,
          &py,
          &pz,
          &nx,
          &ny,
          &nz,
          &mat
      );

      add_plane(
          obj_manager, px, py, pz, nx, ny, nz, &obj_manager->materials[mat]
      );
    } else if (line[0] == 's') { // add sphere
      float cx, cy, cz, radius;
      int mat;

      sscanf(
          line, "%c %f %f %f %f %d", &object_type, &cx, &cy, &cz, &radius, &mat
      );

      add_sphere(obj_manager, cx, cy, cz, radius, &obj_manager->materials[mat]);
    } else if (line[0] == 't') { // add triangle
      float p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z;
      int invert_normal, mat;

      sscanf(
          line,
          "%c %f %f %f %f %f %f %f %f %f %d %d",
          &object_type,
          &p1x,
          &p1y,
          &p1z,
          &p2x,
          &p2y,
          &p2z,
          &p3x,
          &p3y,
          &p3z,
          &invert_normal,
          &mat
      );

      add_triangle(
          obj_manager,
          p1x,
          p1y,
          p1z,
          p2x,
          p2y,
          p2z,
          p3x,
          p3y,
          p3z,
          &obj_manager->materials[mat],
          invert_normal
      );
    }

    free(line);
  }

  fclose(file);

  return NULL;
}
