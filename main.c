#include "viewer/viewer.h"

int main(int argc, char *argv[])
{
    initialize_sdl();
    create_window_and_renderer("GrgaViewer");

    run_viewer(argv[1]);

    cleanup();
    return EXIT_SUCCESS;
}