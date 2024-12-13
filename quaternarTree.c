/*TICUS Rares Andrei - Grupa 313CD*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

static QuaternarTreeNode *initialiseNode(unsigned char node_type
                                        , Pixel_info pixel) {
    QuaternarTreeNode *node = calloc(1, sizeof(QuaternarTreeNode));
    if (node) {
        node->node_type = node_type;
        node->pixel = pixel;
    }

    return node;
}
//  initialises a QuaternarTreeNode

static QuaternarTreeNode *createTreeNodeFromMatrix(PPM_Matrix *ppm_matrix
                                            , unsigned int start_point_line
                                            , unsigned int end_point_line
                                            , unsigned int start_point_column
                                            , unsigned int end_point_column
                                            , unsigned int factor
                                            , unsigned int cur_level
                                            , unsigned int *p_noLevels
                                            , unsigned int *p_noBlocks
                                            , unsigned int *p_maxSizeBlock) {

    if (*p_noLevels < cur_level) {
        *p_noLevels = cur_level;
    }

    Pixel_info pixel = {0, 0, 0};
    int ok = isAriaUniform(ppm_matrix
                    , start_point_line
                    , end_point_line
                    , start_point_column
                    , end_point_column
                    , factor
                    , &pixel);

    QuaternarTreeNode *node = initialiseNode(1, pixel);
    if (ok == 0) {
        node->node_type = 0;
        unsigned int middle_line = (start_point_line + end_point_line) / 2;
        unsigned int middle_column = 
                            (start_point_column + end_point_column) / 2;

        node->kids[0] = createTreeNodeFromMatrix(ppm_matrix
                                                , start_point_line
                                                , middle_line
                                                , start_point_column
                                                , middle_column
                                                , factor
                                                , cur_level + 1
                                                , p_noLevels
                                                , p_noBlocks
                                                , p_maxSizeBlock);

        node->kids[1] = createTreeNodeFromMatrix(ppm_matrix
                                                , start_point_line
                                                , middle_line
                                                , 1 + middle_column
                                                , end_point_column
                                                , factor
                                                , cur_level + 1
                                                , p_noLevels
                                                , p_noBlocks
                                                , p_maxSizeBlock);

        node->kids[2] = createTreeNodeFromMatrix(ppm_matrix
                                                , 1 + middle_line
                                                , end_point_line
                                                , 1 + middle_column
                                                , end_point_column
                                                , factor
                                                , cur_level + 1
                                                , p_noLevels
                                                , p_noBlocks
                                                , p_maxSizeBlock);

        node->kids[3] = createTreeNodeFromMatrix(ppm_matrix
                                                , 1 + middle_line
                                                , end_point_line
                                                , start_point_column
                                                , middle_column
                                                , factor
                                                , cur_level + 1
                                                , p_noLevels
                                                , p_noBlocks
                                                , p_maxSizeBlock);

    } else {
        (*p_noBlocks)++;
        if (end_point_line - start_point_line + 1 > *p_maxSizeBlock) {
            *p_maxSizeBlock = end_point_line - start_point_line + 1;
        }
    }

    return node;
}
//  this function is a helper for createTreeFromMatrix

QuaternarTree *createTreeFromMatrix(PPM_Matrix *ppm_matrix
                                    , unsigned int factor) {

    QuaternarTree *tree = malloc(sizeof(QuaternarTree));
    if (tree == NULL) {
        fprintf(stderr, "Cannot create tree\n");
        exit(1);
    }

    tree->dim_image = ppm_matrix->dim;
    tree->maxSizeBlock = 0;
    tree->noLevels = 0;
    tree->noBlocks = 0;
    tree->root = createTreeNodeFromMatrix(ppm_matrix
                                    , 0
                                    , ppm_matrix->dim - 1
                                    , 0
                                    , ppm_matrix->dim - 1
                                    , factor
                                    , 1
                                    , &(tree->noLevels)
                                    , &(tree->noBlocks)
                                    , &(tree->maxSizeBlock));
    //  It is considered that level of root is 1

    return tree;
}

void printTreeInfo_C1(QuaternarTree *tree, char *Output_FileName) {
    FILE *fout = fopen(Output_FileName, "w");
    if (fout == NULL) {
        fprintf(stderr, "Cannot open file %s\n", Output_FileName);
        exit(1);
    }

    fprintf(fout
            , "%u\n%u\n%u\n"
            , tree->noLevels
            , tree->noBlocks
            , tree->maxSizeBlock);

    fclose(fout);
}
//  prints data in the format requested at task 1

void printTreeInfo_C2(QuaternarTree *tree, char *Output_FileName) {
    FILE *fout = fopen(Output_FileName, "wb");
    if (fout == NULL) {
        fprintf(stderr, "Cannot open file %s\n", Output_FileName);
        exit(1);
    }

    fwrite(&(tree->dim_image), sizeof(unsigned int), 1, fout);
    Queue *queue = initialiseQueue();
    enqueue(queue, tree->root);

    while (queue->head) {
        QuaternarTreeNode *node = dequeue(queue);

        fwrite(&(node->node_type), sizeof(unsigned char), 1, fout);
        if (node->node_type) {
            fwrite(&(node->pixel), sizeof(Pixel_info), 1, fout);
        } else {
            for (unsigned int i = 0; i < 4; i++) {
                enqueue(queue, node->kids[i]);
            }
        }
    }

    queue = destroyQueue(queue);

    fclose(fout);
}
//  prints data in the format requested at task 2

static QuaternarTreeNode *readTreeNode(FILE *fin) {
    Pixel_info pixel = {0, 0, 0};
    QuaternarTreeNode *node = initialiseNode(0, pixel);

    fread(&(node->node_type), sizeof(unsigned char), 1, fin);
    if (node->node_type) {
        fread(&(node->pixel), sizeof(Pixel_info), 1, fin);
    }

    return node;
}

QuaternarTreeNode *readTree(char *Input_FileName, unsigned int *p_dim) {
    FILE *fin = fopen(Input_FileName, "rb");
    if (fin == NULL) {
        fprintf(stderr, "Invalid binary file\n");
        exit(1);
    } //  end of program if the binary file cannot be opened

    fread(p_dim, sizeof(unsigned int), 1, fin);
    Queue *queue = initialiseQueue();

    QuaternarTreeNode *root = readTreeNode(fin);
    if (root->node_type == 0) {
        enqueue(queue, root);
    }

    while (queue->head) {
        QuaternarTreeNode *node = dequeue(queue);
        for (unsigned int i = 0; i < 4; i++) {
            QuaternarTreeNode *iter = readTreeNode(fin);
            if (iter->node_type == 0) {
                enqueue(queue, iter);
            }
            node->kids[i] = iter;
        }
    }

    queue = destroyQueue(queue);
    fclose(fin);
    return root;
}
//  completes a QuaternarTree (without metadata), reading from
//  the file given as argument

static void completeMatrixFromTree(QuaternarTreeNode *root
                                    , PPM_Matrix *ppm_matrix
                                    , unsigned int cur_level
                                    , unsigned int start_point_line
                                    , unsigned int start_point_column) {

    if (root->node_type) {
        unsigned int dim = ppm_matrix->dim;
        Pixel_info *matrix = ppm_matrix->info;
        //  used this to avoid dereferencing ppm_matrix each time

        dim /= (1 << cur_level);
        unsigned int end_point_line = start_point_line + dim - 1;
        unsigned int end_point_column = start_point_column + dim - 1;
        for (unsigned int i = start_point_line; i <= end_point_line; i++) {
            for (unsigned int j = start_point_column
                    ; j <= end_point_column
                    ; j++) {
                
                matrix[i * ppm_matrix->dim + j] = root->pixel;
            }
        }
    } else {
        unsigned int dim = ppm_matrix->dim / (1 << cur_level);
        unsigned int middle_line = start_point_line + dim / 2;
        unsigned int middle_column = start_point_column + dim / 2;

        completeMatrixFromTree(root->kids[0]
                                , ppm_matrix
                                , cur_level + 1
                                , start_point_line
                                , start_point_column);

        completeMatrixFromTree(root->kids[1]
                                , ppm_matrix
                                , cur_level + 1
                                , start_point_line
                                , middle_column);

        completeMatrixFromTree(root->kids[2]
                                , ppm_matrix
                                , cur_level + 1
                                , middle_line
                                , middle_column);

        completeMatrixFromTree(root->kids[3]
                                , ppm_matrix
                                , cur_level + 1
                                , middle_line
                                , start_point_column);

    }
}
//  completes a PPM matrix from the QuaternarTree
//  this function is a helper for createMatrixFromTree

PPM_Matrix *createMatrixFromTree(QuaternarTreeNode *root, unsigned int dim) {
    PPM_Matrix *ppm_matrix = malloc(sizeof(PPM_Matrix));
    if (ppm_matrix == NULL) {
        fprintf(stderr, "Cannot create the matrix\n");
        exit(1);
    }

    ppm_matrix->dim = dim;
    ppm_matrix->info = malloc(dim * dim * sizeof(Pixel_info));
    completeMatrixFromTree(root, ppm_matrix, 0, 0, 0);
    return ppm_matrix;
}
//  creates a matrix with dimension dim and completes it using
//  completeMatrixFromTree function

QuaternarTreeNode *destroyTreeNode(QuaternarTreeNode *root) {
    if (root) {
        if (root->node_type == 0) {
            for (unsigned int i = 0; i < 4; i++) {
                root->kids[i] = destroyTreeNode(root->kids[i]);
            }
        }

        free(root);
    }

    return NULL;
}
//  frees memory allocated for the tree (without metadata)

QuaternarTree *destroyTree(QuaternarTree *tree) {
    tree->root = destroyTreeNode(tree->root);
    free(tree);
    return NULL;
}
//  frees memory allocated for the tree (including metadata)
