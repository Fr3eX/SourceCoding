#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include "huffEng.h"
#include "huffNode.h"
#include "hufflist.h"


#define MAX_CHAR 255
#define CODE_LENGTH 32

typedef struct
{
	unsigned int frequency;
}streamStat;

typedef struct 
{
	unsigned int length;
	char code[CODE_LENGTH];
}Dictionary;



static void resolve(streamStat*,char*);
static void create_tree(huffList**);
static void create_dictionary(Dictionary*,huffNode*,unsigned char*,unsigned);


bool encode_file(char* file_path,char* file_dest)
{
	streamStat* stats=malloc(sizeof(streamStat)*MAX_CHAR);
	huffList* list=NULL;	
	
	unsigned int file_size=0,asci_nbr=0;
	unsigned char bit_write,bit_read,byte_read,set;
	char code[CODE_LENGTH];
	Dictionary* dic;	

	if(!stats)
	{
		fprintf(stderr,"Not enough memory\n");
		return false;
	}
	
	FILE* stream=fopen(file_path,"r+");
	if(!stream)
	{
		fprintf(stderr,"fopen():%s\n",strerror(errno));
		return false;
	}


	char buffer[MAX_CHAR]={0};
	
	while((set=fread(buffer,1,MAX_CHAR,stream))>0)
	{
		for(unsigned char i=0;i<MAX_CHAR;i++)
			(stats+buffer[i])->frequency++;
	
	}	
	
	for(int i=1;i<MAX_CHAR;i++)	
		if((stats+i)->frequency > 0)
			insertSorted(&list,initTree(i,(stats+i)->frequency));
	/*
	*Free stream stats
	*/

	free(stats);
	
	
	fseek(stream,0,SEEK_END);
	file_size=ftell(stream);
	asci_nbr=list_size(list);

	
	FILE* destination  = fopen(file_dest,"wb+");
		
	if(!destination)
	{
		fprintf(stderr,"fopen():%s\n",strerror(errno));
		return false;
	}

	// Orginal File Size	
	fwrite(&file_size,sizeof(file_size),1,destination);
	// Ascii number 
	fwrite(&asci_nbr,sizeof(asci_nbr),1,destination);

	huffList* ll=list;
	
	while(ll)
	{
		fwrite(ll->node->c,sizeof(char),1,destination);	
		fwrite(ll->node->frequency,sizeof(unsigned int),1,destination);	
		
		#ifdef DEBUG
			printf("Writing %c : f : %d\n",*ll->node->c,*ll->node->frequency);
		#endif
		ll=ll->next;
	}


	#ifdef DEBUG
		printf("LIST INIT !! \n\n\n");
		printHuffList(list);
		printf("\n\n\n\n");
	#endif
	
	
	create_tree(&list);
	

	#ifdef DEBUG
		printf("LIST END !! \n\n\n");
		printHuffList(list);
		printf("\n\n\n\n");
	#endif
	
	dic=(Dictionary*)malloc(sizeof(Dictionary));
	if(!dic)
		fprintf(stderr,"Not enough memory\n");
	

	create_dictionary(dic,list->node,code,0);

	
		
	fseek(stream,0,SEEK_SET);

	bit_write=0x80; //1000 0000
	code[0]=0;
	memset(buffer,0,sizeof(buffer));

	

	while((set=fread(buffer,1,MAX_CHAR,stream))>0)
	{
		for(unsigned char i=0;i<set;i++)
		{

			byte_read=0;
			bit_read=0x80;
		
			for(unsigned long bit_count=0;bit_count<(dic+buffer[i])->length;bit_count++)
			{
				if((dic+buffer[i])->code[byte_read] & bit_read)
					code[0]|=bit_write;
		
				bit_read>>=1;
				if(!bit_read)
				{
					byte_read++;
					bit_read=0x80;
				}

				bit_write>>=1;
				if(!bit_write)
				{
					fwrite(code,1,1,destination);
					code[0]=0;
					bit_write=0x80;
				}
			}


		}
	}

	if(bit_write!=0x80)
		fwrite(code,1,1,destination);
	

	fprintf(stdout,"File compressed successfuly\ncompress rate %f\n",(float)file_size/ftell(destination));
	


	/*
	*	Free memory
	*/

	free(dic);
	free(list);

	
	fclose(stream);
	fclose(destination);
	
	return true;
}

