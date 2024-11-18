#include "util.h"

struct grga_image *construct_grga_image(uint16_t width, uint16_t height, uint8_t channels, uint8_t depth, uint8_t data[])
{
    struct grga_image *image = malloc(sizeof(struct grga_header) + sizeof(uint8_t) * width * height * channels);
    if (image == NULL)
    {
        perror("Error while allocating memory!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(image->header.identifier, VALID_IDENTIFIER);
    image->header.width = width;
    image->header.height = height;
    image->header.channels = channels;
    image->header.depth = depth;

    memcpy(image->pixel_data, data, sizeof(uint8_t) * image->header.width * image->header.height * image->header.channels);

    return image;
}

void save_grga_image(const char *path, struct grga_image *image)
{
    FILE *file = fopen(path, "wb");

    if (file == NULL)
    {
        perror("Error while loading file!\n");
        return;
    }

    fwrite(&(image->header), sizeof(struct grga_header), 1, file);
    fwrite(image->pixel_data, sizeof(uint8_t), image->header.width * image->header.height * image->header.channels, file);

    fclose(file);
    fprintf(stdout, "File successfully written to disc!\n");
}

void compress_grga_image(struct grga_image *image)
{
}

void decompress_grga_image(struct grga_image *image)
{
}

struct grga_image *load_grga_image(const char *path)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        perror("Error while loading file!\n");
        exit(EXIT_FAILURE);
    }

    struct grga_header *header = malloc(sizeof(struct grga_header));
    if (header == NULL)
    {
        fclose(file);

        perror("Error while allocating memory!\n");
        exit(EXIT_FAILURE);
    }

    fread(header, sizeof(struct grga_header), 1, file);

    struct grga_image *image = malloc(sizeof(struct grga_header) + sizeof(uint8_t) * (header->width) * (header->height) * (header->channels));
    if (image == NULL)
    {
        free(header);
        header = NULL;
        fclose(file);

        perror("Error while allocating memory!\n");
        exit(EXIT_FAILURE);
    }

    memcpy(&(image->header), header, sizeof(struct grga_header));

    if (strcmp(image->header.identifier, VALID_IDENTIFIER))
    {
        free(header);
        header = NULL;
        free(image);
        image = NULL;
        fclose(file);

        perror("File format not valid!\n");
        exit(EXIT_FAILURE);
    }

    free(header);
    header = NULL;

    fread(image->pixel_data, sizeof(uint8_t), image->header.width * image->header.height * image->header.channels, file);

    fclose(file);

    fprintf(stdout, "Image successfully read!\n");
    return image;
}

void print_grga_image_data(struct grga_image *image)
{
    fprintf(stdout, "\n*** HEADER ***\n\nImage identifier: %s\nImage dimensions: %ux%u\nImage channels: %u\nImage depth: %u\n\n*** PIXEL DATA ***\n\n", image->header.identifier, image->header.width, image->header.height, image->header.channels, image->header.depth);

    for (int i = 0; i < image->header.width * image->header.height * image->header.channels; i++)
    {
        fprintf(stdout, "%d ", image->pixel_data[i]);

        if ((i + 1) % (image->header.channels) == 0)
        {
            fprintf(stdout, "\t");
        }

        if ((i + 1) % (image->header.width * image->header.channels) == 0)
        {
            fprintf(stdout, "\n");
        }
    }
}