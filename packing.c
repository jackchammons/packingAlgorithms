//
//  main.c
//  ece368pa3
//
//  Created by Jack Hammons on 10/22/13.
//  Copyright (c) 2013 Jack Hammons. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "packing.h"


int main(int argc, const char * argv[])
{
    char * inputFile = (char*)argv[1]; // input file   
    char * outputFile =(char*)argv[2]; // sorted output
    int numberOfBlocks;
    int numberOfNodes;
    Node * head = Load_File(inputFile, &numberOfBlocks, &numberOfNodes);
    Node * nodePointers[numberOfNodes];
    printerHelper(head, nodePointers);
    double timeOne = clock();
    sumWidth(head);
    sumHeight(head);
    determineCoordinates(head, 0, 0);
    double timeTwo = clock();
    printf("Width: %f\nHeight: %f\n\nX-coordinate: %f\nY-coordinate: %f\n\nElapsed time: %lf\n",head->width, head->height, nodePointers[numberOfBlocks-1]->xcoord, nodePointers[numberOfBlocks-1]->ycoord, (timeTwo-timeOne)/CLOCKS_PER_SEC);
    
    Save_File(outputFile, head, numberOfBlocks);
    
    int i;
    for(i = 0; i<numberOfNodes; i++){
        
        free(nodePointers[i]);
        
    }
    
    return 0;
}




Node *Load_File(char *Filename, int* numBlocks, int* numNodes){
    
    //create the file pointer
    FILE * fptr = fopen(Filename, "r");
    
    //check if the file pointer exists
    if(fptr == NULL){
        fclose(fptr);
        return NULL;
    }
    
    int numberOfBlocks;
    
    fscanf(fptr, "%d", &(numberOfBlocks));
    
    *numBlocks = numberOfBlocks;
    
    int numberOfNodes;
    
    fscanf(fptr, "%d", &(numberOfNodes));
    
    *numNodes = numberOfNodes;
    
    Node * nodePointers[numberOfNodes];
    
    long thisnode = 0;
    int parnode = 0;
    int lcnode = 0;
    int rcnode = 0;
    char cutline;
    double width = 0;
    double height = 0;
    char extraChar1, extraChar2;
    
    while(thisnode < numberOfBlocks){
        
        fscanf(fptr, "%ld %d %d %d %c %lf %lf", &(thisnode), &(parnode), &(lcnode), &(rcnode), &(cutline), &(width), &(height));
        
        nodePointers[(int)thisnode-1] = malloc(sizeof(Node));
        
        nodePointers[thisnode-1]->thisnode = thisnode;
        nodePointers[thisnode-1]->parnode = parnode;
        nodePointers[thisnode-1]->lcnode = lcnode;
        nodePointers[thisnode-1]->rcnode = rcnode;
        nodePointers[thisnode-1]->cutline = cutline;
        nodePointers[thisnode-1]->width = width;
        nodePointers[thisnode-1]->height = height;
        nodePointers[thisnode-1]->xcoord = 0;
        nodePointers[thisnode-1]->ycoord = 0;
    }
    
    while(thisnode < numberOfNodes){
        
        fscanf(fptr, "%ld %d %d %d %c %c %c", &(thisnode), &(parnode), &(lcnode), &(rcnode), &(cutline), &(extraChar1), &(extraChar2));
        
        nodePointers[(int)thisnode-1] = malloc(sizeof(Node));
        
        nodePointers[thisnode-1]->thisnode = thisnode;
        nodePointers[thisnode-1]->parnode = parnode;
        nodePointers[thisnode-1]->lcnode = lcnode;
        nodePointers[thisnode-1]->rcnode = rcnode;
        nodePointers[thisnode-1]->cutline = cutline;
        nodePointers[thisnode-1]->width = 0;
        nodePointers[thisnode-1]->height = 0;
        nodePointers[thisnode-1]->xcoord = 0;
        nodePointers[thisnode-1]->ycoord = 0;
    }
    
    fclose(fptr);
    
    Node * head = nodePointers[thisnode-1];
    
    addNodes(head, nodePointers);
    
    return head;
}


void addNodes(Node *head, Node * * nodePointers){
    
    if(head->lcnode == -1 && head->rcnode == -1){
        return ;
    }
    
    if(head->lcnode == -1){
        head->rightnode = nodePointers[head->rcnode -1];
        addNodes(head->rightnode, nodePointers);
    }
    
    if(head->rcnode == -1){
        head->leftnode = nodePointers[head->lcnode -1];
        addNodes(head->leftnode, nodePointers);
    }
    
    if(head->rcnode != -1 && head->lcnode != -1){
        head->leftnode = nodePointers[head->lcnode -1];
        head->rightnode = nodePointers[head->rcnode -1];
        addNodes(head->leftnode, nodePointers);
        addNodes(head->rightnode, nodePointers);
        
    }
}


int sumWidth(Node * head){

    if(head->cutline == '-'){
        return head->width;
    }
    
    if(head->cutline == 'V'){
         head->width = sumWidth(head->leftnode) + sumWidth(head->rightnode);
    }
    else{
        int temp1 = sumWidth(head->leftnode);
        int temp2 = sumWidth(head->rightnode);
        
        if(temp1 > temp2){
            head->width = temp1;
        }
        else{
            head->width = temp2;
        }
        
    }
    
    return head->width;
}



int sumHeight(Node * head){
    
    if(head->cutline == '-'){
        return head->height;
    }
    
    if(head->cutline == 'H'){
        head->height = sumHeight(head->leftnode) + sumHeight(head->rightnode);
    }
    else{
        int temp1 = sumHeight(head->leftnode);
        int temp2 = sumHeight(head->rightnode);
        
        if(temp1 > temp2){
            head->height = temp1;
        }
        else{
            head->height = temp2;
        }
        
    }
    
    return head->height;
}



int Save_File(char *Filename, Node * head, int numberOfBlocks){
    
    
    FILE *fptr = fopen(Filename,"w");
    
    fprintf(fptr, "%d\n", numberOfBlocks);
    
    Node * nodePointers[head->thisnode];
    
    printerHelper(head, nodePointers);
    
    
    int index = 0;
    
    while(index < numberOfBlocks){
        
        fprintf(fptr, "%ld %e %e %e %e\n", nodePointers[index]->thisnode, nodePointers[index]->width, nodePointers[index]->height, nodePointers[index]->xcoord, nodePointers[index]->ycoord);
        
        index++;
    }
    
    
    fclose(fptr);
    
    return 0;
}

void printerHelper(Node *head, Node * * nodePointers){
    
    if(head != NULL){
        printerHelper(head->leftnode, nodePointers);
        printerHelper(head->rightnode, nodePointers);
        nodePointers[head->thisnode -1] = head;
    }
    else{
        return;
    }
    
}



int determineCoordinates(Node *head, double x, double y){
    if(head==NULL){
        return 0;
    }
    head->xcoord = x;
    head->ycoord = y;
    
    if(head->cutline == '-'){
        return 0;
    }

    if(head->cutline == 'H'){
        determineCoordinates(head->leftnode, x, y + head->rightnode->height);
        determineCoordinates(head->rightnode, x, y);
    }
    if(head->cutline == 'V'){
        determineCoordinates(head->leftnode, x, y);
        determineCoordinates(head->rightnode, x + head->leftnode->width, y);
        
    }



    return 0;
    
}


