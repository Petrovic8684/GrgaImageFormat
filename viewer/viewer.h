#ifndef VIEWER_H
#define VIEWER_H

#include "../util/util.h"
#include "../gui/gui.h"
#include <SDL2/SDL_image.h>

#define MAX_PIXEL_SIZE 100
#define MIN_PIXEL_SIZE 1
#define PIXEL_ZOOM_STEP 1

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern int window_width;
extern int window_height;

extern struct grga_image *current_image;
extern uint16_t current_image_index;

extern uint8_t pixel_size;
extern int offset_x, offset_y;

extern bool is_window_open;
extern char *base_path;

void initialize_sdl(void);
void create_window_and_renderer(const char *);
void poll_events(void);
void handle_window_resize(SDL_Event *);
void handle_image_zoom(SDL_Event *);
void handle_image_change(SDL_Event *);
void handle_file_drop(SDL_Event *);
void init_gui(void);
void render(void);
void set_current_image(const char *);
void run_viewer(const char *);
void cleanup(void);

#endif