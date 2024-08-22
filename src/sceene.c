#include <stdio.h>

#include "sceene.h"

void load_sceene(char *file_name, object_manager *objm) {
  printf("loading sceene: %s\n", file_name);

  char *line;
  size_t n;
  char descirp_char;

  FILE *sceene_file = fopen(file_name, "r");

  while (getline(&line, &n, sceene_file) != -1) {
    if (line[0] == 'l') { // add light
      float x, y, z;
      float dr, dg, db;
      float sr, sg, sb;

      sscanf(
          line,
          "%c %f %f %f %f %f %f %f %f %f",
          &descirp_char,
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

      add_light(objm, x, y, z, dr, dg, db, sr, sg, sb);
    } else if (line[0] == 'm') { // add material
      float dr, dg, db;
      int sc;
      float reflc, refrc, refri;

      sscanf(
          line,
          "%c %f %f %f %d %f %f %f",
          &descirp_char,
          &dr,
          &dg,
          &db,
          &sc,
          &reflc,
          &refrc,
          &refri
      );

      add_material(objm, dr, dg, db, sc, reflc, refrc, refri);
    } else if (line[0] == 'p') { // add plane
      float px, py, pz, nx, ny, nz;
      int mat;

      sscanf(
          line,
          "%c %f %f %f %f %f %f %d",
          &descirp_char,
          &px,
          &py,
          &pz,
          &nx,
          &ny,
          &nz,
          &mat
      );

      add_plane(objm, px, py, pz, nx, ny, nz, &objm->materials[mat]);
    } else if (line[0] == 's') { // add sphere
      float cx, cy, cz, radius;
      int mat;

      sscanf(
          line, "%c %f %f %f %f %d", &descirp_char, &cx, &cy, &cz, &radius, &mat
      );

      add_sphere(objm, cx, cy, cz, radius, &objm->materials[mat]);
    }
  }

  fclose(sceene_file);
}
