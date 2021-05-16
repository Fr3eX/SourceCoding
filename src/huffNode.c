#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "huffNode.h"



static void freeNode(huffNode*);

huffNode* initTree(const char c,const unsigned int frequency)
{
	huffNode *node=(huffNode*)malloc(sizeof(huffNode));
	
	if(!node)
		return NULL;

	node->frequency=(unsigned int*)malloc(sizeof(unsigned int));
	node->c=(char*)malloc(sizeof(char));
	
	if(!node->frequency || !node->c)
		return NULL;

	*node->frequency=frequency;
	*node->c=c;
	
	node->lchild=NULL;
	node->rchild=NULL;	
	
	return node;
}



huffNode* createNode(const char c,const unsigned int frequency,huffNode* lchild,huffNode* rchild)
{
	huffNode* new_node=(huffNode*)malloc(sizeof(huffNode));
	
	if(!new_node)
		return NULL;

	new_node->c=malloc(sizeof(char));
	new_node->frequency=malloc(sizeof(unsigned int));

	if(!new_node->frequency || !new_node->c)
		return NULL;

	*new_node->frequency=frequency;
	*new_node->c=c;

	new_node->lchild=lchild;
	new_node->rchild=rchild;

	return new_node;
}


static void freeNode(huffNode* node)
{
	if(!node)
		return;
	
	free(node->frequency);
	free(node->c);
	free(node);
}


void freeTree(huffNode* node)
{
	if(!node)
		return;
	freeTree(node->lchild);
	freeTree(node->rchild);

	freeNode(node);
}


void printNode(huffNode* node)
{
	if(!node)
		return;
	
	printNode(node->lchild);
	printNode(node->rchild);

	printf("Node value int : %d ; char : %c ; frequency : %d \n",*node->c,*node->c,*node->frequency);
}

