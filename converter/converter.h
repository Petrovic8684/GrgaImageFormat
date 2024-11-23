#ifndef CONVERTER_H
#define CONVERTER_H

#include "../util/util.h"
#include "../gui/gui.h"

void initialize_sdl(void);
void create_window_and_renderer(const char *title);
void poll_events(void);
void handle_window_resize(SDL_Event *event);
void handle_file_drop(SDL_Event *event);
void render(void);
void run_converter(const char *path);
void cleanup(void);

#endif