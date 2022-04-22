/*
 * filter.c
 *
 *  Created on: Jan 15, 2020
 *      Author: moliver
 */

#include "main.h"

float quant[8][8] = 
{
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 57, 68,109,103, 77},
    {24, 35, 55, 64, 81,104,113, 92},
    {49, 64, 78, 87,103,121,120,101},
    {72, 92, 95, 98,112,100,103, 99}
};
/**
 * renvoie la 
 */
void processDataUnit(const image_t src_block, image_t dst_block)
{
    float block1[8][8][3] = {.0f};
    float block2[8][8][3] = {.0f};
    uchar  x_1, y_1,col_1;
    uchar x_2, y_2;
    /* Centrage */    
    for(x_1=0;x_1<8;++x_1)
    {
        for(y_1=0; y_1<8;++y_1 )
        {
            for(col_1=0; col_1<3; ++col_1)
            {
                block1[x_1][y_1][col_1] =
                    (float) src_block[x_1][y_1][col_1] -128.0;
            }
        }
    }
    /* DCT */
    for(x_2=0; x_2<8; ++x_2)
    {
        for(y_2=0; y_2<8; ++y_2)
        {
            for(col_1=0; col_1<3; ++col_1)
            {
                block2[x_2][y_2][col_1] =
                    pietroisDCT(block1, x_2, y_2, col_1);
                /* Quantificaton */
                block2[x_2][y_2][col_1] /= quant[y_2][x_2];
            }
        }
    }
    /* Arrondi */
    for(x_2=0;x_2<8;++x_2)
    {
        for(y_2=0; y_2<8;++y_2 )
        {
            for(col_1=0; col_1<3; ++col_1)
            {
                dst_block[x_2][y_2][col_1] = 
                (char) (block2[x_2][y_2][col_1] + .5); 
            }
        }
    }
}

float pietroisDCT(float block[8][8][3],uchar x, uchar y, uchar col)
{
    double dct = 0;
    double temp;
    uchar x_1, y_1;
    for(x_1=0; x_1<8; ++x_1)
    {
        for(y_1=0; y_1<8; ++y_1)
        {
            temp = block[x_1][y_1][col];
            temp *= cos(PI*(double) x * (double) (2* x_1 + 1) /16.0);
            temp *= cos(PI*(double) y * (double) (2* y_1 + 1) /16.0);
            dct += temp; 
        }
    }
    dct *= (x==0)?0.3535533906:0.5;
    dct *= (y==0)?0.3535533906:0.5;

    return (float) (dct);
}

float invDCT(float block[8][8][3],uchar x, uchar y, uchar col)
{
    double dct = 0;
    double temp;
    uchar x_1, y_1;
    for(x_1=0; x_1<8; ++x_1)
    {
        for(y_1=0; y_1<8; ++y_1)
        {
            temp = block[x_1][y_1][col];
            temp *= cos(PI * (double) x_1 * (double) (2*x + 1) /16.0);
            temp *= cos(PI * (double) y_1 * (double) (2*y + 1) /16.0);
            temp *= (x_1==0)?0.3535533906:0.5;
            temp *= (y_1==0)?0.3535533906:0.5;
            dct += temp; 
        }
    }

    return (float) (dct);
}

void deProcessDataUnit(const image_t src_block, image_t dst_block)
{
   float block[8][8][3] = {0};
   float block2[8][8][3] = {0};
   float temp;
   uchar x,y,col;
   /* Dequantification */
   for(x=0;x<8;++x)
   {
       for(y=0;y<8;++y)
       {
           for(col=0;col<3;++col)
           {
               block[x][y][col] = 
                   quant[y][x] * (char) src_block[x][y][col];
           }
       }
   }

   /* IDCT */
   for(x=0;x<8;++x)
   {
       for(y=0;y<8;++y)
       {
           for(col=0;col<3;++col)
           {
               block2[x][y][col] = invDCT(block, x, y, col);
           }
       }
   }

   /* Decentrage */
   for(x=0;x<8;++x)
   {
       for(y=0;y<8;++y)
       {
           for(col=0;col<3;++col)
           {
               temp = block2[x][y][col];
               temp += 128;
               dst_block[x][y][col] =(temp>=0.0f)?((temp<=255.0f)?temp:255):0;
           }
       }
   }

}

void zigZag(image_t zig, zigzag_t* zag)
{
    uchar col;
    char delta;
    int counter;
    int plus;
    int x_max,x_min,x,y;
    for(col=0;col<3;++col)
    {
        for(counter=0, delta=1,plus=0; plus<15; ++plus, delta *= -1)
        {
            x_max = (plus<8)?plus:7;
            x_min = (plus>7)?plus-7:0;

            for(x=(delta== 1)?x_min:x_max, y=plus-x;
                    x>=x_min && x<=x_max ;
                    x += delta, y -= delta)
            {
                zag->zigzag[counter++][col] = zig[x][y][col];
            }
        }
    }
    counter = 63;
    char b_is_tail = zag->zigzag[counter][0] == 0
            && zag->zigzag[counter][1] == 0
            && zag->zigzag[counter][2] == 0;

    while(b_is_tail && counter>=0)
    {
        counter--;
        b_is_tail = zag->zigzag[counter][0] == 0
            && zag->zigzag[counter][1] == 0
            && zag->zigzag[counter][2] == 0;
    }
    zag->length = 1 + counter;

}

void deZigZag(image_t zig, zigzag_t* zag)
{
    uchar col;
    char delta;
    int counter;
    int plus;
    int x_max,x_min,x,y;
    for(col=0;col<3;++col)
    {
        for(counter=0, delta=1,plus=0; plus<15; ++plus, delta *= -1)
        {
            x_max = (plus<8)?plus:7;
            x_min = (plus>7)?plus-7:0;

            for(x=(delta== 1)?x_min:x_max, y=plus-x;
                    x>=x_min && x<=x_max ;
                    x += delta, y -= delta)
            {
                zig[x][y][col] = zag->zigzag[counter++][col];
            }
        }
    }

}

void writeZigZag(zigzag_t* zigzag, FILE* file)
{
    int index;
    fwrite(&(zigzag->length), sizeof(unsigned int), 1, file);
    fwrite(zigzag->zigzag, sizeof(zigzag->zigzag[0]), zigzag->length, file);
}

void readZigZag(zigzag_t* zigzag, FILE* file)
{
    fread(&(zigzag->length),sizeof(unsigned int), 1, file);
    int index;
    for(index=0; index<zigzag->length;++index)
    {
        fread(zigzag->zigzag[index],sizeof(uchar[3]), 1, file);
    }
    
    for(index=zigzag->length; index<64;++index)
    {
        int col;
        for(col=0; col<3; ++col)
        {
            zigzag->zigzag[index][col] = 0;
        }
    }
}

