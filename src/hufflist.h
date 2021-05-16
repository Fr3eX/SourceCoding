#ifndef HUFFLIST_H
#define HUFFLIST_H


typedef struct huffList 
{
	huffNode* node;
	struct huffList *next;
}huffList;



huffList* initList(huffNode*);


bool insert(huffList** list,huffNode*);
bool insertSorted(huffList**,huffNode*);
bool insertQueue(huffList**,huffNode*);

void sortHuffList(huffList*);

unsigned char list_size(huffList*);

void printHuffList(huffList*);
void freeList(huffList*);


#endif
