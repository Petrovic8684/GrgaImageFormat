#ifndef VIEWER_H
#define VIEWER_H

#include "../util/util.h"
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 550

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern bool is_window_open;

void initialize_sdl(void);
void create_window_and_renderer(const char *);
void poll_events(void);
void render(void);
void start_viewer_and_keep_running(void);
void cleanup(void);

#endif