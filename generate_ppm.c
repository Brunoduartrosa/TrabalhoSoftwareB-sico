#include <stdio.h>

int main() {
    int width = 200;    // largura da imagem
    int height = 200;   // altura da imagem
    FILE *fp = fopen("example.ppm", "wb");  // Abre o arquivo em modo binário

    if (!fp) {
        fprintf(stderr, "Erro ao criar o arquivo.\n");
        return 1;
    }

    // Escrevendo o cabeçalho da imagem
    fprintf(fp, "P6\n%d %d\n255\n", width, height);

    // Gerando dados da imagem: um gradiente simples de cores
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char r = (unsigned char)(x * 255 / width);   // Gradiente de vermelho
            unsigned char g = (unsigned char)(y * 255 / height);  // Gradiente de verde
            unsigned char b = 128;                                  // Azul fixo
            fwrite(&r, sizeof(unsigned char), 1, fp); // Grava o vermelho
            fwrite(&g, sizeof(unsigned char), 1, fp); // Grava o verde
            fwrite(&b, sizeof(unsigned char), 1, fp); // Grava o azul
        }
    }

    fclose(fp);
    printf("Imagem PPM gerada: example.ppm\n");
    return 0;
}
