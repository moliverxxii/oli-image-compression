in order to create the program you need gcc installed as well as make.
open your shell.
once in the directory build with "make all".
then run with "./compressor <name of source bmp image>".
24bit per pixel ".bmp" files are required for the program to work
if wrong image requested : "Segmentation fault" will display

the compressed file is written in a ".chapron" file.

example :
to open "lena.bmp" :
"./compressor lena"
