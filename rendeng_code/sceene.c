#include "sceene.h"
#include <stdio.h>




void load_sceene(char* file_name, object_manager* objm){
	printf("loading sceene: %s\n", file_name);

	char* line;
	size_t n;
	char descirp_char;

	FILE* sceene_file = fopen(file_name, "r");

	while(getline(&line, &n, sceene_file) != -1){


		if(line[0] == 'l'){ // add light
			float x, y, z;
			float dr, dg, db;
			float sr, sg, sb;

			sscanf(line, "%c %f %f %f %f %f %f %f %f %f", &descirp_char, &x, &y, &z, &dr, &dg, &db, &sr, &sg, &sb);
			
			// printf("light\n");
			// printf("%f %f %f\n", x, y, z);
			// printf("%f %f %f\n", dr, dg, db);
			// printf("%f %f %f\n", sr, sg, sb);

			add_light(objm, x, y, z, dr, dg, db, sr, sg, sb);

		}else if(line[0] == 'm'){ // add material
			float dr, dg, db;
			int sc;
			float reflc, refrc, refri;

			sscanf(line, "%c %f %f %f %d %f %f %f", &descirp_char, &dr, &dg, &db, &sc, &reflc, &refrc, &refri);

			// printf("material\n");
			// printf("%f %f %f\n", dr, dg, db);
			// printf("%d\n", sc);
			// printf("%f %f %f\n", reflc, refrc, refri);

			add_material(objm, dr, dg, db, sc, reflc, refrc, refri);

		}else if(line[0] == 'p'){ // add plane
			float px, py, pz, nx, ny, nz;
			int mat;

			sscanf(line, "%c %f %f %f %f %f %f %d", &descirp_char, &px, &py, &pz, &nx, &ny, &nz, &mat);

			// printf("plane\n");
			// printf("%f %f %f\n", px, py, pz);
			// printf("%f %f %f\n", nx, ny, nz);
			// printf("%d\n", mat);

			add_plane(objm, px, py, pz, nx, ny, nz, &objm->materials[mat]);

		}else if(line[0] == 's'){ // add sphere
			float cx, cy, cz, radius;
			int mat;

			sscanf(line, "%c %f %f %f %f %d", &descirp_char, &cx, &cy, &cz, &radius, &mat);

			// printf("sphere\n");
			// printf("%f %f %f %f\n", cx, cy, cz, radius);
			// printf("%d\n", mat);

			add_sphere(objm, cx, cy, cz, radius, &objm->materials[mat]);


		}

		//printf("%s", line);

	}


	fclose(sceene_file);

}


/*
m material
extern void add_material(object_manager* objm, float dr, float dg, float db, int sc, float reflc, float refrc, float refri);

s sphere
extern void add_sphere(object_manager* objm, float cx, float cy, float cz, float radius, material* mat);

p plane
l light


typedef struct LIGHT{
	vect3d pos;
	rgb_color diffuse_light_color;
	rgb_color specular_light_color;
} light;
l 200 200 200 1 0 0 0 1 0
m 1 0 0.5 1 0.1 0.1 0.1
p
*/