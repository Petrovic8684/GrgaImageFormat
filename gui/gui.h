#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define FONT_PATH "assets/runescape.ttf"

extern TTF_Font *font;

struct button
{
    SDL_Rect rect;
    const char *label;
    void (*callback)();
};

struct slider
{
    SDL_Rect track;
    SDL_Rect knob;
    float min_value;
    float max_value;
    float current_value;
    bool is_dragging;
};

void load_window_icon(SDL_Window *window, const char *path);
void load_font(uint8_t size);
void render_text(SDL_Renderer *renderer, const char *text, int parent_width, int parent_height, int x_pos_offset, int y_pos_offset, uint8_t font_size);
void initialize_slider(struct slider *slider, int x, int y, int w, int h, float min_value, float max_value, float initial_value);
void initialize_button(struct button *button, int x, int y, int w, int h, const char *label, void (*callback)(void));
void render_button(SDL_Renderer *renderer, struct button *button);
void render_slider(SDL_Renderer *renderer, struct slider *slider);
void detect_click_on_button(SDL_Event *event, struct button *btn);
void detect_click_on_slider(SDL_Event *event, struct slider *slider);
void detect_release_from_slider(SDL_Event *event, struct slider *slider);
void detect_drag_slider(SDL_Event *event, struct slider *slider, float *pixel_size, void (*render)(void));
void gui_cleanup(void);

#endif