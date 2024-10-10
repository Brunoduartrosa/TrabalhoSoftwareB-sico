#include <stdlib.h>
#include <stdio.h>
#include "lib_ppm.h"

// Função que aplica um padrão de zoom baseado na cor do pixel
void apply_zoom_pattern(struct pixel_s pixel, struct image_s *output, int x, int y) {
    int r = pixel.r;
    int g = pixel.g;
    int b = pixel.b;

    // Definição de sub-pixels para o nível de vermelho
    struct pixel_s r_subpixels[3];
    if (r < 75) {
        // Todos os sub-pixels são pretos
        r_subpixels[0] = (struct pixel_s){0, 0, 0};
        r_subpixels[1] = (struct pixel_s){0, 0, 0};
        r_subpixels[2] = (struct pixel_s){0, 0, 0};
    } else if (r < 135) {
        // Sub-pixel do meio colorido, outros pretos
        r_subpixels[0] = (struct pixel_s){0, 0, 0};
        r_subpixels[1] = (struct pixel_s){r, 0, 0};
        r_subpixels[2] = (struct pixel_s){0, 0, 0};
    } else if (r < 180) {
        // Sub-pixel do meio preto, outros coloridos
        r_subpixels[0] = (struct pixel_s){r, 0, 0};
        r_subpixels[1] = (struct pixel_s){0, 0, 0};
        r_subpixels[2] = (struct pixel_s){r, 0, 0};
    } else {
        // Todos os sub-pixels são coloridos
        r_subpixels[0] = (struct pixel_s){r, 0, 0};
        r_subpixels[1] = (struct pixel_s){r, 0, 0};
        r_subpixels[2] = (struct pixel_s){r, 0, 0};
    }

    // Definição de sub-pixels para o nível de verde
    struct pixel_s g_subpixels[3];
    if (g < 75) {
        g_subpixels[0] = (struct pixel_s){0, 0, 0};
        g_subpixels[1] = (struct pixel_s){0, 0, 0};
        g_subpixels[2] = (struct pixel_s){0, 0, 0};
    } else if (g < 135) {
        g_subpixels[0] = (struct pixel_s){0, 0, 0};
        g_subpixels[1] = (struct pixel_s){0, g, 0};
        g_subpixels[2] = (struct pixel_s){0, 0, 0};
    } else if (g < 180) {
        g_subpixels[0] = (struct pixel_s){0, g, 0};
        g_subpixels[1] = (struct pixel_s){0, 0, 0};
        g_subpixels[2] = (struct pixel_s){0, g, 0};
    } else {
        g_subpixels[0] = (struct pixel_s){0, g, 0};
        g_subpixels[1] = (struct pixel_s){0, g, 0};
        g_subpixels[2] = (struct pixel_s){0, g, 0};
    }

    // Definição de sub-pixels para o nível de azul
    struct pixel_s b_subpixels[3];
    if (b < 75) {
        b_subpixels[0] = (struct pixel_s){0, 0, 0};
        b_subpixels[1] = (struct pixel_s){0, 0, 0};
        b_subpixels[2] = (struct pixel_s){0, 0, 0};
    } else if (b < 135) {
        b_subpixels[0] = (struct pixel_s){0, 0, 0};
        b_subpixels[1] = (struct pixel_s){0, 0, b};
        b_subpixels[2] = (struct pixel_s){0, 0, 0};
    } else if (b < 180) {
        b_subpixels[0] = (struct pixel_s){0, 0, b};
        b_subpixels[1] = (struct pixel_s){0, 0, 0};
        b_subpixels[2] = (struct pixel_s){0, 0, b};
    } else {
        b_subpixels[0] = (struct pixel_s){0, 0, b};
        b_subpixels[1] = (struct pixel_s){0, 0, b};
        b_subpixels[2] = (struct pixel_s){0, 0, b};
    }

    // Aplica os sub-pixels na imagem de saída
    // Primeiro para o canal vermelho
    output->pix[(y * 3) * output->width + (x * 3)] = r_subpixels[0];
    output->pix[(y * 3 + 1) * output->width + (x * 3)] = r_subpixels[1];
    output->pix[(y * 3 + 2) * output->width + (x * 3)] = r_subpixels[2];

    // Depois para o canal verde
    output->pix[(y * 3) * output->width + (x * 3 + 1)] = g_subpixels[0];
    output->pix[(y * 3 + 1) * output->width + (x * 3 + 1)] = g_subpixels[1];
    output->pix[(y * 3 + 2) * output->width + (x * 3 + 1)] = g_subpixels[2];

    // Finalmente para o canal azul
    output->pix[(y * 3) * output->width + (x * 3 + 2)] = b_subpixels[0];
    output->pix[(y * 3 + 1) * output->width + (x * 3 + 2)] = b_subpixels[1];
    output->pix[(y * 3 + 2) * output->width + (x * 3 + 2)] = b_subpixels[2];
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
