#include "viewer.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int window_width = 800;
int window_height = 600;

struct grga_image *current_image = NULL;
uint16_t current_image_index = 0;

float pixel_size = 1.0;
float offset_x = 0.0, offset_y = 0.0;

bool is_window_open = true;

struct button btn_prev, btn_next;
struct slider zoom_slider, horizontal_scrollbar, vertical_scrollbar;

void initialize_sdl(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        perror("SDL failed to initialize!\n");
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1)
    {
        perror("TTF failed to initialize!\n");
        exit(EXIT_FAILURE);
    }
}

void create_window_and_renderer(const char *title)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (window == NULL)
    {
        perror("SDL window failed to initialize!\n");
        exit(EXIT_FAILURE);
    }

    if (renderer == NULL)
    {
        perror("SDL renderer failed to initialize!\n");
        exit(EXIT_FAILURE);
    }

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
}

void poll_events(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            is_window_open = false;
            break;
        case SDL_WINDOWEVENT:
            handle_window_resize(&event);
            break;
        case SDL_MOUSEBUTTONDOWN:
            detect_click_on_button(&event, &btn_prev);
            detect_click_on_button(&event, &btn_next);
            detect_click_on_slider(&event, &zoom_slider);
            detect_click_on_slider(&event, &horizontal_scrollbar);
            detect_click_on_slider(&event, &vertical_scrollbar);
            break;
        case SDL_MOUSEBUTTONUP:
            detect_release_from_slider(&event, &zoom_slider);
            detect_release_from_slider(&event, &horizontal_scrollbar);
            detect_release_from_slider(&event, &vertical_scrollbar);
            break;
        case SDL_MOUSEMOTION:
            detect_drag_slider(&event, &zoom_slider, &pixel_size, &render);
            detect_drag_slider(&event, &horizontal_scrollbar, &offset_x, &render);
            detect_drag_slider(&event, &vertical_scrollbar, &offset_y, &render);
            break;
        case SDL_MOUSEWHEEL:
            handle_image_zoom(&event);
            break;
        case SDL_KEYUP:
            handle_image_change(&event);
            break;
        case SDL_DROPFILE:
            handle_file_drop(&event);
            break;
        }
    }
}

void handle_window_resize(SDL_Event *event)
{
    if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
    {
        window_width = event->window.data1;
        window_height = event->window.data2;

        init_gui();
        render();
    }
}

void handle_image_zoom(SDL_Event *event)
{
    if (event->wheel.y > 0 && pixel_size < MAX_PIXEL_SIZE)
    {
        pixel_size += PIXEL_ZOOM_STEP;
        init_gui();
        render();
    }
    else if (event->wheel.y < 0 && pixel_size > MIN_PIXEL_SIZE)
    {
        pixel_size -= PIXEL_ZOOM_STEP;
        init_gui();
        render();
    }
}

void change_to_previous_image(void)
{
    if (image_count <= 0 || current_image_index <= 0)
        return;

    set_current_image(image_files[--current_image_index]);
    render();
}

void change_to_next_image(void)
{
    if (image_count <= 0 || current_image_index >= image_count - 1)
        return;

    set_current_image(image_files[++current_image_index]);
    render();
}

void handle_image_change(SDL_Event *event)
{
    if (event->key.keysym.sym == SDLK_LEFT)
        change_to_previous_image();
    else if (event->key.keysym.sym == SDLK_RIGHT)
        change_to_next_image();
}

void handle_file_drop(SDL_Event *event)
{
    image_count = 0;
    current_image_index = 0;

    set_current_image(event->drop.file);
    render();
}

bool is_image_clipping_horizontally(void)
{
    if (window_width - current_image->header.width * pixel_size < 0.0)
        return false;

    return true;
}

bool is_image_clipping_vertically(void)
{
    if (window_height - current_image->header.height * pixel_size < 0.0)
        return false;

    return true;
}

void init_gui(void)
{
    initialize_button(&btn_prev, (SDL_Rect){40, window_height / 2 - 20, 60, 40}, (SDL_Color){210, 210, 210, 255}, "<", &change_to_previous_image);
    initialize_button(&btn_next, (SDL_Rect){window_width - 100, window_height / 2 - 20, 60, 40}, (SDL_Color){210, 210, 210, 255}, ">", &change_to_next_image);

    initialize_slider(&zoom_slider, (SDL_Rect){(window_width - 300) / 2, window_height - 50, 300, 20}, (SDL_Color){210, 210, 210, 255}, (SDL_Color){100, 100, 255, 255}, false, MIN_PIXEL_SIZE, MAX_PIXEL_SIZE, pixel_size, true);
    initialize_slider(&horizontal_scrollbar, (SDL_Rect){0, window_height - 15, window_width - 15, 15}, (SDL_Color){230, 230, 230, 255}, (SDL_Color){200, 200, 200, 255}, false, -500, 500, 0, false);
    initialize_slider(&vertical_scrollbar, (SDL_Rect){window_width - 15, 0, 15, window_height - 15}, (SDL_Color){230, 230, 230, 255}, (SDL_Color){200, 200, 200, 255}, true, -500, 500, 0, false);
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    if (current_image == NULL)
    {
        render_text(renderer, "Drag and drop any .grga image into viewer to start.", window_width, window_height, 0, -20, 30);
        render_text(renderer, "Alternatively, drag and drop a directory containing .grga images.", window_width, window_height, 0, 20, 24);
        SDL_RenderPresent(renderer);

        return;
    }

    uint8_t *src_pixel;
    uint8_t r, g, b;
    SDL_FRect rect;

    uint16_t width = current_image->header.width;
    uint16_t height = current_image->header.height;
    uint8_t channels = current_image->header.channels;

    for (uint16_t y = 0; y < height; y++)
        for (uint16_t x = 0; x < width; x++)
        {
            src_pixel = &current_image->pixel_data[(y * width + x) * channels];

            r = src_pixel[0];
            g = src_pixel[1];
            b = src_pixel[2];

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);

            rect.x = (window_width - width * pixel_size) / 2 - offset_x + x * pixel_size;
            rect.y = (window_height - height * pixel_size) / 2 - offset_y + y * pixel_size;
            rect.w = pixel_size;
            rect.h = pixel_size;

            SDL_RenderFillRectF(renderer, &rect);
        }

    if (image_count > 0)
    {
        render_button(renderer, &btn_prev);
        render_button(renderer, &btn_next);
    }

    render_slider(renderer, &zoom_slider);

    if (is_image_clipping_horizontally() == false)
        render_slider(renderer, &horizontal_scrollbar);
    if (is_image_clipping_vertically() == false)
        render_slider(renderer, &vertical_scrollbar);

    SDL_RenderPresent(renderer);
}

void set_current_image(const char *path)
{
    current_image = load_grga_image(path);

    // If path does not point to a .grga image, see if it does to a directory.
    if (current_image == NULL)
    {
        image_count = 0;
        current_image_index = 0;

        search_directory_contents(path);
        if (image_count <= 0)
        {
            perror("Directory contains no .grga images!\n");
            exit(EXIT_FAILURE);
        }

        current_image = load_grga_image(image_files[0]);
    }
}

void run_viewer(const char *path)
{
    load_window_icon(window, "assets/magnifier.png");

    if (path != NULL)
        set_current_image(path);

    init_gui();
    render();

    while (is_window_open == true)
        poll_events();
}

void cleanup(void)
{
    free(current_image);
    current_image = NULL;

    gui_cleanup();
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}