objdir = ./make_objects
codedir = ./rendeng_code

rendeng : $(objdir)/main.o $(objdir)/linalg.o $(objdir)/imgexp.o $(objdir)/objs.o $(objdir)/sceene.o
	gcc -o rendeng $(objdir)/main.o $(objdir)/linalg.o $(objdir)/imgexp.o $(objdir)/objs.o $(objdir)/sceene.o -lm

$(objdir)/main.o : $(codedir)/main.c $(codedir)/linalg.c $(codedir)/linalg.h $(codedir)/imgexp.c $(codedir)/imgexp.h $(codedir)/objs.c $(codedir)/objs.h $(codedir)/sceene.c $(codedir)/sceene.h
	gcc -o $(objdir)/main.o -c $(codedir)/main.c -lm

$(objdir)/linalg.o : $(codedir)/linalg.c $(codedir)/linalg.h
	gcc -o $(objdir)/linalg.o -c  $(codedir)/linalg.c

$(objdir)/imgexp.o : $(codedir)/imgexp.c $(codedir)/imgexp.h
	gcc -o $(objdir)/imgexp.o -c $(codedir)/imgexp.c

$(objdir)/objs.o : $(codedir)/objs.c $(codedir)/objs.h
	gcc -o $(objdir)/objs.o -c $(codedir)/objs.c

$(objdir)/sceene.o : $(codedir)/sceene.c $(codedir)/sceene.h
	gcc -o $(objdir)/sceene.o -c $(codedir)/sceene.c

clean :
	rm rendeng $(objdir)/main.o $(objdir)/linalg.o $(objdir)/imgexp.o $(objdir)/objs.o
