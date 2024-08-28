#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rendeng/scene.h"

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
  printf("loading scene: %s\n", filename);

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return new_error("could not open scene file");
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
