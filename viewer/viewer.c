#include "viewer.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int window_width = 800;
int window_height = 600;

struct grga_image *current_image = NULL;
uint16_t current_image_index = 0;

uint8_t pixel_size = 1;
int offset_x = 1, offset_y = 1;

bool is_window_open = true;
char *base_path = NULL;

struct button btn_prev, btn_next;
struct slider zoom_slider;

TTF_Font *font;

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

    font = TTF_OpenFont("assets/runescape.ttf", FONT_SIZE);
    if (!font)
    {
        perror("Failed to load font!\n");
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

    char *icon_path;
    sprintf(icon_path, "%s%s", base_path, "assets/icon.png");

    SDL_Surface *icon = IMG_Load(icon_path);

    if (icon == NULL)
    {
        perror("Unable to load icon!\n");
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    free(icon_path);
    icon_path = NULL;
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
            break;
        case SDL_MOUSEBUTTONUP:
            detect_click_release_from_slider(&event, &zoom_slider);
            break;
        case SDL_MOUSEMOTION:
            detect_drag_slider(&event, &zoom_slider, &pixel_size, &render);
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
    if (current_image_index <= 0)
        return;

    set_current_image(image_files[--current_image_index]);
    render();
}

void change_to_next_image(void)
{
    if (current_image_index >= image_count - 1)
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
    set_current_image(event->drop.file);
    render();
}

void init_gui(void)
{
    btn_prev = (struct button){
        .rect = {10, window_height / 2 - 20, 60, 40},
        .label = "<",
        .callback = change_to_previous_image};

    btn_next = (struct button){
        .rect = {window_width - 70, window_height / 2 - 20, 60, 40},
        .label = ">",
        .callback = change_to_next_image};

    initialize_slider(&zoom_slider, window_width, window_height, 300, 20, MIN_PIXEL_SIZE, MAX_PIXEL_SIZE, pixel_size);
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    if (current_image == NULL)
    {
        render_welcome_message(renderer, window_width, window_height, font);
        SDL_RenderPresent(renderer);
        return;
    }

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
                (window_width - current_image->header.width) / 2 + offset_x + x * pixel_size,
                (window_height - current_image->header.height) / 3 + offset_y + y * pixel_size,
                pixel_size,
                pixel_size};

            SDL_RenderFillRect(renderer, &rect);
        }

    if (image_count > 0)
    {
        render_button(renderer, &btn_prev, font);
        render_button(renderer, &btn_next, font);
    }
    render_slider(renderer, &zoom_slider);

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
            perror("Invalid file path!\n");
            exit(EXIT_FAILURE);
        }

        current_image = load_grga_image(image_files[0]);
    }
}

void run_viewer(const char *path)
{
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

    SDL_free(base_path);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // SDL_Quit(); // TODO: See why this line causes a seg fault.
}