all:
	gcc -I sdl2/include -L sdl2/lib -L util -L gui -L viewer main.c util/util.c gui/gui.c viewer/viewer.c -o GrgaViewer iconData.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf