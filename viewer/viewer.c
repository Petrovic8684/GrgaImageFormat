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

void start_viewer_and_keep_running(void)
{
    /*uint8_t data[] = {20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0, 20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0, 20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0, 20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0, 20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0, 20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0, 20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0, 20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0};
    struct grga_image *new_image = construct_grga_image(10, 10, 3, 8, data);

    if (new_image != NULL)
    {
        save_grga_image("square.grga", new_image);
        free(new_image);
        new_image = NULL;
    }*/

    current_image = load_grga_image("square.grga");

    if (current_image != NULL)
    {
        print_grga_image_data(current_image);
        render(current_image, 40);
        free(current_image);
        current_image = NULL;
    }

    while (is_window_open == true)
    {
        poll_events();
    }
}

void cleanup(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

/*
    uint16_t width = 10, height = 10;
    uint8_t channels = 3, depth = 8; // RGBA, 8-bit channels

    struct grga_image *image = malloc(sizeof(struct grga_header) + sizeof(uint8_t) * width * height * channels);

    strcpy(image->header.identifier, VALID_IDENTIFIER);
    image->header.width = width;
    image->header.height = height;
    image->header.channels = channels;
    image->header.depth = depth;

    uint8_t data[] = {20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0,
                      20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0,
                      20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0,
                      20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0,
                      20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0,
                      20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0,
                      20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0,
                      20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0,
                      20, 255, 0, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 255, 90, 50, 20, 255, 0,
                      20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0, 20, 255, 0};

    memcpy(image->pixel_data, data, sizeof(uint8_t) * image->header.width * image->header.height * image->header.channels);

    save_grga_image("test.grga", image);
*/