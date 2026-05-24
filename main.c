#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Size of the generated image in char
#define WIDTH_CHAR 150

// Proportion between width and height of a char in the terminal
#define CHAR_WIDTH_HEIGHT 2.2


void
printCharMatrice(int width, int height, char matrice[width][height])
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            printf("%c", matrice[j][i]);
        }
        printf("\n");
    }
}


// Stores the sum of the brightness of every pixel on a char
typedef struct {
    int brightness;
    int pixels;
}charData;


int
main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./pngToAscii \"IMAGE NAME\"");
        return 1;
    }

    int imageWidth, imageHeight, nChanels;

    // Stores the image as an array of chars 
    unsigned char *data = stbi_load(argv[1], &imageWidth, &imageHeight, &nChanels, 1); 
    
    if(data == NULL)
    {
        printf("Error to load image");
        stbi_image_free(data);
        return 1;
    }

    // Array of some chars sorted by brightness
    char bChar[] = " .:-=+*#%@";

    // Pixel width and char width proportion
    float widthCharToPixel = (float)imageWidth / WIDTH_CHAR;

    // Uses width proportion to calculate height
    const int HEIGHT_CHAR = imageHeight / (widthCharToPixel * CHAR_WIDTH_HEIGHT); 

    // Pixel height and char height proportion
    float heightCharToPixel = (float)imageHeight / HEIGHT_CHAR;

    charData matrice[WIDTH_CHAR][HEIGHT_CHAR];
    char result[WIDTH_CHAR][HEIGHT_CHAR];

    // Clear matrice
    for(int i = 0; i < HEIGHT_CHAR; i++)
    {
        for(int j = 0; j < WIDTH_CHAR; j++)
        {
            matrice[j][i].brightness = 0;
            matrice[j][i].pixels = 0;
        }
    } 

    // Compute brightness
    for(int i = 0; i < imageHeight; i++)
    {
        for(int j = 0; j < imageWidth; j++)
        {
            matrice[(int)(j / widthCharToPixel)][(int)(i / heightCharToPixel)].brightness += data[i * imageWidth + j];
            matrice[(int)(j / widthCharToPixel)][(int)(i / heightCharToPixel)].pixels++;
        }
    } 

    // Compute average brightness
    for(int i = 0; i < HEIGHT_CHAR; i++)
    {
        for(int j = 0; j < WIDTH_CHAR; j++)
        {
            result[j][i] = bChar[(int)((matrice[j][i].brightness / matrice[j][i].pixels) * 9 / 255)];
        }
    } 

    printCharMatrice(WIDTH_CHAR, HEIGHT_CHAR, result);

    stbi_image_free(data);
    return 0;
}
