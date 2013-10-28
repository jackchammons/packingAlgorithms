//
//  packing.h
//  ece368pa3
//
//  Created by Jack Hammons on 10/22/13.
//  Copyright (c) 2013 Jack Hammons. All rights reserved.
//

#ifndef ece368pa3_packing_h
#define ece368pa3_packing_h


#include <stdio.h>


typedef struct node {
    long thisnode;
    double width;
    double height;
    int parnode;
    int lcnode;
    int rcnode;
    double xcoord;
    double ycoord;
    char cutline;
    struct node *leftnode;
    struct node *rightnode;
} Node;

Node *Load_File(char *Filename, int* numBlocks, int* numNodes);
void addNodes(Node *head, Node * * nodePointers);
int sumWidth(Node * head);
int sumHeight(Node * head);
int Save_File(char *Filename, Node * head, int numberOfBlocks);
void printerHelper(Node *head, Node **nodePointers);
int determineCoordinates(Node *head, double x, double y);






#endif





