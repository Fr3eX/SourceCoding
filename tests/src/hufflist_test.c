#include <criterion/criterion.h>

#include "../../src/huffNode.h"
#include "../../src/hufflist.h"

#include <stdbool.h>

huffList* list;

void suitesetup(void)
{	
	initList(initTree('a',10));	
}

void suiteteardown(void)
{
	freeList(list);
}


TestSuite(huffList,.init=suitesetup,.fini=suiteteardown);


Test(huffList,insertion)
{
	cr_expect(insert(&list,initTree('b',12))==true,"Insert Error!");
	cr_expect(insert(&list,initTree('c',15))==true,"Insert Error!");
	cr_expect(insertSorted(&list,initTree('d',24))==true,"Insert Error!");
	cr_expect(insertSorted(&list,initTree('e',4))==true,"Insert Error!");
	cr_expect(insertQueue(&list,initTree('f',8))==true,"Insert Error!");
	cr_expect(insertQueue(&list,initTree('g',3))==true,"Insert Error!");

	printHuffList(list);
}
