#ifndef VIEWER_H
#define VIEWER_H

#include "../util/util.h"
#include "../gui/gui.h"

#define MAX_PIXEL_SIZE 10.0
#define MIN_PIXEL_SIZE 0.3
#define PIXEL_ZOOM_STEP 0.03

void initialize_sdl(void);
void create_window_and_renderer(const char *title);
void run_viewer(const char *path);
bool is_image_clipping_horizontally(void);
bool is_image_clipping_vertically(void);
void init_gui(void);
void limit_offsets(void);
void render(void);
void poll_events(void);
void set_current_image(const char *path);
void handle_window_resize(SDL_Event *event);
void handle_image_zoom(SDL_Event *event);
void handle_file_drop(SDL_Event *event);
void handle_image_change(SDL_Event *event);
void change_to_previous_image(void);
void change_to_next_image(void);
void cleanup(void);

#endif