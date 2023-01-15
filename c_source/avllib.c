#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>

#include "common_types.h"

AVLNode * createAVLNodeNode(){
	AVLNode *a = malloc(sizeof(AVLNode));
	if(a == NULL){
		printf("\nMemory allocation error.\n");
		exit(4);
	}

	a->Value = 0;
	a->leftNode = NULL;
	a->rightNode = NULL;
	a->Balance = 0;

	return a;
}

AVLNode * RotationLeftNode(AVLNode *a){
	AVLNode *b = a->rightNode;
	a->rightNode = b->leftNode;
	b->leftNode = a;
	return b;
}

AVLNode * RotationRightNode(AVLNode *a){
	AVLNode *b = a->leftNode;
	a->leftNode = b->rightNode;
	b->rightNode = a;
	return b;
}