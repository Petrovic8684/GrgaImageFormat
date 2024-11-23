#include "util.h"

char image_files[256][256];
uint16_t image_count = 0;

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
        fprintf(stdout, "Error while loading file!\n");
        return NULL;
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

    if (strcmp(image->header.identifier, VALID_IDENTIFIER) || image->header.depth != IMAGE_DEPTH || image->header.channels < MIN_CHANNELS || image->header.channels > MAX_CHANNELS)
    {
        free(header);
        header = NULL;
        free(image);
        image = NULL;
        fclose(file);

        fprintf(stdout, "File not a .grga image!\n");
        return NULL;
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

void search_directory_contents(const char *path)
{
    fprintf(stdout, "Interpreting path as a directory...\n");
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        perror("Error opening directory!\n");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    char full_path[256];
    while ((entry = readdir(dir)) != NULL)
    {
        if (strstr(entry->d_name, ".grga"))
        {
            strcpy(full_path, path);
            strcat(full_path, "/");
            strcat(full_path, entry->d_name);

            printf("%s\n", full_path);
            strcpy(image_files[image_count++], full_path);
            strcpy(full_path, "");
        }
    }

    closedir(dir);
}