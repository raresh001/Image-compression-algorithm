/*TICUS Rares Andrei - Grupa 313CD*/

#ifndef _PPM_MATRIX_H
#define _PPM_MATRIX_H

#include <stdio.h>

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Pixel_info;

typedef struct {
    Pixel_info *info;
    unsigned int dim; //  we only use square matrices
} PPM_Matrix;

PPM_Matrix *readMatrix(const char *Input_FileName);
int isAriaUniform(PPM_Matrix *ppm_matrix
                , unsigned int start_point_line
                , unsigned int end_point_line
                , unsigned int start_point_column
                , unsigned int end_point_column
                , unsigned int factor
                , Pixel_info *p_pixel);
void write_PPM_file(char *Output_FileName, PPM_Matrix *ppm_matrix);
void freeMatrix(PPM_Matrix **p_ppm_matrix);

#endif //  _PPM_MATRIX_H
