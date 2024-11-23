#include "converter.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int window_width = 800;
int window_height = 600;

bool is_window_open = true;

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

        render();
    }
}

void handle_file_drop(SDL_Event *event)
{
    // set_current_image(event->drop.file);
    render();
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // if (current_image == NULL)
    //{
    render_text(renderer, "Drag and drop any .jpeg or .png image into converter to start.", window_width, window_height, 0, -20, 30);
    render_text(renderer, "Alternatively, drag and drop a directory containing .jpeg or .png images.", window_width, window_height, 0, 20, 24);
    SDL_RenderPresent(renderer);

    //    return;
    //}

    SDL_RenderPresent(renderer);
}

/*void set_current_image(const char *path)
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
}*/

void run_converter(const char *path)
{
    load_window_icon(window, "assets/loop.png");

    // if (path != NULL)
    // set_current_image(path);

    render();

    while (is_window_open == true)
        poll_events();
}

void cleanup(void)
{
    // free(current_image);
    // current_image = NULL;

    gui_cleanup();
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}