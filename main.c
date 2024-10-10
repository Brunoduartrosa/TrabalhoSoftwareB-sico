#include <stdlib.h>
#include <stdio.h>
#include "lib_ppm.h"

// Função que aplica um padrão de zoom baseado na cor do pixel
void apply_zoom_pattern(struct pixel_s pixel, struct image_s *output, int x, int y) {
    int r = pixel.r;
    int g = pixel.g;
    int b = pixel.b;

    // Verifica as faixas de valores RGB e aplica diferentes padrões de zoom
    if (r < 75 && g < 75 && b < 75) {
        // Caso 1: Todos os sub-pixels são pretos se o pixel original é escuro
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 3; dx++) {
                output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = (struct pixel_s){0, 0, 0};
            }
        }
    } else if (r < 135 && g < 135 && b < 135) {
        // Caso 2: Apenas o pixel do meio é colorido, os outros são pretos
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 3; dx++) {
                // Aplica a cor original ao pixel do meio
                if (dy == 1 && dx == 1) {
                    output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = pixel;  // Middle sub-pixel
                // Todos os outros sub-pixels são pretos
                } else {
                    output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = (struct pixel_s){0, 0, 0};  // Black
                }
            }
        }
        // Caso 3: Os pixels externos são coloridos, o do meio é preto
    } else if (r < 180 && g < 180 && b < 180) {
        // Outer pixels colored, middle black
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 3; dx++) {
                // O sub-pixel do meio é preto
                if (dy == 1 && dx == 1) {
                    output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = (struct pixel_s){0, 0, 0};  // Middle sub-pixel black
                // Sub-pixels externos são coloridos
                } else {
                    output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = pixel;  // Colored
                }
            }
        }
    // Caso 4: Todos os sub-pixels são coloridos
    } else {
        // All sub-pixels are colored
        for (int dy = 0; dy < 3; dy++) {
            for (int dx = 0; dx < 3; dx++) {
                // Todos os sub-pixels recebem a cor original do pixel
                output->pix[(y * 3 + dy) * output->width + (x * 3 + dx)] = pixel;  // All colored
            }
        }
    }
}

// Função que aplica o zoom em toda a imagem, pixel por pixel
void zoom_image(struct image_s *input, struct image_s *output) {
    // Itera sobre cada pixel da imagem de entrada
    for (int y = 0; y < input->height; y++) {
        for (int x = 0; x < input->width; x++) {
            // Pega o pixel da posição (x, y) da imagem original
            struct pixel_s pixel = input->pix[y * input->width + x];
            // Aplica o padrão de zoom no pixel atual
            apply_zoom_pattern(pixel, output, x, y);
        }
    }
}

int main() {
    // Declaração das imagens de entrada e saída
    struct image_s input_image;
    struct image_s output_image;

    // Leitura da imagem PPM de entrada
    if (read_ppm("lena.ppm", &input_image) != 0) {
        // Se houver erro ao ler a imagem, imprime mensagem de erro
        fprintf(stderr, "Error reading the image file.\n");
        return -1;
    }

    // Criação da imagem de saída com dimensões 3 vezes maiores que a original
    if (new_ppm(&output_image, input_image.width * 3, input_image.height * 3) != 0) {
        // Se houver erro ao alocar memória para a imagem ampliada, imprime mensagem de erro
        fprintf(stderr, "Error allocating memory for the zoomed image.\n");
        // Libera a memória da imagem original e retorna erro
        free_ppm(&input_image);
        return -1;
    }

    // Aplica o zoom na imagem de entrada e preenche a imagem de saída
    zoom_image(&input_image, &output_image);

   // Grava a imagem ampliada em um arquivo PPM
    if (write_ppm("zoomed_lena.ppm", &output_image) != 0) {
        // Se houver erro ao gravar a imagem, imprime mensagem de erro
        fprintf(stderr, "Error writing the zoomed image file.\n");
        // Se a gravação for bem-sucedida, imprime a mensagem de sucesso e o tamanho da nova imagem
    }else{
        printf("Zoom aplicado com sucesso!\n");
        printf("Novo tamanho da imagem: %d x %d pixels\n", output_image.width, output_image.height);
    }

    // Libera a memória das imagens de entrada e saída
    free_ppm(&input_image);
    free_ppm(&output_image);

    return 0;
}
