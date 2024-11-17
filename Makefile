all:
	gcc -I sdl2/include -L sdl2/lib -L util -L viewer main.c util/util.c viewer/viewer.c -o GrgaViewer -lmingw32 -lSDL2main -lSDL2