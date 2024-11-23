#include "converter.h"

int main(int argc, char *argv[])
{
    initialize_sdl();
    create_window_and_renderer("GrgaConverter");

    run_converter(argv[1]);

    cleanup();
    return EXIT_SUCCESS;
}