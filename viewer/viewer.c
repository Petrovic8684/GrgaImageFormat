#include "viewer.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
struct grga_image *current_image = NULL;

uint8_t pixel_size = 1;
int offset_x = 0, offset_y = 0;

bool is_window_open = true;
char *base_path = NULL;

void initialize_sdl(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        perror("SDL failed to initialize!\n");
        exit(EXIT_FAILURE);
    }

    base_path = SDL_GetBasePath();

    if (base_path == NULL)
    {
        perror("Could not identify base path!\n");
        exit(EXIT_FAILURE);
    }
}

void create_window_and_renderer(const char *title)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

    SDL_Surface *icon = IMG_Load(strcat(base_path, "assets/icon.png"));

    if (icon == NULL)
    {
        perror("Unable to load icon!\n");
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);
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
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                render();
            break;
        case SDL_MOUSEWHEEL:
            handle_zoom(&event);
            render();
            break;
        }
    }
}

void handle_zoom(SDL_Event *event)
{
    if (event->wheel.y > 0 && pixel_size < MAX_PIXEL_SIZE)
        pixel_size += PIXEL_ZOOM_STEP;
    else if (event->wheel.y < 0 && pixel_size > MIN_PIXEL_SIZE)
        pixel_size -= PIXEL_ZOOM_STEP;
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    uint8_t *src_pixel;
    uint8_t r, g, b;

    for (uint16_t y = 0; y < current_image->header.height; y++)
        for (uint16_t x = 0; x < current_image->header.width; x++)
        {
            src_pixel = &current_image->pixel_data[(y * current_image->header.width + x) * 3];

            r = src_pixel[0];
            g = src_pixel[1];
            b = src_pixel[2];

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);

            SDL_Rect rect = {
                offset_x + x * pixel_size,
                offset_y + y * pixel_size,
                pixel_size,
                pixel_size};

            SDL_RenderFillRect(renderer, &rect);
        }

    SDL_RenderPresent(renderer);
}

void set_current_image(const char *path)
{
    current_image = load_grga_image(path);

    if (current_image->header.depth != 8 || current_image->header.channels < 1 || current_image->header.channels > 4)
    {
        perror("Unsupported image format.\n");
        exit(EXIT_FAILURE);
    }
}

void start_viewer_and_keep_running(const char *path)
{
    set_current_image(path);
    render();

    while (is_window_open == true)
    {
        poll_events();
    }
}

void cleanup(void)
{
    free(current_image);
    current_image = NULL;

    SDL_free(base_path);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}