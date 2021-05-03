#include <criterion/criterion.h>


extern void *test(void);

Test(tesmake,cc)
{
	cr_expect(test()==NULL,"FUnction khedama");
}

Test(tesmake,c)
{
	cr_expect(test()==NULL,"FUnction khedama");
}
