#include <stdio.h>
#include <stdlib.h>
#include "lib_ppm.h" // Biblioteca fornecida para manipulação de imagens PPM

// Função auxiliar para gerar matriz 3x3 de sub-pixels para um canal de cor
void gerar_subpixels(unsigned char cor, unsigned char subpixel[3][3]) {
    if (cor <= 74) {
        // Todos pretos
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                subpixel[i][j] = 0;
            }
        }
    } else if (cor <= 134) {
        // Apenas o subpixel do meio ligado
        subpixel[0][0] = subpixel[0][1] = subpixel[0][2] = 0;
        subpixel[1][0] = subpixel[1][2] = 0;
        subpixel[1][1] = 1; // Subpixel do meio
    } else if (cor <= 179) {
        // Subpixel do meio desligado, os outros dois ligados
        subpixel[0][0] = subpixel[0][1] = subpixel[0][2] = 1;
        subpixel[1][1] = 0;
    } else {
        // Todos ligados
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                subpixel[i][j] = 1;
            }
        }
    }
}

// Função principal de zoom com resolução de sub-pixel
void aplicar_zoom(struct image_s *image, struct image_s *output_image) {
    // Alocação da nova imagem com 3x o tamanho da original
    output_image->width = image->width * 3;
    output_image->height = image->height * 3;
    output_image->pix = (struct pixel_s*) malloc(output_image->width * output_image->height * sizeof(struct pixel_s));

    // Processamento de cada pixel da imagem original
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            // Obter o pixel atual
            struct pixel_s pixel = image->pix[y * image->width + x];
            
            // Gerar subpixels para cada cor
            unsigned char subpixel_r[3][3], subpixel_g[3][3], subpixel_b[3][3];
            gerar_subpixels(pixel.r, subpixel_r);
            gerar_subpixels(pixel.g, subpixel_g);
            gerar_subpixels(pixel.b, subpixel_b);

            // Preencher os subpixels na imagem de saída
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int new_x = x * 3 + j;
                    int new_y = y * 3 + i;
                    struct pixel_s *new_pixel = &output_image->pix[new_y * output_image->width + new_x];

                    // Configurar as cores do subpixel
                    new_pixel->r = subpixel_r[i][j] ? 255 : 0;
                    new_pixel->g = subpixel_g[i][j] ? 255 : 0;
                    new_pixel->b = subpixel_b[i][j] ? 255 : 0;
                }
            }
        }
    }
}

int main() {
    struct image_s input_image, output_image;

    // Leitura da imagem original
    if (read_ppm("example.ppm", &input_image) != 0) {
        printf("Erro ao ler a imagem de entrada.\n");
        return 1;
    }

    // Aplicar zoom com resolução de sub-pixel
    aplicar_zoom(&input_image, &output_image);

    // Escrita da nova imagem
    if (write_ppm("input.ppm", &output_image) != 0) {
        printf("Erro ao escrever a imagem de saída.\n");
        return 1;
    }

    // Liberação de memória
    free_ppm(&input_image);
    free_ppm(&output_image);

    return 0;
}
