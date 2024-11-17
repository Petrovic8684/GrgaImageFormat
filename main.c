#include "viewer/viewer.h"

int main(int argc, char *argv[])
{
    initialize_sdl();
    create_window_and_renderer("GrgaViewer");

    uint16_t width = 2, height = 2;
    uint8_t channels = 3, depth = 8; // RGB, 8-bit channels
    uint64_t pixel_data[] = {
        255, 0, 0, 255, 0, 0, // First row: red pixels
        255, 0, 0, 255, 0, 0  // Second row: red pixels
    };

    struct grga_image image = {
        {VALID_IDENTIFIER, width, height, channels, depth},
        pixel_data};

    save_grga_image("test.grga", image);

    struct grga_image *result = load_grga_image("test.grga");

    if (result != NULL)
    {
        print_grga_image_data(*result);
        free(result);
    }

    start_viewer_and_keep_running();

    cleanup();

    return EXIT_SUCCESS;
}