#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>

#include "common_types.h"

// ------------------------------------- ABR Manipulation -------------------------------------
int searchForValueInABR(pABRNode a, int e) {
	if(a == NULL){
		return 0;
	}

	if(a->Value == e){
		return 1;
	}

	if(a->Value > e){
		return searchForValueInABR(a->LeftNode, e);
	} else{
		return searchForValueInABR(a->RightNode, e);
	}
}

void showABRPrefix(pABRNode a){
	if(a != NULL){
		printf("%d ", a->Value);
		showABRPrefix(a->LeftNode);
		showABRPrefix(a->RightNode);
	} else {
		printf("| ");
	}
}

int IsABRProperlyFormatted(pABRNode a){
	if(a == NULL){
		return 1;
	} else {
		if(a->LeftNode != NULL){
			if(a->LeftNode->Value > a->Value){
				return 0;
			}
		}
		if(a->RightNode != NULL){
			if(a->RightNode->Value < a->Value){
				return 0;
			}
		}
		return IsABRProperlyFormatted(a->LeftNode) && IsABRProperlyFormatted(a->RightNode);
	}
}

// ------------------------------- ABR Modification -------------------------------
pABRNode insertABRrecursive(pABRNode a, int e){
	if(a == NULL){
		a = malloc(sizeof(ABRNode));
		if(a == NULL){
			printf("\nMemory allocation error.\n");
			exit(4);
		}
		a->Value = e;
		a->LeftNode = NULL;
		a->RightNode = NULL;
	} else {
		if(a->Value > e){
			a->LeftNode = insertABRrecursive(a->LeftNode, e);
		} else {
			a->RightNode = insertABRrecursive(a->RightNode, e);
		}
	}
	return a;
}

pABRNode insertABRiterative(pABRNode a, int e){
	pABRNode temp = a;
	pABRNode stockageTemp = temp;
	while(temp != NULL){
		stockageTemp = temp;
		if(temp->Value < e){
			temp = temp->LeftNode;
		} else {
			temp = temp->RightNode;
		}
	}
	temp = malloc(sizeof(ABRNode));
	if(temp == NULL){
		printf("\nMemory allocation error.\n");
		exit(4);
	}
	temp->Value = e;
	temp->LeftNode = NULL;
	temp->RightNode = NULL;

	if(stockageTemp->Value < e){
		stockageTemp->LeftNode = temp;
	} else {
		stockageTemp->RightNode = temp;
	}

	return a;
}

pABRNode delMax(pABRNode ABRNode, int * ValueToRemoveFromABR){
	pABRNode tmp;

	if (ABRNode->RightNode != NULL){
		ABRNode->RightNode = delMax(ABRNode->RightNode,ValueToRemoveFromABR);
	} else {
		*ValueToRemoveFromABR = ABRNode->Value;
		tmp = ABRNode;
		ABRNode = ABRNode->LeftNode;
		free(tmp);
	}
	return ABRNode;
}

pABRNode deleteElementFromABR(pABRNode ABRNode, int ValueToRemoveFromABR){
	pABRNode tmp;

	//Element not in tree
	if(ABRNode == NULL){
		return ABRNode;
	}

	//Recursion
	else if (ValueToRemoveFromABR > ABRNode->Value){
		ABRNode->RightNode = deleteElementFromABR(ABRNode->RightNode,ValueToRemoveFromABR);
	} else if (ValueToRemoveFromABR < ABRNode->Value){
		ABRNode->LeftNode = deleteElementFromABR(ABRNode->LeftNode,ValueToRemoveFromABR);
	}

	//It's equal.
	else if (ABRNode->LeftNode == NULL){
		tmp = ABRNode;
		ABRNode = ABRNode->RightNode;
		free(tmp);
	}

	else {
		ABRNode->LeftNode = delMax(ABRNode->LeftNode,&ABRNode->Value);
	}

	return NULL;
}