#include "gui.h"

TTF_Font *font = NULL;

void load_window_icon(SDL_Window *window, const char *path)
{
    SDL_Surface *icon = IMG_Load(path);
    if (icon == NULL)
    {
        perror("Unable to load icon!\n");
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);
}

void load_font(uint8_t size)
{
    font = TTF_OpenFont(FONT_PATH, size);
    if (font == NULL)
    {
        perror("Failed to load font!\n");
        exit(EXIT_FAILURE);
    }
}

void render_text(SDL_Renderer *renderer, const char *text, int parent_width, int parent_height, int x_pos_offset, int y_pos_offset, uint8_t font_size)
{
    load_font(font_size);

    SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, (SDL_Color){0, 0, 0, 255});
    if (text_surface == NULL)
    {
        perror("Failed to render text surface!\n");
        exit(EXIT_FAILURE);
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (text_texture == NULL)
    {
        SDL_FreeSurface(text_surface);
        perror("Failed to render text surface!\n");
        exit(EXIT_FAILURE);
    }

    SDL_Rect target_rect;
    target_rect.w = text_surface->w;
    target_rect.h = text_surface->h;
    target_rect.x = (parent_width - target_rect.w) / 2 + x_pos_offset;
    target_rect.y = (parent_height - target_rect.h) / 2 + y_pos_offset;

    SDL_RenderCopy(renderer, text_texture, NULL, &target_rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

void initialize_slider(struct slider *slider, int x, int y, int w, int h, uint8_t min_value, uint8_t max_value, uint8_t initial_value)
{
    slider->track.x = x;
    slider->track.y = y;
    slider->track.w = w;
    slider->track.h = h / 2;

    slider->knob.w = h;
    slider->knob.h = h;
    slider->knob.x = x + (initial_value - min_value) * (w - slider->knob.w) / (max_value - min_value);
    slider->knob.y = y - (h / 4);

    slider->min_value = min_value;
    slider->max_value = max_value;
    slider->current_value = initial_value;
    slider->is_dragging = false;
}

void initialize_button(struct button *button, int x, int y, int w, int h, const char *label, void (*callback)())
{
    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = w;
    button->rect.h = h;

    button->label = label;
    button->callback = callback;
}

void render_button(SDL_Renderer *renderer, struct button *button)
{
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &button->rect);

    render_text(renderer, button->label, button->rect.w, button->rect.h, button->rect.x, button->rect.y + 5, 40);
}

void render_slider(SDL_Renderer *renderer, struct slider *slider)
{
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &slider->track);

    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_RenderFillRect(renderer, &slider->knob);
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

void detect_release_from_slider(SDL_Event *event, struct slider *slider)
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

void gui_cleanup(void)
{
    TTF_CloseFont(font);
}