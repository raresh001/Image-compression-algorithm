/*TICUS Rares Andrei - Grupa 313CD*/

#ifndef _QUATERNAR_TREE_H
#define _QUATERNAR_TREE_H

#include "matrix.h"

typedef struct quaternarTreeNode {
    struct quaternarTreeNode *kids[4];
    Pixel_info pixel;
    unsigned char node_type;
} QuaternarTreeNode;

typedef struct {
    QuaternarTreeNode *root;
    unsigned int dim_image;
    unsigned int noLevels;
    unsigned int noBlocks;
    unsigned int maxSizeBlock;
} QuaternarTree;
//  QuaternarTree contains the root of the tree, as well as metadata about it

QuaternarTree *createTreeFromMatrix(PPM_Matrix *ppm_matrix
                                    , unsigned int factor);
void printTreeInfo_C1(QuaternarTree *tree, char *Output_FileName);
void printTreeInfo_C2(QuaternarTree *tree, char *Output_FileName);
QuaternarTreeNode *readTree(char *Input_FileName, unsigned int *p_dim);
PPM_Matrix *createMatrixFromTree(QuaternarTreeNode *root, unsigned int dim);
QuaternarTreeNode *destroyTreeNode(QuaternarTreeNode *root);
QuaternarTree *destroyTree(QuaternarTree *tree);

#endif //  _QUATERNAR_TREE_H
