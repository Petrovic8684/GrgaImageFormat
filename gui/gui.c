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

void initialize_slider(struct slider *slider, int x, int y, int w, int h, float min_value, float max_value, float initial_value, SDL_Color track_color, SDL_Color knob_color, bool is_vertical, bool smaller_track)
{
    slider->track.x = x;
    slider->track.y = y;
    slider->track.w = w;
    slider->track.h = smaller_track ? h / 2 : h;
    slider->is_vertical = is_vertical;

    if (is_vertical == true)
    {
        slider->knob.w = w;
        slider->knob.h = w;
        slider->knob.x = smaller_track ? x - (w / 4) : x;
        slider->knob.y = y + (initial_value - min_value) * (h - slider->knob.h) / (max_value - min_value);
    }
    else
    {
        slider->knob.w = h;
        slider->knob.h = h;
        slider->knob.x = x + (initial_value - min_value) * (w - slider->knob.w) / (max_value - min_value);
        slider->knob.y = smaller_track ? y - (h / 4) : y;
    }

    slider->min_value = min_value;
    slider->max_value = max_value;
    slider->current_value = initial_value;
    slider->is_dragging = false;

    slider->track_color = track_color;
    slider->knob_color = knob_color;
}

void initialize_button(struct button *button, int x, int y, int w, int h, const char *label, SDL_Color bg_color, void (*callback)())
{
    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = w;
    button->rect.h = h;

    button->label = label;
    button->bg_color = bg_color;
    button->callback = callback;
}

void render_button(SDL_Renderer *renderer, struct button *button)
{
    SDL_SetRenderDrawColor(renderer, button->bg_color.r, button->bg_color.g, button->bg_color.b, button->bg_color.a);
    SDL_RenderFillRect(renderer, &button->rect);

    render_text(renderer, button->label, button->rect.w, button->rect.h, button->rect.x, button->rect.y + 5, 40);
}

void render_slider(SDL_Renderer *renderer, struct slider *slider)
{
    SDL_SetRenderDrawColor(renderer, slider->track_color.r, slider->track_color.g, slider->track_color.b, slider->track_color.a);
    SDL_RenderFillRect(renderer, &slider->track);

    SDL_SetRenderDrawColor(renderer, slider->knob_color.r, slider->knob_color.g, slider->knob_color.b, slider->knob_color.a);
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

void detect_drag_slider(SDL_Event *event, struct slider *slider, float *actual_parameter_to_change, void (*callback)(void))
{
    if (!slider->is_dragging)
        return;

    int mouse_position = slider->is_vertical ? event->motion.y : event->motion.x;

    if (slider->is_vertical)
    {
        slider->knob.y = mouse_position - slider->knob.h / 2;

        if (slider->knob.y < slider->track.y)
            slider->knob.y = slider->track.y;
        else if (slider->knob.y > slider->track.y + slider->track.h - slider->knob.h)
            slider->knob.y = slider->track.y + slider->track.h - slider->knob.h;

        slider->current_value = slider->min_value +
                                (slider->knob.y - slider->track.y) * (slider->max_value - slider->min_value) /
                                    (slider->track.h - slider->knob.h);
    }
    else
    {
        slider->knob.x = mouse_position - slider->knob.w / 2;

        if (slider->knob.x < slider->track.x)
            slider->knob.x = slider->track.x;
        else if (slider->knob.x > slider->track.x + slider->track.w - slider->knob.w)
            slider->knob.x = slider->track.x + slider->track.w - slider->knob.w;

        slider->current_value = slider->min_value +
                                (slider->knob.x - slider->track.x) * (slider->max_value - slider->min_value) /
                                    (slider->track.w - slider->knob.w);
    }

    *actual_parameter_to_change = slider->current_value;
    callback();
}

void gui_cleanup(void)
{
    TTF_CloseFont(font);
}