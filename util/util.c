#include "util.h"

void save_grga_image(const char *filename, const struct grga_image image)
{
    FILE *file = fopen(filename, "wb");

    if (file == NULL)
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

void decompress_grga_image(struct grga_image *image)
{
}

struct grga_image *load_grga_image(const char *filename)
{
    FILE *file = fopen(filename, "rb");

    if (file == NULL)
    {
        perror("Error while loading file!\n");
        return NULL;
    }

    struct grga_image *image = (struct grga_image *)malloc(sizeof(struct grga_image));

    if (image == NULL)
    {
        perror("Memory allocation failed!\n");
        fclose(file);

        return NULL;
    }

    fread(image, sizeof(struct grga_image), 1, file);

    fclose(file);

    if (strcmp(image->header.identifier, VALID_IDENTIFIER))
    {
        perror("File format not valid!\n");
        return NULL;
    }

    fprintf(stdout, "Image successfully read!\n");

    return image;
}

void print_grga_image_data(const struct grga_image image)
{
    fprintf(stdout, "\n*** HEADER ***\n\nImage dimensions: %ux%u\nImage channels: %u\nImage depth: %u\n\n*** PIXEL DATA ***\n\n", image.header.width, image.header.height, image.header.channels, image.header.depth);

    for (uint8_t i = 0; i < image.header.width * image.header.height * image.header.channels; i++)
    {
        fprintf(stdout, "%d ", image.pixel_data[i]);

        if ((i + 1) % (image.header.channels) == 0)
        {
            fprintf(stdout, "\t");
        }

        if ((i + 1) % (image.header.width * image.header.channels) == 0)
        {
            fprintf(stdout, "\n");
        }
    }
}