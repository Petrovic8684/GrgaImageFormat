#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define FONT_SIZE 24

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
    uint8_t min_value;
    uint8_t max_value;
    uint8_t current_value;
    bool is_dragging;
};

void render_text(SDL_Renderer *, const char *, SDL_Rect *, SDL_Color, TTF_Font *);
void render_centered_text(SDL_Renderer *, const char *, SDL_Color, TTF_Font *, int, int);
void render_welcome_message(SDL_Renderer *, int, int, TTF_Font *);
void initialize_slider(struct slider *, int, int, int, int, uint8_t, uint8_t, uint8_t);
void render_button(SDL_Renderer *, struct button *, TTF_Font *);
void render_slider(SDL_Renderer *, struct slider *);
void detect_click_on_button(SDL_Event *, struct button *);
void detect_click_on_slider(SDL_Event *, struct slider *);
void detect_click_release_from_slider(SDL_Event *, struct slider *);
void detect_drag_slider(SDL_Event *, struct slider *, uint8_t *, void (*)(void));

#endif