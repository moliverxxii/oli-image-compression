/*
 * filter.h
 *
 *  Created on: Jan 15, 2020
 *      Author: moliver
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "main.h"
#define PI 3.14159265 

typedef struct
{
    unsigned int length; /* La longueur du zigzag */
    uchar zigzag[64][3];
} zigzag_t;


void processDataUnit(const image_t src_block, image_t dst_block);
void deProcessDataUnit(const image_t src_block, image_t dst_block);
float pietroisDCT(float block[8][8][3],uchar x, uchar y, uchar col);
float invDCT(float block[8][8][3],uchar x, uchar y, uchar col);
void zigZag(image_t zig, zigzag_t* zag);
void deZigZag(image_t zig, zigzag_t* zag);
void writeZigZag(zigzag_t* zigzag, FILE* file);
void readZigZag(zigzag_t* zigzag, FILE* file);
#endif /* FILTER_H_ */
