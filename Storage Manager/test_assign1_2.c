#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"

// test name
char *testName;

/* test output files */
#define TESTPF "test_pagefile.bin"

/* prototypes for test functions */
static void otherOperation(void);
//static void otherOperation(void);

/* main function running all tests */
int main(void)
{

	testName = "";
	otherOperation();
	return 0;
}



/* Try to create, open, and close a page file */
void otherOperation(void)
{
	SM_FileHandle fh;
	SM_PageHandle ph;
	int i;
	int before, after, curentPage,curtotalPage;

	testName = "test read write block method";

	ph = (SM_PageHandle)malloc(PAGE_SIZE);



	// create a new page file
	TEST_CHECK(createPageFile(TESTPF));
	TEST_CHECK(openPageFile(TESTPF, &fh));
	before = fh.totalNumPages;

	//Appending an empty block
	TEST_CHECK(appendEmptyBlock(&fh));

	after = fh.totalNumPages;
	ASSERT_TRUE((before<after), "Block appended successfully");

	//Read current block
	for (i = 0; i < PAGE_SIZE; i++)
		ph[i] = (i % 10) + '0';
	TEST_CHECK(writeBlock(0,&fh , ph));
	TEST_CHECK(readBlock(0,&fh, ph));

	for (i = 0; i < PAGE_SIZE; i++)
		ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");

	for (i = 0; i < PAGE_SIZE; i++)
		ph[i] = (i % 10) + '0';
	TEST_CHECK(writeBlock(0, &fh, ph));

	TEST_CHECK(readCurrentBlock(&fh, ph));

	for (i = 0; i < PAGE_SIZE; i++)
		ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");

	//write current block
	curentPage = fh.curPagePos;
	for (i = 0; i < PAGE_SIZE; i++)
		ph[i] = (i % 10) + '0';
	TEST_CHECK(writeCurrentBlock(&fh, ph));
	ASSERT_TRUE((curentPage < fh.totalNumPages), "Current block written successfully");

	//ensure capacity
	
	TEST_CHECK(ensureCapacity(15,&fh));
	ASSERT_TRUE(( 15 == fh.totalNumPages) , "Capacity ensured successfully");

	//Read Last Block
	TEST_CHECK(readLastBlock(&fh, ph));

	for (i = 0; i < PAGE_SIZE; i++)
		ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");

	//GetCurrent Pos
	curentPage = fh.curPagePos;
	ASSERT_TRUE((curentPage == getBlockPos(&fh)),"Successfully retrieved the block number");
	
	//Read Previous block
	curentPage = fh.curPagePos - 1;
	TEST_CHECK(readPreviousBlock(&fh, ph));
	ASSERT_TRUE((curentPage == fh.curPagePos ), "Successfully retrieved previous block number");

	//Read Next block
	curentPage = fh.curPagePos + 1;
	TEST_CHECK(readNextBlock(&fh, ph));
	ASSERT_TRUE((curentPage == fh.curPagePos), "Successfully retrieved next block number");


	///temp added because it will create prob in destroying file
	TEST_CHECK(closePageFile(&fh));

	// destroy new page file
	TEST_CHECK(destroyPageFile(TESTPF));

	TEST_DONE();

}
