#include "gui.h"

void render_text(SDL_Renderer *renderer, const char *text, SDL_Rect *target_rect, SDL_Color color, TTF_Font *font)
{
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, color);
    if (!text_surface)
    {
        perror("Failed to render text surface!\n");
        exit(EXIT_FAILURE);
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture)
    {
        SDL_FreeSurface(text_surface);
        perror("Failed to render text surface!\n");
        exit(EXIT_FAILURE);
    }

    SDL_RenderCopy(renderer, text_texture, NULL, target_rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

void render_centered_text(SDL_Renderer *renderer, const char *text, SDL_Color color, TTF_Font *font, int window_width, int window_height)
{
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, color);
    if (!text_surface)
    {
        perror("Failed to render text surface!\n");
        exit(EXIT_FAILURE);
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture)
    {
        SDL_FreeSurface(text_surface);
        perror("Failed to create text texture from surface!\n");
        exit(EXIT_FAILURE);
    }

    SDL_Rect text_rect;
    text_rect.w = text_surface->w;
    text_rect.h = text_surface->h;
    text_rect.x = (window_width - text_rect.w) / 2;
    text_rect.y = (window_height - text_rect.h) / 2;

    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

void render_welcome_message(SDL_Renderer *renderer, int window_width, int window_height, TTF_Font *font)
{
    SDL_Color text_color = {0, 0, 0, 255};

    render_centered_text(renderer,
                         "Drag and drop any .grga image into viewer to start.",
                         text_color, font, window_width, window_height - 20);

    render_centered_text(renderer,
                         "Alternatively, drag and drop a directory containing .grga images.",
                         text_color, font, window_width, window_height + 20);
}

void initialize_slider(struct slider *slider, int window_width, int window_height, int width, int height, uint8_t min_value, uint8_t max_value, uint8_t initial_value)
{
    slider->track.x = (window_width - width) / 2;
    slider->track.y = window_height - height - 10;
    slider->track.w = width;
    slider->track.h = height / 2;

    slider->knob.w = height;
    slider->knob.h = height;
    slider->knob.x = (window_width - width) / 2 + (initial_value - min_value) * (width - slider->knob.w) / (max_value - min_value);
    slider->knob.y = window_height - height - 10 - (height / 4);

    slider->min_value = min_value;
    slider->max_value = max_value;
    slider->current_value = initial_value;
    slider->is_dragging = false;
}

void render_button(SDL_Renderer *renderer, struct button *button, TTF_Font *font)
{
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &button->rect);

    SDL_Color text_color = {0, 0, 0, 255};
    SDL_Rect text_rect = {
        button->rect.x + button->rect.w / 4,
        button->rect.y + button->rect.h / 4,
        button->rect.w / 2,
        button->rect.h / 2};

    render_text(renderer, button->label, &text_rect, text_color, font);
}

void render_slider(SDL_Renderer *renderer, struct slider *slider)
{
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &slider->track);

    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_RenderFillRect(renderer, &slider->knob);
}

void render_scrollbar(SDL_Renderer *renderer, struct scrollbar *scrollbar)
{
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &scrollbar->rect);

    SDL_Rect handle = {
        scrollbar->rect.x,
        scrollbar->rect.y + scrollbar->position * (scrollbar->rect.h / scrollbar->max_position),
        scrollbar->rect.w,
        20};

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &handle);
}

void detect_click_on_button(SDL_Event *event, struct button *btn)
{
    if (SDL_PointInRect(&(SDL_Point){event->button.x, event->button.y}, &(btn->rect)))
        btn->callback();
}

void detect_click_on_slider(SDL_Event *event, struct slider *slider)
{
    int mx = event->button.x;
    int my = event->button.y;

    if (mx >= slider->knob.x && mx <= slider->knob.x + slider->knob.w &&
        my >= slider->knob.y && my <= slider->knob.y + slider->knob.h)
        slider->is_dragging = true;
}

void detect_click_release_from_slider(SDL_Event *event, struct slider *slider)
{
    if (!slider->is_dragging)
        return;

    slider->is_dragging = false;
}

void detect_drag_slider(SDL_Event *event, struct slider *slider, uint8_t *pixel_size, void (*render)(void))
{
    if (!slider->is_dragging)
        return;

    int mx = event->motion.x;

    slider->knob.x = mx - slider->knob.w / 2;
    if (slider->knob.x < slider->track.x)
        slider->knob.x = slider->track.x;
    else if (slider->knob.x > slider->track.x + slider->track.w - slider->knob.w)
        slider->knob.x = slider->track.x + slider->track.w - slider->knob.w;

    slider->current_value = slider->min_value +
                            (slider->knob.x - slider->track.x) * (slider->max_value - slider->min_value) /
                                (slider->track.w - slider->knob.w);

    *pixel_size = slider->current_value;
    render();
}