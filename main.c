#include <stdio.h>
#define RED 0
#define GREEN 1
#define BLUE 2

typedef struct pixelProperties {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct imageProperties {
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;

Image grayScale(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int average = img.pixel[i][j][RED] +
                          img.pixel[i][j][GREEN] +
                          img.pixel[i][j][BLUE];

            average /= 3;

            img.pixel[i][j][RED] = average;
            img.pixel[i][j][GREEN] = average;
            img.pixel[i][j][BLUE] = average;
        }
    }

    return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], int size, unsigned int width) {

    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel average = {0, 0, 0};

            int menor_height = (height - 1 > i + size/2) ? i + size/2 : height - 1;
            int min_width = (width - 1 > j + size/2) ? j + size/2 : width - 1;
            for(int x = (0 > i - size/2 ? 0 : i - size/2); x <= menor_height; ++x) {
                for(int y = (0 > j - size/2 ? 0 : j - size/2); y <= min_width; ++y) {
                    average.red += pixel[x][y][RED];
                    average.green += pixel[x][y][GREEN];
                    average.blue += pixel[x][y][BLUE];
                }
            }

            average.red /= size * size;
            average.green /= size * size;
            average.blue /= size * size;

            pixel[i][j][RED] = average.red;
            pixel[i][j][GREEN] = average.green;
            pixel[i][j][BLUE] = average.blue;
        }
    }
}

Image rotate90Right(Image img) {
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {

            rotated.pixel[i][j][RED] = img.pixel[x][y][RED];
            rotated.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
            rotated.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];
        }
    }

    return rotated;
}

void invertColors(unsigned short int pixel[512][512][3], unsigned int width, unsigned int height) {

      for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][RED] = 255 - pixel[i][j][RED];
            pixel[i][j][GREEN] = 255 - pixel[i][j][GREEN];
            pixel[i][j][BLUE] = 255 - pixel[i][j][BLUE];
        }
    }
}

Image cutImage(Image img, int x, int y, int width, int height) {
    Image cutted;

    cutted.width = width;
    cutted.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {

            cutted.pixel[i][j][RED] = img.pixel[i + y][j + x][RED];
            cutted.pixel[i][j][GREEN] = img.pixel[i + y][j + x][GREEN];
            cutted.pixel[i][j][BLUE] = img.pixel[i + y][j + x][BLUE];
        }
    }

    return cutted;
}


int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int maxColor;
    scanf("%u %u %d", &img.width, &img.height, &maxColor);

    // read all pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][RED],
                                 &img.pixel[i][j][GREEN],
                                 &img.pixel[i][j][BLUE]);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = grayScale(img);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x = 0; x < img.height; ++x) {
                    for (unsigned int j = 0; j < img.width; ++j) {
                        unsigned short int pixel[3];
                        pixel[RED] = img.pixel[x][j][RED];
                        pixel[GREEN] = img.pixel[x][j][GREEN];
                        pixel[BLUE] = img.pixel[x][j][BLUE];

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int minimunRed = (255 >  p) ? p : 255;
                        img.pixel[x][j][RED] = minimunRed;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        minimunRed = (255 >  p) ? p : 255;
                        img.pixel[x][j][GREEN] = minimunRed;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        minimunRed = (255 >  p) ? p : 255;
                        img.pixel[x][j][BLUE] = minimunRed;
                    }
                }

                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.height, img.pixel, tamanho, img.width);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotate90Right(img);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int width = img.width, height = img.height;

                if (horizontal == 1) width /= 2;
                else height /= 2;

                for (int i2 = 0; i2 < height; ++i2) {
                    for (int j = 0; j < width; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = img.width - 1 - j;
                        else x = img.height - 1 - i2;

                        Pixel aux1;
                        aux1.red = img.pixel[i2][j][RED];
                        aux1.green = img.pixel[i2][j][GREEN];
                        aux1.blue = img.pixel[i2][j][BLUE];

                        img.pixel[i2][j][RED] = img.pixel[x][y][RED];
                        img.pixel[i2][j][GREEN] = img.pixel[x][y][GREEN];
                        img.pixel[i2][j][BLUE] = img.pixel[x][y][BLUE];

                        img.pixel[x][y][RED] = aux1.red;
                        img.pixel[x][y][GREEN] = aux1.green;
                        img.pixel[x][y][BLUE] = aux1.blue;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                invertColors(img.pixel, img.width, img.height);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cutImage(img, x, y, width, height);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][RED],
                                   img.pixel[i][j][GREEN],
                                   img.pixel[i][j][BLUE]);

        }
        printf("\n");
    }
    return 0;
}