bool decode_file(char* file_path,char* file_dest)
{
	unsigned int file_size,asci_nbr;	
	unsigned char bit_write,bit_read,byte_read;
	
	char c,code[CODE_LENGTH],buffer[MAX_CHAR];
	unsigned int frequency;
	huffList* list=NULL;

	FILE* stream_in=fopen(file_path,"rb+");
	FILE* stream_out=fopen(file_dest,"w+");
	
	if(!stream_in || !stream_out)
	{
		fprintf(stderr,"fopen():%s\n",strerror(errno));
		return NULL;
	}

	fread(&file_size,sizeof(file_size),1,stream_in);
	fread(&asci_nbr,sizeof(asci_nbr),1,stream_in);
	
	#ifdef DEBUG 
		printf("FILE_SIZE %d\nASCI_NBR %d\n",file_size,asci_nbr);
	#endif
	
	for(unsigned i =0;i<asci_nbr;i++)
	{
		fread(&c,sizeof(char),1,stream_in);
		fread(&frequency,sizeof(unsigned int),1,stream_in);
		insertQueue(&list,initTree(c,frequency));
		
		#ifdef DEBUG
			printf("reading  %c : f : %d\n",c,frequency);
		#endif
	}

	create_tree(&list);
	
	huffNode* tree=list->node;
	
	unsigned set;
	while((set=fread(buffer,1,sizeof(buffer),stream_in))>0)
	{
		for(int i=0;i<set && file_size>0;i++)
		{
			for(unsigned long bit_read=0x80;bit_read!=0 && file_size > 0; bit_read>>=1)
			{
				if(buffer[i] & bit_read)
					tree=tree->rchild;
				else
					tree=tree->lchild;
				
				if(!tree->lchild && !tree->rchild)
				{
					putc(*tree->c,stream_out);
					tree=list->node;
					file_size--;
				}
			}
		}

	}

	fprintf(stdout,"File decompressed successfuly \n");

	fclose(stream_in);
	fclose(stream_out);
	return true;
}


static void create_dictionary(Dictionary *dic,huffNode* node,unsigned char* code,unsigned int pos)
{
	if(! node->lchild && ! node->rchild )
	{
		memcpy((dic+(*node->c))->code,code,CODE_LENGTH);
	
		(dic+(*node->c))->length = pos;
	
		
		#ifdef DEBUG
			printf("[DIC] Char : %c ; taille code : %d ; code :  \n",*node->c,pos);
		#endif

	}
	else
	{
	
	 	//WRITE 0
		*(code+(pos/8))&=~(0x80>>(pos%8));
		create_dictionary(dic,node->lchild,code,pos+1);

		//WRITE 1
		*(code+(pos/8))|=(0x80>>(pos%8));
		create_dictionary(dic,node->rchild,code,pos+1);
	}

}

static void create_tree(huffList** sorted_list)
{
	huffList* ll=*sorted_list;
	huffList*  tmp_chunk;
	unsigned int sum_node=0;
	if(!ll)
		return;

	while(ll)
	{
	
		if((ll)->next != NULL)
		{
			sum_node=*(ll)->node->frequency + *(ll)->next->node->frequency;
			
			huffNode* new_node = createNode(-1,sum_node,(ll)->node,(ll)->next->node);
	
			tmp_chunk=(ll)->next->next;
			free((ll)->next);
			free((ll));
			

			insertSorted(&tmp_chunk,new_node);
			
			#ifdef DEBUG
				printf("CHUNK !! \n\n\n");
				printHuffList(tmp_chunk);
				printf("\n\n\n\n");
			#endif

			ll=tmp_chunk;
			*sorted_list=tmp_chunk;
			
			continue;
		}
		
		ll=(ll)->next;
	}
	
}



static void resolve(streamStat* stats,char *buffer)
{
	unsigned char i=0;

	while(*(buffer+(i++))!='\0' && i < MAX_CHAR)
		(stats+i)->frequency++;	
}
