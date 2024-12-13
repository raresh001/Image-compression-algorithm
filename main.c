/*TICUS Rares Andrei - Grupa 313CD*/

#include "queue.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Not enough arguments\n");
        exit(1);
    }    

    if (strcmp(argv[1], "-c1") == 0) {
        if (argc != 5) {
            fprintf(stderr, "Invalid argument format\n");
            exit(1);
        }

        unsigned int factor = atoi(argv[2]);
        PPM_Matrix *ppm_matrix = readMatrix(argv[3]);

        QuaternarTree *tree = createTreeFromMatrix(ppm_matrix, factor);
        printTreeInfo_C1(tree, argv[4]);
        freeMatrix(&ppm_matrix);
        tree = destroyTree(tree);
    } else if (strcmp(argv[1], "-c2") == 0) {
        if (argc != 5) {
            fprintf(stderr, "Invalid argument format\n");
            exit(1);
        }

        unsigned int factor = atoi(argv[2]);
        PPM_Matrix *ppm_matrix = readMatrix(argv[3]);

        QuaternarTree *tree = createTreeFromMatrix(ppm_matrix, factor);
        printTreeInfo_C2(tree, argv[4]);
        freeMatrix(&ppm_matrix);
        tree = destroyTree(tree);
    } else if (strcmp(argv[1], "-d") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Invalid argument format\n");
            exit(1);
        }

        unsigned int dim = 0;
        QuaternarTreeNode *root = readTree(argv[2], &dim);

        PPM_Matrix *ppm_matrix = createMatrixFromTree(root, dim);
        write_PPM_file(argv[3], ppm_matrix);

        freeMatrix(&ppm_matrix);
        root = destroyTreeNode(root);
    } else {
        fprintf(stderr, "%s is not valid\n", argv[1]);
    }

    return 0;
}
