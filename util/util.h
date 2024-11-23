#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <dirent.h>

#define VALID_IDENTIFIER "GRGA"
#define IMAGE_DEPTH 8
#define MIN_CHANNELS 1
#define MAX_CHANNELS 4

extern char image_files[256][256];
extern uint16_t image_count;

struct grga_header
{
    char identifier[5];
    uint16_t width;
    uint16_t height;
    uint8_t channels;
    uint8_t depth;
};

struct grga_image
{
    struct grga_header header;
    uint8_t pixel_data[];
};

struct grga_image *construct_grga_image(uint16_t width, uint16_t height, uint8_t channels, uint8_t depth, uint8_t data[]);
void save_grga_image(const char *path, struct grga_image *image);
void compress_grga_image(struct grga_image *image);
void decompress_grga_image(struct grga_image *image);
struct grga_image *load_grga_image(const char *path);
void print_grga_image_data(struct grga_image *image);
void search_directory_contents(const char *path);

#endif