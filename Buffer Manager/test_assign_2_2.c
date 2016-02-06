#include "storage_mgr.h"
#include "buffer_mgr_stat.h"
#include "buffer_mgr.h"
#include "dberror.h"
#include "test_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// var to store the current test's name
char *testName;

// check whether two the content of a buffer pool is the same as an expected content 
// (given in the format produced by sprintPoolContent)
#define ASSERT_EQUALS_POOL(expected,bm,message)			        \
  do {									\
    char *real;								\
    char *_exp = (char *) (expected);                                   \
    real = sprintPoolContent(bm);					\
    if (strcmp((_exp),real) != 0)					\
	      {									\
	  printf("[%s-%s-L%i-%s] FAILED: expected <%s> but was <%s>: %s\n", TEST_INFO, _exp, real, message); getchar(); \
		free(real);							\
		exit(1);							\
	      }									\
	 printf("[%s-%s-L%i-%s] OK: expected <%s> and was <%s>: %s\n",TEST_INFO, _exp, real, message); \
    free(real);								\
    } while(0)

static void testCLOCK(void);

// main method
int
main(void)
{
	testCLOCK();
}


void
testCLOCK()
{
	// expected results
	const char *poolContents[] = {
		"[2 0],[-1 0],[-1 0]",
		"[2 0],[3 0],[-1 0]",
		"[2 1],[3 0],[-1 0]",
		"[2 1],[3 0],[1 0]"
	};
	const int requests[] = { 2, 3, 2, 1};
	const int numLinRequests = 4;
	//const int numChangeRequests = 3;

	int i;
	BM_BufferPool *bm = MAKE_POOL();
	BM_PageHandle *h = MAKE_PAGE_HANDLE();

	testName = "Testing CLOCK page replacement";

	CHECK(createPageFile("testbuffer.bin"));



	CHECK(initBufferPool(bm, "testbuffer.bin", 3, RS_CLOCK, NULL));

	// reading some pages linearly with direct unpin and no modifications
	for (i = 0; i < numLinRequests; i++)
	{
		pinPage(bm, h, requests[i]);
	    unpinPage(bm, h);
		ASSERT_EQUALS_POOL(poolContents[i], bm, "check pool content");
	}

	CHECK(shutdownBufferPool(bm));
	CHECK(destroyPageFile("testbuffer.bin"));

	free(bm);
	free(h);
	TEST_DONE();
	getchar();


}