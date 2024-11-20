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

extern char image_files[100][256];
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

struct grga_image *construct_grga_image(uint16_t, uint16_t, uint8_t, uint8_t, uint8_t[]);
void save_grga_image(const char *, struct grga_image *);
void compress_grga_image(struct grga_image *);
void decompress_grga_image(struct grga_image *);
struct grga_image *load_grga_image(const char *);
void print_grga_image_data(struct grga_image *);
void search_directory_contents(const char *);
uint16_t find_index_by_name(const char *, const char[100][256], uint16_t);
const char *get_filename_from_path(const char *);

#endif