#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffEng.h"


int main(int argc,char *argv[])
{
	if(argc < 4 )
	{
		fprintf(stderr,"Syntax incorrect :\n"
			"For compressing : %s -e source_file destination_file\n"
			"For decompressing : %s -d source_file destination_file\n"
			,argv[0],argv[0]);
		return EXIT_FAILURE;
	}

	if(!strcmp(argv[1],"-e"))
		encode_file(argv[2],argv[3]);
	else if(!strcmp(argv[1],"-d"))
		decode_file(argv[2],argv[3]);
	else
	{
		fprintf(stderr,"Syntax incorrect :\n"
			"For compressing : %s -e source_file destination_file\n"
			"For decompressing : %s -d source_file destination_file\n"
			,argv[0],argv[0]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
