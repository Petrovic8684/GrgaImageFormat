#ifndef VIEWER_H
#define VIEWER_H

#include "../util/util.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 550

#define MAX_PIXEL_SIZE 100
#define MIN_PIXEL_SIZE 1
#define PIXEL_ZOOM_STEP 1

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern struct grga_image *current_image;

extern uint8_t pixel_size;
extern int offset_x, offset_y;

extern bool is_window_open;
extern char *base_path;

void initialize_sdl(void);
void create_window_and_renderer(const char *);
void poll_events(void);
void handle_zoom(SDL_Event *);
void render(void);
void set_current_image(const char *);
void start_viewer_and_keep_running(const char *path);
void cleanup(void);

#endif