#include "util.h"

void save_grga_image(const char *filename, const struct grga_image image)
{
    FILE *file = fopen(filename, "wb");

    if (!file)
    {
        perror("Error while loading file!\n");
        return;
    }

    fwrite(&image, sizeof(struct grga_image), 1, file);

    fclose(file);

    fprintf(stdout, "File successfully written to disc!\n");
}

void compress_grga_image(struct grga_image *image)
{
}

struct grga_image *load_grga_image(const char *filename)
{
    FILE *file = fopen(filename, "rb");

    if (!file)
    {
        perror("Error while loading file!\n");
        return;
    }

    struct grga_image image;
    fread(&image, sizeof(struct grga_image), 1, file);

    fclose(file);

    if (strcmp(image.header.identifier, VALID_IDENTIFIER))
    {
        fprintf(stdout, "File format not valid!\n");
        return NULL;
    }

    fprintf(stdout, "Image dimensions: %ux%u\nImage channels: %u\nImage depth: %u\n", image.header.width, image.header.height, image.header.channels, image.header.depth);

    for (size_t i = 0; i < image.pixel_data && i < 10; i++)
    {
        printf("%u ", image.pixel_data[i]);
    }

    return &image;
}

void main()
{
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

    return 0;
}