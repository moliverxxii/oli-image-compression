/*
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"


int main(int argc, char* argv[])
{
    //Paramètres de base.
    if(argc == 1)
    {
        printf("Syntaxe : \n\t%s <nom de l'image sans extension>\n",argv[0]);
        return EXIT_FAILURE;
    }
    char* name_image = bmpExtension(argv[1]);
    char* name_compr = chapronExtension(argv[1]);
    char name_output[40];
    strcpy(name_output,argv[1]);
    strcat(name_output," ouput");
    int width = 8;
    int height = 8;
    FILE* image_f = fopen(name_image,"rb");
    FILE* compr_f = fopen(name_compr,"wb+");
    zigzag_t bloc_zig;
    //Initialisation de l'image.
    int width_img = getWidth(image_f);
    int height_img = getHeight(image_f);
    printf("[%d, %d]\n",width_img,height_img);
    
    image_t block = initImage(8,8);
    image_t block1 = initImage(8,8);
    image_t image0 = fileToImage(image_f); 
    int x_bl, y_bl, i, j;
    int plus, x, y;
    int x_min, x_max;
    char delta = 1;
    int counter;
    //Encodage de l'image.
    for(x_bl=0; x_bl<width_img; x_bl+=8)
    { 
        for(y_bl=0; y_bl<height_img; y_bl+=8)
        {
            getBlock8(image0, x_bl, y_bl, block);    
            processDataUnit(block,block1);
            zigZag(block1, &bloc_zig);
            writeZigZag(&bloc_zig, compr_f);
            deProcessDataUnit(block1,block);
            setBlock8(image0, x_bl, y_bl, block);
            
            //Affichage d'un bloc DCT quantifie arbitraire.
            if(y_bl==x_bl && y_bl%10==0 && x_bl%10==0)
            {
                displayBlock8(block1);
            }

        }
    }
    fclose(compr_f);
    
    compr_f = fopen(name_compr,"rb");
    //decodage de l'image.
    for(x_bl=0; x_bl<width_img; x_bl+=8)
    { 
        for(y_bl=0; y_bl<height_img; y_bl+=8)
        {
            readZigZag(&bloc_zig, compr_f);
            deZigZag(block1, &bloc_zig);
            deProcessDataUnit(block1,block);
            setBlock8(image0, x_bl, y_bl, block);
            
            //Affichage d'un bloc arbitraire qui a ete decode.
            if(y_bl==x_bl && y_bl%10==0 && x_bl%10==0)
            {
                displayBlock8(block1);
            }
        }
    }
    


    FILE* output = initImageFile(
            name_output,
            getWidth(image_f),
            getHeight(image_f),
            image0);

    


    return EXIT_SUCCESS;
}
