#include <stdio.h>

typedef struct pixelProperties {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct imageProperties {
    Pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
    char type[4];
    int maxColor;
} Image;

Image readImage();
Image grayScale(Image img);
Image sepia(Image img);
Image blur(Image img);
Image rotate90Right(Image img);
Image mirroringImage(Image img, int horizontally);
Image invertColors(Image img);
Image cutImage(Image img, int x, int y, int width, int height);
void printImage(Image img);

int main() {
    Image img;

    img = readImage();

    int numberOpcions;
    scanf("%d", &numberOpcions);

    for(int i = 0; i < numberOpcions; ++i) {
        int opcion;
        scanf("%d", &opcion);

        switch(opcion) {

            case 1: {
                img = grayScale(img);
                break;
            }

            case 2: {
                img = sepia(img);
                break;
            }

            case 3: {
                img = blur(img);
                break;
            }

            case 4: {
                int howManyTimes = 0;
                scanf("%d", &howManyTimes);
                howManyTimes %= 4;
                for (int j = 0; j < howManyTimes; ++j) {
                    img = rotate90Right(img);
                }
                break;
            }

            case 5: {
                int horizontally = 0;
                scanf("%d", &horizontally);

                img = mirroringImage(img, horizontally);
                break;
            }

            case 6: {
                img = invertColors(img);
                break;
            }

            case 7: {
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cutImage(img, x, y, width, height);
                break;
            }
        }
    }

    printImage(img);

    return 0;
}

Image readImage(){
  Image img;

  scanf("%s", img.type);
  scanf("%u %u", &img.width, &img.height);
  scanf("%d", &img.maxColor);

  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          scanf("%hu %hu %hu", &img.pixel[i][j].red,
                               &img.pixel[i][j].green,
                               &img.pixel[i][j].blue);

      }
  }
  return img;
}

Image grayScale(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {

            int average = img.pixel[i][j].red +
                          img.pixel[i][j].green +
                          img.pixel[i][j].blue;

            average /= 3;

            img.pixel[i][j].red = average;
            img.pixel[i][j].green = average;
            img.pixel[i][j].blue = average;
        }
    }

    return img;
}

Image sepia(Image img) {

  for (unsigned int x = 0; x < img.height; ++x) {
      for (unsigned int j = 0; j < img.width; ++j) {
          unsigned short int pixel[3];
          pixel[0] = img.pixel[x][j].red;
          pixel[1] = img.pixel[x][j].green;
          pixel[2] = img.pixel[x][j].blue;

          int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          int minimunRed = (255 >  p) ? p : 255;
          img.pixel[x][j].red = minimunRed;

          p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          minimunRed = (255 >  p) ? p : 255;
          img.pixel[x][j].green = minimunRed;

          p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          minimunRed = (255 >  p) ? p : 255;
          img.pixel[x][j].blue = minimunRed;
      }
  }

  return img;
}

Image blur(Image img) {

    int size = 0;
    scanf("%d", &size);

        for (unsigned int i = 0; i < img.height; ++i) {
          for (unsigned int j = 0; j < img.width; ++j) {

            Pixel average = {0, 0, 0};

            int minHeight = (img.height - 1 > i + size/2) ? i + size/2 : img.height - 1;
            int minWidth = (img.width - 1 > j + size/2) ? j + size/2 : img.width - 1;

              for(int x = (0 > i - size/2 ? 0 : i - size/2); x <= minHeight; ++x) {
                for(int y = (0 > j - size/2 ? 0 : j - size/2); y <= minWidth; ++y) {
                    average.red += img.pixel[x][y].red;
                    average.green += img.pixel[x][y].green;
                    average.blue += img.pixel[x][y].blue;
                }
            }

            average.red /= size * size;
            average.green /= size * size;
            average.blue /= size * size;

            img.pixel[i][j].red = average.red;
            img.pixel[i][j].green = average.green;
            img.pixel[i][j].blue = average.blue;
        }
    }
    return img;
}

Image rotate90Right(Image img) {
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {

            rotated.pixel[i][j].red = img.pixel[x][y].red;
            rotated.pixel[i][j].green = img.pixel[x][y].green;
            rotated.pixel[i][j].blue = img.pixel[x][y].blue;
        }
    }

    return rotated;
}

Image mirroringImage(Image img, int horizontally) {

  int width = img.width;
  int height = img.height;

  if (horizontally == 1) width /= 2;
  else height /= 2;

  for (int i2 = 0; i2 < height; ++i2) {
      for (int j = 0; j < width; ++j) {
          int x = i2, y = j;

          if (horizontally == 1) y = img.width - 1 - j;
          else x = img.height - 1 - i2;

          Pixel aux1;
          aux1.red = img.pixel[i2][j].red;
          aux1.green = img.pixel[i2][j].green;
          aux1.blue = img.pixel[i2][j].blue;

          img.pixel[i2][j].red = img.pixel[x][y].red;
          img.pixel[i2][j].green = img.pixel[x][y].green;
          img.pixel[i2][j].blue = img.pixel[x][y].blue;

          img.pixel[x][y].red = aux1.red;
          img.pixel[x][y].green = aux1.green;
          img.pixel[x][y].blue = aux1.blue;
      }
  }
  return img;
}

Image invertColors(Image img) {
    Image invert;

      for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            invert.pixel[i][j].red = 255 - img.pixel[i][j].red;
            invert.pixel[i][j].green = 255 - img.pixel[i][j].green;
            invert.pixel[i][j].blue = 255 - img.pixel[i][j].blue;
        }
    }

    return img;
}

Image cutImage(Image img, int x, int y, int width, int height) {
    Image cutted;

    cutted.width = width;
    cutted.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {

            cutted.pixel[i][j].red = img.pixel[i + y][j + x].red;
            cutted.pixel[i][j].green = img.pixel[i + y][j + x].green;
            cutted.pixel[i][j].blue = img.pixel[i + y][j + x].blue;
        }
    }

    return cutted;
}

void printImage(Image img){

  printf("P3\n");
  printf("%u %u\n255\n", img.width, img.height);

  for (unsigned int i = 0; i < img.height; ++i) {
    for (unsigned int j = 0; j < img.width; ++j) {
        printf("%hu %hu %hu ", img.pixel[i][j].red,
                               img.pixel[i][j].green,
                               img.pixel[i][j].blue);

    }
    printf("\n");
  }
}
