#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "huffNode.h"
#include "hufflist.h"	


/*	
 *	initList : can be used for initiate a list or creating a simple item 
 *
 * */

huffList* initList(huffNode* node)
{
	huffList* list=(huffList*)malloc(sizeof(*list));

	if(!list || !node)
		return NULL;	

	list->node=node;
	list->next=NULL;
}

bool insert(huffList** list,huffNode* node)
{
	huffList** ll=list;	
	if(!ll)
	{
		*ll=initList(node);	
		if(!ll)
			return false;
	}	

	huffList* new=(huffList*)malloc(sizeof(*new));
	if(!new)
		return false;
	new->node=node;	
	new->next=*ll;
 	
	*ll=new;
	
	return ll;
}


bool insertQueue(huffList** list,huffNode* node)
{
	huffList* ll=*list;	
	
	if(!ll)
	{
		*list=initList(node);
		return true;
	}

	while(ll->next!=NULL)ll=ll->next;
	
	huffList* new=(huffList*)malloc(sizeof(*new));
	if(!new)
		return false;
	new->node=node;	
	new->next=NULL;
 	
	ll->next=new;

	return ll;
}




bool insertSorted(huffList** list_sorted,huffNode* node)
{
	huffList* ll=*list_sorted;
	huffList* tmp=NULL;
	huffNode* tmp_node;
	if(!ll)
	{
		*list_sorted=initList(node);
		return true;
	}
	while(ll)
	{
		if( *node->frequency <= *(ll)->node->frequency ) 
		{
			huffList*  new_item=initList(node);
			
			if(!new_item)
				return false;	
			
			
			tmp=(ll)->next;
			(ll)->next=new_item;
			new_item->next=tmp;
		
			tmp_node=(ll)->node;
			(ll)->node=node;
			new_item->node=tmp_node;
			
			return true;		
		}
		else if(!(ll)->next)
		{
			huffList * new_item=initList(node);
			if(!new_item)
				return false;

			new_item->next=NULL;	
			(ll)->next=new_item;
	
			return true;
		}
			
		ll=(ll)->next;
	}	

	return false;
}

void sortHuffList(huffList* list)
{
	huffList* ll=list;
		
	if(!ll)
		return; 

	
	huffList* list_tmp=NULL;
	huffNode* node_tmp=NULL;
	unsigned int min=(!ll->node)?0:*ll->node->frequency;

	while(ll)
	{
		for(huffList* tmp=ll->next;tmp;tmp=tmp->next)
		{
			if(*tmp->node->frequency <= min)
			{
				min=*tmp->node->frequency;		
				list_tmp=tmp;
			}
		}		
		
		if(list_tmp)
		{
			node_tmp=ll->node;
			ll->node=list_tmp->node;
			list_tmp->node=node_tmp;		
		}		
		
		ll=ll->next;
		min=(!ll)?0:*ll->node->frequency;
		list_tmp=NULL;
		node_tmp=NULL;
	}
	
}


void freeList(huffList* list){

	if(!list || !list->node)
		return;
	
	freeList(list->next);	
	freeTree(list->node);
	free(list);
}

unsigned char list_size(huffList* list)
{
	if(!list) 
		return 0;
	else
		return 1 + list_size(list->next);
}




void printHuffList(huffList* list)
{
	huffList* pointer=list;
	char count=0;

	while(pointer)
	{
		printf("Item %d :\n",count);
		printNode(pointer->node);		
		pointer=pointer->next;
		count++;
	}
}
