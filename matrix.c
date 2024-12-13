/*TICUS Rares Andrei - Grupa 313CD*/

#include <stdlib.h>
#include <string.h>
#include "matrix.h"

PPM_Matrix *readMatrix(const char *Input_FileName) {
    FILE *fin = fopen(Input_FileName, "rb");
    if (fin == NULL) {
        fprintf(stderr, "Invalid binary file %s\n", Input_FileName);
        exit(1);
    } //  end of program if the binary file cannot be opened

    PPM_Matrix *ppm_matrix = malloc(sizeof(PPM_Matrix));
    if (ppm_matrix == NULL) {
        fprintf(stderr, "Failure at allocating the matrix\n");
        exit(1);
    } //  end of program if the allocation fails

    char line[32];

    //  reads P6 (first line)
    fgets(line, sizeof(line), fin); //  ignore the information

    //  reads matrix dimension
    if (fgets(line, sizeof(line), fin) == NULL) {
        fprintf(stderr, "Invalid dimension\n");
        exit(1);
    }

    ppm_matrix->dim = (unsigned int)atoi(line);

    //  reads colour range (default 255)
    fgets(line, sizeof(line), fin); //  ignore the information

    unsigned int points_no = ppm_matrix->dim * ppm_matrix->dim;
    ppm_matrix->info = malloc(points_no * sizeof(Pixel_info));
    if (ppm_matrix->info == NULL) {
        fprintf(stderr, "Failure at allocating the matrix\n");
        exit(1);
    } //  end of program if the allocation fails

    if (fread(ppm_matrix->info, sizeof(Pixel_info), points_no, fin)
            != points_no) {
        fprintf(stderr, "Problem at reading binary file\n");
        exit(1);
    }

    fclose(fin);
    return ppm_matrix;
}
//  reads the matrix of pixels from the file given as argument

int isAriaUniform(PPM_Matrix *ppm_matrix
                , unsigned int start_point_line
                , unsigned int end_point_line
                , unsigned int start_point_column
                , unsigned int end_point_column
                , unsigned int factor
                , Pixel_info *p_pixel) {

    unsigned int dim = ppm_matrix->dim;
    Pixel_info *matrix = ppm_matrix->info;
    //  used this to avoid dereferencing ppm_matrix each time

    unsigned long long sum_red = 0, sum_green = 0, sum_blue = 0;
    //  hold sums of colours as unsigned long long to avoid overflowing char

    unsigned int i, j;
    for (i = start_point_line; i <= end_point_line; i++) {
        for (j = start_point_column; j <= end_point_column; j++) {
            sum_red += matrix[i * dim + j].red;
            sum_green += matrix[i * dim + j].green;
            sum_blue += matrix[i * dim + j].blue;
        }
    }

    unsigned int total_dim = (end_point_column - start_point_column + 1) *
                        (end_point_line - start_point_line + 1);
    unsigned char mean_red = sum_red / total_dim;
    unsigned char mean_green = sum_green / total_dim;
    unsigned char mean_blue = sum_blue / total_dim;

    unsigned long long mean = 0;
    for (i = start_point_line; i <= end_point_line; i++) {
        for (j = start_point_column; j <= end_point_column; j++) {
            int red_error = mean_red - matrix[i * dim + j].red;
            int green_error = mean_green - matrix[i * dim + j].green;
            int blue_error = mean_blue - matrix[i * dim + j].blue;

            mean += red_error * red_error;
            mean += green_error * green_error;
            mean += blue_error * blue_error; 
        }
    }

    mean /= (3 * total_dim);

    int ok = (mean <= factor);
    if (ok) {
        p_pixel->red = mean_red;
        p_pixel->green = mean_green;
        p_pixel->blue = mean_blue;
    }

    return ok;
}
/**************************************************************
 Analyzes the region bounded by the line no starting_point,
 line no end_point, column no starting_point and column no
 end_point; verifies if the error caused by approximating this region
 as uniform is greater than the factor. Function returns 1 if the error
 is smaller than the factor and 0 otherwise
 Function receives a pointer to a Pixel_info and, if
 the return value is 1, writes the mean value in the address pointed
 by it. Function does nothing to the address if the return value is 0
**************************************************************/

static unsigned int convertU_intToString(unsigned int number, char *no_string) {
    unsigned int i = 0, p = 1;
    while (p <= number) {
        p *= 10;
    }

    p /= 10;
    while (p) {
        no_string[i] = (number / p) % 10 + '0';
        i++;
        p /= 10;
    }

    no_string[i] = '\0';
    return i;
}
//  converts unsigned int to string

void write_PPM_file(char *Output_FileName, PPM_Matrix *ppm_matrix) {
    FILE *fout = fopen(Output_FileName, "wb");
    if (fout == NULL) {
        fprintf(stderr, "Cannot open file %s\n", Output_FileName);
        exit(1);
    }

    char buffer[10] = "P6\n";
    fwrite(buffer, sizeof(char), 3, fout);

    unsigned int len = convertU_intToString(ppm_matrix->dim, buffer);
    fwrite(buffer, sizeof(char), len, fout);
    
    char space_const = ' ';
    fwrite(&space_const, sizeof(char), 1, fout);

    fwrite(buffer, sizeof(char), len, fout);

    space_const = '\n';
    fwrite(&space_const, sizeof(char), 1, fout);

    strcpy(buffer, "255\n");
    fwrite(buffer, sizeof(char), 4, fout);

    fwrite(ppm_matrix->info
        , sizeof(Pixel_info)
        , ppm_matrix->dim * ppm_matrix->dim
        , fout);

    fclose(fout);
}
//  writes information about the QuaternarTree in the PPM format

void freeMatrix(PPM_Matrix **p_ppm_matrix) {
    free((*p_ppm_matrix)->info);
    free(*p_ppm_matrix);
    *p_ppm_matrix = NULL;
}
//  frees all memory allocated for matrix