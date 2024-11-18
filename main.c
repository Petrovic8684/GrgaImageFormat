#include "viewer/viewer.h"

int main(int argc, char *argv[])
{
    initialize_sdl();
    create_window_and_renderer("GrgaViewer");

    start_viewer_and_keep_running(argv[1]);

    cleanup();

    return EXIT_SUCCESS;
}