#include "viewer.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
struct grga_image *current_image = NULL;

bool is_window_open = true;

void initialize_sdl(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        perror("SDL failed to initialize!\n");
        exit(EXIT_FAILURE);
    }
}

void create_window_and_renderer(const char *title)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

    /*SDL_Surface *icon = IMG_Load("../assets/icon.ico");

    if (icon == NULL)
    {
        perror("Unable to load icon!\n");
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowIcon(window, icon);*/
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
        }
    }
}

void render(struct grga_image *image, uint8_t pixel_size)
{
    if (image->header.depth != 8 || image->header.channels < 1 || image->header.channels > 4)
    {
        perror("Unsupported image format.\n");
        exit(EXIT_FAILURE);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (uint16_t y = 0; y < image->header.height; ++y)
    {
        for (uint16_t x = 0; x < image->header.width; ++x)
        {
            uint8_t *src_pixel = &image->pixel_data[(y * image->header.width + x) * 3];

            uint8_t r = src_pixel[0];
            uint8_t g = src_pixel[1];
            uint8_t b = src_pixel[2];
            uint8_t a = 255;

            SDL_SetRenderDrawColor(renderer, r, g, b, a);

            SDL_Rect rect = {
                x * pixel_size,
                y * pixel_size,
                pixel_size,
                pixel_size};

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
}

void set_current_image(const char *path)
{
    current_image = load_grga_image(path);

    if (current_image != NULL)
    {
        // print_grga_image_data(current_image);
        render(current_image, PIXEL_SIZE);
    }
}

void start_viewer_and_keep_running(const char *path)
{
    set_current_image(path);

    while (is_window_open == true)
    {
        poll_events();
    }
}

void cleanup(void)
{
    free(current_image);
    current_image = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}