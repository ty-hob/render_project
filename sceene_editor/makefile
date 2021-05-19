objs = ./build_objs
code = ./code

cc = gcc
cflags = `pkg-config --cflags sdl2 SDL2_gfx` -W
libs = `pkg-config --libs sdl2 SDL2_gfx` -lm

scedit : $(objs)/main.o $(objs)/view.o $(objs)/linalg2.o
	$(cc) -o scedit ./build_objs/*  $(cflags) $(libs)
	
$(objs)/main.o : $(code)/main.c $(code)/view.c $(code)/view.h $(code)/linalg2.c $(code)/linalg2.h
	$(cc) -o $(objs)/main.o -c $(cflags) $(libs) $(code)/main.c

$(objs)/view.o : $(code)/view.c $(code)/view.h $(code)/linalg2.c $(code)/linalg2.h
	$(cc) -o $(objs)/view.o -c $(cflags) $(libs) $(code)/view.c

$(objs)/linalg2.o : $(code)/linalg2.c $(code)/linalg2.h
	$(cc) -o $(objs)/linalg2.o -c $(cflags) $(libs) $(code)/linalg2.c

clean :
	rm scedit $(objs)/*