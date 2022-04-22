/*
 * imgUtil.h
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#ifndef IMGUTIL_H_
#define IMGUTIL_H_

#include "main.h"

#define RES 2835

typedef uchar color_t[3];
typedef color_t** image_t;
typedef struct point_t
{
    int x; 
    int y;
    int z;
    uchar blue;
    uchar green;
    uchar red;
} point_t;
typedef struct camera_t
{
    vector_t origine;
    vector_t direction;
    float distance;
} camera_t;
typedef struct figure_t
{
    uint nombre_Point; 
    point_t* sequence;
} figure_t;

/* Elementaires */
image_t initImage(int width, int height);
void setImage(image_t image, int width, int height);
void dispImage(image_t image, int width, int height);
void freeImage(image_t image, int width, int height);

void getBlock8(const image_t src, unsigned int x, unsigned int y, image_t block);
void setBlock8(image_t dst, unsigned int x, unsigned int y, const image_t block);
void displayBlock8(image_t bloc);

#endif /* IMGUTIL_H_ */
