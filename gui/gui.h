#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define FONT_PATH "assets/runescape.ttf"

extern TTF_Font *font;

struct button
{
    SDL_Rect rect;
    SDL_Color bg_color;
    const char *label;
    void (*callback)();
};

struct slider
{
    SDL_Rect track;
    SDL_Rect knob;
    SDL_Color track_color;
    SDL_Color knob_color;
    bool is_vertical;
    float min_value;
    float max_value;
    float current_value;
    bool is_dragging;
};

void load_window_icon(SDL_Window *window, const char *path);
void load_font(uint8_t size);
void render_text(SDL_Renderer *renderer, const char *text, int parent_width, int parent_height, int x_pos_offset, int y_pos_offset, uint8_t font_size);
void initialize_button(struct button *button, SDL_Rect rect_position_size, SDL_Color bg_color, const char *label, void (*callback)(void));
void initialize_slider(struct slider *slider, SDL_Rect rect_position_size, SDL_Color track_color, SDL_Color knob_color, bool is_vertical, float min_value, float max_value, float initial_value, bool smaller_track);
void render_button(SDL_Renderer *renderer, struct button *button);
void render_slider(SDL_Renderer *renderer, struct slider *slider);
void detect_click_on_button(SDL_Event *event, struct button *btn);
void detect_click_on_slider(SDL_Event *event, struct slider *slider);
void detect_release_from_slider(SDL_Event *event, struct slider *slider);
void detect_drag_slider(SDL_Event *event, struct slider *slider, float *actual_parameter_to_change, void (*callback)(void));
void gui_cleanup(void);

#endif