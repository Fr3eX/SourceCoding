#ifndef HUFFNODE_H
#define HUFFNODE_H


typedef struct huffnode
{
	unsigned int* frequency;
	char *c;
	struct huffnode* lchild;
	struct huffnode* rchild;
}huffNode;


huffNode* initTree(const char,const unsigned int);
huffNode* createNode(const char,const unsigned int,huffNode*,huffNode*);

void freeTree(huffNode*);
void printNode(huffNode*);

#endif
