#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CUnit/Basic.h"

#include "../src/linkedList.h"
#include "../src/constants.h"
#include "../src/bufferRes.h"

int init_suite1()
{
	initList();
	initBufferRes(4);

	return 0;
}
int clean_suite1()
{
	freeList();
	freeBufferRes();

	return 0;
}

void testScorePswd()
{
	CU_ASSERT(getScore("abcd",VOWEL) == 1);
	CU_ASSERT(getScore("abcd",CONSONANT) == 3);
	CU_ASSERT(getScore("bonjour",VOWEL) == 3);
	CU_ASSERT(getScore("bonjour",CONSONANT) == 4);
	CU_ASSERT(getScore("kayak",VOWEL) == 3);
	CU_ASSERT(getScore("kayak",CONSONANT) == 2);
	CU_ASSERT(getScore("ordinateur",VOWEL) == 5);
	CU_ASSERT(getScore("ordinateur",CONSONANT) == 5);
	CU_ASSERT(getScore("programmation",VOWEL) == 5);
	CU_ASSERT(getScore("programmation",CONSONANT) == 8);
}
void testLinkedList()
{
	addIfGood("abcd", VOWEL);
	CU_ASSERT(calculateSizeList() == 1);
	addIfGood("abcd", VOWEL);
	CU_ASSERT(calculateSizeList() == 2);
	addIfGood("kith", VOWEL);
	CU_ASSERT(calculateSizeList() == 3);
	addIfGood("true", VOWEL);
	CU_ASSERT(calculateSizeList() == 1);
	addIfGood("ae", VOWEL);
	CU_ASSERT(calculateSizeList() == 2);
	addIfGood("kalk", VOWEL);
	CU_ASSERT(calculateSizeList() == 2);
	addIfGood("zombie", VOWEL);
	CU_ASSERT(calculateSizeList() == 1);
	eraseList();
	CU_ASSERT(calculateSizeList() == 0);
	addIfGood("waa", CONSONANT);
	CU_ASSERT(calculateSizeList() == 1);
	addIfGood("wii", CONSONANT);
	CU_ASSERT(calculateSizeList() == 2);
	addIfGood("eitoua", CONSONANT);
	CU_ASSERT(calculateSizeList() == 3);
	addIfGood("eitoual", CONSONANT);
	CU_ASSERT(calculateSizeList() == 1);
	addIfGood("nt", CONSONANT);
	CU_ASSERT(calculateSizeList() == 2);
	addIfGood("eru", CONSONANT);
	CU_ASSERT(calculateSizeList() == 2);
	addIfGood("frtsapln", CONSONANT);
	CU_ASSERT(calculateSizeList() == 1);
}

void testBufferRes()
{
	char test[8];
	insertInBufferRes("oui");
	removeFromBufferRes(test);
	CU_ASSERT(strcmp("oui",test) == 0);

	insertInBufferRes("oui");
	insertInBufferRes("non");

	removeFromBufferRes(test);
	CU_ASSERT(strcmp("oui",test) == 0);

	insertInBufferRes("quoi");

	removeFromBufferRes(test);
	CU_ASSERT(strcmp("quoi",test) == 0);

	removeFromBufferRes(test);
	CU_ASSERT(strcmp("non",test) == 0);
}

int main()
{
	CU_pSuite pSuite = NULL;
	if(CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
	if (NULL == pSuite) {
    	CU_cleanup_registry();
    	return CU_get_error();
   	}

   	/* add the tests to the suite */
   	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   	if (
		(NULL == CU_add_test(pSuite,"getScore : compte le nombre de voyelles ou de consonnes dans un mot", testScorePswd)) ||
       	(NULL == CU_add_test(pSuite, "linkedList : tests de la liste en ajoutant des mots comme si il venait d'être crackés", testLinkedList)) ||
		(NULL == CU_add_test(pSuite,"bufferRes : buffer où on stocke les mots de passe crackés, fonctionnalités identiques au buffer des hashs", testBufferRes))
		)
   	{
    	CU_cleanup_registry();
    	return CU_get_error();
   	}

   	/* Run all tests using the CUnit Basic interface */
   	CU_basic_set_mode(CU_BRM_VERBOSE);
   	CU_basic_run_tests();

	CU_cleanup_registry();
	return CU_get_error();
}
