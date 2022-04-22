/*
 * imgUtil.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

image_t initImage(int width, int height)
{
    //Returns a pointer to an image.

    int x;
    int y;
    image_t image = (image_t) malloc(width*sizeof(color_t*));

    for(x=0; x<width; ++x)
    {
        image[x] = (color_t*) malloc(height*
                sizeof(color_t));
        if(image[x] == NULL) exit(1);
    }
    printf("image à : %p\n",image);
    return image;
}



void setImage(image_t image, int width, int height)
{
    //Turns the image black.
    int x;
    int y;
    int color;

    for(x=0; x<width; ++x)
    {
        for(y=0; y<height;++y)
        {
            for(color=0; color<3; ++color)
            {
                image[x][y][color] = 0x0;
            }
        }
    }
}

void dispImage(image_t image, int width, int height)
{
    //Displays the values of the pixels. Line by line.
    int x;
    int y;
    for(x = 0; x<width;++x)
    {
        for(y = 0;y<height;++y)
        {
            printf("Point[%d,%d] = (%x,%x,%x)\n",
                    x+1,y+1,
                    image[x][y][0],
                    image[x][y][1],
                    image[x][y][2]);
        }
    }
}


void freeImage(image_t image, int width, int height)
{
    //Frees the memory the image occupies in memory.
    int x;
    int y;
    for(x=0;x<width;++x)
    {
        for(y=0;y<height;++y)
        {
            free(image[x][y]);
        }
        free(image[x]);
    }
    free(image);
}

/**
 * assigne à @param block l'image_t 8x8 de src commençant à (@param x,@param y)
 *
 */
void getBlock8(image_t src, unsigned int x, unsigned int y, image_t block)
{
    uchar x_b,y_b,col;
    for(x_b=0; x_b<8; ++x_b)
    {
        for(y_b=0; y_b<8; ++y_b)
        {
            for(col=0; col<3; ++col)
            {
                block[x_b][y_b][col] = src[x+x_b][y+y_b][col];
            }
        }
    }
}

/**
* assigne @param block à l'image_t 8x8 de @param dst commençant à (@param x,@param y)
*
*/
void setBlock8(image_t dst, unsigned int x, unsigned int y, const image_t block)
{
    uchar x_b,y_b,col;
    for(x_b=0; x_b<8; ++x_b)
    {
        for(y_b=0; y_b<8; ++y_b)
        {
            for(col=0; col<3; ++col)
            {
                dst[x+x_b][y+y_b][col] = block[x_b][y_b][col];
            }
        }
    }
}

void displayBlock8(image_t bloc)
{
    int i,j;
    for(j=7; j>=0; --j)
    {
        printf("| ");
        for(i=0; i<8; ++i)
        {
            printf("%4hhd | ",bloc[i][j][0]);
        }
        printf("\n");
    }
    printf("\n");
}

