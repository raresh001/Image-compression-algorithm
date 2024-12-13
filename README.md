Proiectul contine urmatoarele fisiere sursa:
  1. main.c - contine implementarea functiei main, care analizeaza argumentele
    primite in linia de comanda (comenzile -c1 / -c2 / -d, fisierele de
    input / output, respectiv factorul de compresie) si indeplineste taskurile
    conform cerintelor.

  2. quaternarTree - contine implementarea arborelui cuaternar; are urmatoarele
    functii:
    - static QuaternarTreeNode *initialiseNode(unsigned char node_type
                                        , Pixel_info pixel)
        Initializeaza un nod al arborelui; este declarata statica, intrucat
        doar functiile din acest fisier o folosesc

    - static QuaternarTreeNode *createTreeNodeFromMatrix(PPM_Matrix *ppm_matrix
                                            , unsigned int start_point_line
                                            , unsigned int end_point_line
                                            , unsigned int start_point_column
                                            , unsigned int end_point_column
                                            , unsigned int factor
                                            , unsigned int cur_level
                                            , unsigned int *p_noLevels
                                            , unsigned int *p_noBlocks
                                            , unsigned int *p_maxSizeBlock)

        Creaza un nod al arborelui cuaternar pe baza matricei ppm_matrix
        Functia este una recursiva: la un pas se doreste verificarea unui
        patrat din cadrul matricei, patrat care este marginit de cele 4 drepte
        date ca parametru
        La un pas recursiv, se verifica daca zona de memorie aleasa este
        uniforma (in limitele factorului impus), caz in care se completeaza
        nodul cu informatia medie si se marcheaza ca fiind o frunza. In caz
        contrar, se imparte patratul in 4 patratele egale, nodul este marcat
        ca fiind nod intern, iar toti copii sai sunt completati prin apelul
        functiei cu cele 4 patratele.
        Functia completeaza concomitent si metadate despre arborele cuaternar
        (numarul de nivele, numarul de blocuri si dimensiunea maxima a unui
        bloc), informatie ce este trecuta la adresele date ca parametrii finali
        Intrucat functia aceasta va fi folosita doar in cadrul implementarii
        functiei createTreeFromMatrix, ea este declarata ca statica

    - QuaternarTree *createTreeFromMatrix(PPM_Matrix *ppm_matrix
                                    , unsigned int factor)
        Creeaza un element de tip QuaternarTree pe baza matricei ppm_matrix
        QuaternarTree contine atat nodul root al arborelui cuaternar, cat si
        metadate despre arbore (numarul de nivele, dimensiunea maxima a unui
        bloc, respectiv cate blocuri comprimate exista in arbore)

    - void printTreeInfo_C1(QuaternarTree *tree, char *Output_FileName)
        Completeaza fisierul de output conform cerintei 1

    - void printTreeInfo_C2(QuaternarTree *tree, char *Output_FileName)
        Completeaza fisierul de output conform cerintei 2; intrucat afisarea
        presupune o parcurgere pe nivel, implementarea acestei functii foloseste
        o coada

    - static QuaternarTreeNode *readTreeNode(FILE *fin)
        Citeste informatie despre un nod al arborelui de la tastatura;
        intrucat aceasta functie va fi utilizata doar in impelementarea
        functiei readTree, ea a fost declarata ca statica

    - QuaternarTreeNode *readTree(char *Input_FileName, unsigned int *p_dim)
        Citeste informatie despre un arbore cuaternar (fara metadate) si
        returneaza arborele citit. Intrucat fisierul de input contine informatii
        despre arbore pe nivele, este necesara utilizarea unei cozi in
        implementare. Aceasta coada va contine doar nodurile interne, iar
        pentru fiecare element din ea i se citesc fii, care, daca sunt la
        randul lor noduri interne, sunt adaugati la finalul cozii.

    - static void completeMatrixFromTree(QuaternarTreeNode *root
                                    , PPM_Matrix *ppm_matrix
                                    , unsigned int cur_level
                                    , unsigned int start_point_line
                                    , unsigned int start_point_column)

        Completeaza matricea ppm_matrix in functie de informatia din arborele
        cuaternar. Este o functie recursiva, care analizeaza pentru fiecare nod
        al arborelui daca este sau nu nod frunza: daca nodul este terminal,
        atunci se completeaza patratul din matrice corespunzator lui
        (functia primeste ca parametrii nivelul curent al nodului, deci
        dimensiunea patratului, si coltul din stanga sus al patratului). Daca
        nodul nu este terminal, atunci functia se apeleaza recusiv pentru toti
        fiii acelui nod, crescand la fiecare nivelul.
        Intrucat aceasta functie este apelata doar in cadrul functiei
        createMatrixFromTree, ea este declarata ca statica

    - PPM_Matrix *createMatrixFromTree(QuaternarTreeNode *root, unsigned int dim)
        Creeaza o matrice ppm_matrix de dimensiune dim, pe care o completeaza,
        folosind functia completeMatrixFromTree

    - QuaternarTreeNode *destroyTreeNode(QuaternarTreeNode *root)
        Elibereaza spatiul pentru un arbore cuaternar, fara metadate

    - QuaternarTree *destroyTree(QuaternarTree *tree)
        Elibereaza spatiul pentru un arbore cuaternar, cu metadate

  3. matrix.c - contine implemtentarea functiilor ce tin de matricea PPM. Are
        urmatoarele functii:
    - PPM_Matrix *readMatrix(const char *Input_FileName)
        Creeaza o matrice de tip PPM_Matrix, pe care o completeaza pe baza
        fisierului de tip PPM primit ca argument.

    - int isAriaUniform(PPM_Matrix *ppm_matrix
                , unsigned int start_point_line
                , unsigned int end_point_line
                , unsigned int start_point_column
                , unsigned int end_point_column
                , unsigned int factor
                , Pixel_info *p_pixel)

        Verifica daca patratul din matricea ppm_matrix cuprins intre cele 4
        drepte date ca argument este uniform (in limitele impuse de factor);
        in caz afirmativ, returneaza 1 si completeaza la adresa p_pixel valorile
        medii calculate; returneaza 0 in caz contrar

    - static unsigned int convertU_intToString(unsigned int number, char *no_string)
        Converteste un numar la un string (implementata din cauza lipsei
        functiei itoa); este declarata ca static, pentru ca este folosita doar
        de functia write_PPM_file

    - void write_PPM_file(char *Output_FileName, PPM_Matrix *ppm_matrix)
        Completeaza fisierul de output dat ca parametru in format PPM, pe baza
        matricei ppm_matrix

    - void freeMatrix(PPM_Matrix **p_ppm_matrix)
        Elibereaza spatiul alocat pentru elementul de la adresa p_ppm_matrix
        si seteaza variabila ppm_matrix cu NULL

  4. queue.c - contine implementarea unei cozi (ce contine nodurile arborului
    cuaternar); are functiile specifice unei cozi - initialiseQueueNode, 
    initialiseQueue, enqueue, dequeue, destroyQueue
