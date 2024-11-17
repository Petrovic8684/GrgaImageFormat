#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define VALID_IDENTIFIER "GRGA"

struct grga_header
{
    char identifier[5];
    uint16_t width;
    uint16_t height;
    uint8_t channels;
    uint8_t depth;
};

/*struct grga_metadata
{
};*/

struct grga_image
{
    struct grga_header header;
    uint64_t *pixel_data;
    // struct grga_metadata metadata;
};

void save_grga_image(const char *, const struct grga_image);
void compress_grga_image(struct grga_image *);
void decompress_grga_image(struct grga_image *);
struct grga_image *load_grga_image(const char *);
void print_grga_image_data(const struct grga_image);

#endif