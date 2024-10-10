#include <stdlib.h>
#include <stdio.h>
#include "lib_ppm.h"


void apply_zoom_pattern(struct pixel_s pixel, struct image_s *output, int x, int y) {
    int r = pixel.r;
    int g = pixel.g;
    int b = pixel.b;

    
    if (r < 75 && g < 75 && b < 75) {
        // All sub-pixels are black
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 3; dx++) {
                output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = (struct pixel_s){0, 0, 0};
            }
        }
    } else if (r < 135 && g < 135 && b < 135) {
        // Middle pixel colored, others black
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 3; dx++) {
                if (dy == 1 && dx == 1) {
                    output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = pixel;  // Middle sub-pixel
                } else {
                    output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = (struct pixel_s){0, 0, 0};  // Black
                }
            }
        }
    } else if (r < 180 && g < 180 && b < 180) {
        // Outer pixels colored, middle black
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 3; dx++) {
                if (dy == 1 && dx == 1) {
                    output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = (struct pixel_s){0, 0, 0};  // Middle sub-pixel black
                } else {
                    output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = pixel;  // Colored
                }
            }
        }
    } else {
        // All sub-pixels are colored
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 3; dx++) {
                output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = pixel;  // All colored
            }
        }
    }
}


void zoom_image(struct image_s *input, struct image_s *output) {
    for (int y = 0; y < input->height; y++) {
        for (int x = 0; x < input->width; x++) {
            struct pixel_s pixel = input->pix[y * input->width + x];
            apply_zoom_pattern(pixel, output, x, y);
        }
    }
}

int main() {
    struct image_s input_image;
    struct image_s output_image;

    
    if (read_ppm("lena.ppm", &input_image) != 0) {
        fprintf(stderr, "Error reading the image file.\n");
        return -1;
    }

    
    if (new_ppm(&output_image, input_image.width * 3, input_image.height * 3) != 0) {
        fprintf(stderr, "Error allocating memory for the zoomed image.\n");
        free_ppm(&input_image);
        return -1;
    }

    
    zoom_image(&input_image, &output_image);

   
    if (write_ppm("zoomed_lena.ppm", &output_image) != 0) {
        fprintf(stderr, "Error writing the zoomed image file.\n");
    }

    
    free_ppm(&input_image);
    free_ppm(&output_image);

    return 0;
}
