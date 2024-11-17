#include "viewer.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool is_window_open = true;

void initialize_sdl(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL failed to initialize: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void create_window_and_renderer(const char *title)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (window == NULL)
    {
        fprintf(stderr, "SDL window failed to initialize: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (renderer == NULL)
    {
        fprintf(stderr, "SDL renderer failed to initialize: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /*SDL_Surface *icon = IMG_Load("../assets/icon.ico");

    if (icon == NULL)
    {
        fprintf(stdout, "Unable to load icon!\n");
        return;
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

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // TODO: rendering image

    SDL_RenderPresent(renderer);
}

void start_viewer_and_keep_running(void)
{
    /*uint16_t width = 2, height = 2;
    uint8_t channels = 3, depth = 8; // RGB, 8-bit channels

    struct grga_image image = {
        {VALID_IDENTIFIER, width, height, channels, depth},
        {255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0}};

    save_grga_image("test.grga", image);*/

    struct grga_image result = load_grga_image("test.grga");
    print_grga_image_data(result);

    while (is_window_open == true)
    {
        render();
        poll_events();
    }
}

void cleanup(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}