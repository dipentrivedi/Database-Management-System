#include<stdio.h>
#include "dberror.h"
#include "storage_mgr.h"
#include "test_helper.h"




void initStorageManager(void)
{

}

/***********************************/
/* create, open, close and destroy */
/***********************************/

RC createPageFile(char *fileName)
{
	int i;
   FILE *aFilePtr = fopen(fileName, "wb");				/*File test_pagefile.bin* is opened*/

	if (aFilePtr == NULL)
	{
		return RC_FILE_NOT_FOUND;						/* File pointer gets null value and file is not created*/

	}
	else
	{
		for (i = 0; i < PAGE_SIZE; i++)
		{
			fputc(0, aFilePtr);						/*File pointer is puting the NULL value in the file*/

		}
		fclose(aFilePtr);								/*File is closed*/
		return RC_OK;
	}
}

RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{
	FILE *aFilePtr = NULL;
	aFilePtr = fopen(fileName, "rb");							/*File test_pagefile.bin is opened*/

	if (aFilePtr == NULL)
	{
		return RC_FILE_NOT_FOUND;								/* File pointer gets null value and file is not created*/
	}
	else
	{
		fHandle->fileName = fileName;							/*Values of fileName, mgmtInfo, totalNumPages and curPagePos are assigned*/
		fHandle->mgmtInfo = aFilePtr;
		fseek(aFilePtr, 0L, SEEK_END);
		unsigned long sz = ftell(aFilePtr);						/*File pointer goes to the last positon of the file*/
		fHandle->totalNumPages = sz / PAGE_SIZE;				/* and gives total pages in file*/
		fHandle->curPagePos = 0;
		fseek(aFilePtr, 0L, SEEK_SET);
		return RC_OK;
	}
}

RC closePageFile(SM_FileHandle *fHandle)
{
	FILE *aFilePtr = fHandle->mgmtInfo;
	if (aFilePtr == NULL)
	{
		return RC_FILE_NOT_FOUND;						/* File pointer gets null value and file is not created*/
	}

	int returnValue = fclose(aFilePtr);					/*File test_pagefile.bin is closed */
	if (returnValue == 0)
	{
		fHandle->mgmtInfo = NULL;
		aFilePtr = 0;									/*File successfullly closed*/
		return RC_OK;
	}
	else
	{
		return RC_FILE_NOT_FOUND;						/*Error in closing the file*/
	}
}

RC destroyPageFile(char *fileName)
{
	int returnValue = remove(fileName);					/*File test_pagefile.bin is destroyed*/
	if (returnValue == 0)
	{
		return RC_OK;									/*file is successfully destroyed*/
	}
	else
	{
		return RC_FILE_NOT_FOUND;						/*Error in destroying the file*/
	}
}



/***********************************/
/* reading blocks from disc		   */
/***********************************/

RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *aFilePtr;
	aFilePtr = fHandle->mgmtInfo;								/*Gets the address of the file*/
	int returnSeek, totalPages, diff, returnRead, currentPos;	

	if (aFilePtr == NULL)
	{
		return RC_FILE_NOT_FOUND;								/*File does not exist*/
	}
	else
	{
		returnSeek = fseek(aFilePtr, (pageNum)*PAGE_SIZE, SEEK_SET);	/*file poiter is assigned to the file*/

		if (fHandle->totalNumPages < pageNum)
		{
			return RC_READ_NON_EXISTING_PAGE;							/*PageNumb is greater than the total pages in the file*/
		}
		else
		{
			if (returnSeek == 0)										/*PageNum is equal or less than the total pages in the file*/
			{
				returnRead = fread(memPage, 1, PAGE_SIZE, aFilePtr);	/*Reads the page of the size PageSize*/
				currentPos = ftell(aFilePtr);							/*gets the current position of the file pointer*/
				if (returnRead >= 0)
				{														/*page read successfully*/
					fHandle->curPagePos = currentPos / PAGE_SIZE;		/*Assigns the current page position to the curPagePos*/
					return RC_OK;
				}
				else
				{
					return RC_FILE_NOT_FOUND;							/*error in reading the page*/
				}

			}
		}

	}
}
int getBlockPos(SM_FileHandle *fHandle)
{
	int blkpos = 0;
	if (fHandle != NULL)
	{
		blkpos = fHandle->curPagePos;									/*sets the current postion of the page*/
	}
	else
	{
		printf("Current Page Not Available");							/*page not available or file pointer is NULL*/
	}

	return blkpos;
}
RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *aFilePtr;
	aFilePtr = fHandle->mgmtInfo;										/*Gets the address of the file*/
	if (aFilePtr == NULL)
	{
		return RC_FILE_NOT_FOUND;										/*File does not exist*/
	}
	else
	{
		int returnValue = fseek(aFilePtr, 0, SEEK_SET);					/*File pointer is set to the 0th position of the block*/
		if (returnValue == 0)
		{
			int currentPos = ftell(aFilePtr);							/*Gets the current position of the pointer*/
			if (fread(memPage, PAGE_SIZE, 1, aFilePtr) == -1)
			{
				return RC_FILE_NOT_FOUND;								/*First read block is not sucessfull*/
			}
			else
			{															/*ReadFirstBlock is sucessfull*/
				return RC_OK;											/*current position of the page is assignes to the curPagePos*/
			}
		}
		else
		{
			return RC_FILE_NOT_FOUND;									/*ReadFirstBlock is unsucessfull*/
		}
	}
}
RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *tempfile;
	tempfile = fHandle->mgmtInfo;										/*Gets the address of the file*/

	if (tempfile == NULL || (fHandle->curPagePos) - 1 <0)
	{
		return RC_READ_NON_EXISTING_PAGE;								/*Read Previous Block is unsuccessfull*/
	}
	else
	{
		fseek(tempfile, (PAGE_SIZE *((fHandle->curPagePos) - 1)) + 1, SEEK_SET);	/*File pointer is set to the previous block of current block */


		fread(memPage, 1, PAGE_SIZE, tempfile);							/*Read Previous block is successfull*/
		fHandle->curPagePos = (fHandle->curPagePos) - 1;				/*new current position of pointer is assigned to the curPagePos*/

		return RC_OK;
	}

}

RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *aFilePtr;
	aFilePtr = fHandle->mgmtInfo;										/*Gets the address of the file*/
	if (aFilePtr == NULL)
	{
		return RC_FILE_NOT_FOUND;										/*file not found*/
	}
	else
	{
		int currentPos = fHandle->curPagePos;							/*get current position of page*/

		if (currentPos < 0)
		{
			return RC_BLOCK_NOT_FOUND;									/*no page found*/
		}
		else
		{
			int returnValue = fseek(aFilePtr, currentPos, SEEK_SET);	/*file pointer is set to the currentPos*/
			if (returnValue == 0)
			{
				if (fread(memPage, PAGE_SIZE, 1, aFilePtr) == -1)
				{
					return RC_FILE_NOT_FOUND;							/*ReadCurrentBlock is not successfull*/
				}
				else
				{
					return RC_OK;										/*ReadCurrentBlock is successfull*/
				}

			}
			else
			{
				return RC_FILE_NOT_FOUND;								/*Seek is not successfull*/
			}
		}
	}
}

RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *tempfile;
	tempfile = fHandle->mgmtInfo;											/*Gets the address of the file*/

	if (tempfile == NULL || (fHandle->curPagePos) + 1 >(fHandle->totalNumPages))	/*File pointer is not assigned*/
	{																				/*and*/
		return RC_READ_NON_EXISTING_PAGE;											/*readNextBlock is unsuccessfull*/		
	}
	else
	{
		fseek(tempfile, (PAGE_SIZE *((fHandle->curPagePos) + 1)) + 1, SEEK_SET);	/*File pointer is set*/

		fread(memPage, 1, PAGE_SIZE, tempfile);										/*current page is read*/
		fHandle->curPagePos = (fHandle->curPagePos) + 1;							/*ReadNextBlock is successfull*/
		return RC_OK;
	}


}
RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *aFilePtr;
	int currentPos;
	aFilePtr = fHandle->mgmtInfo;													/*Gets the address of the file*/
	if (aFilePtr == NULL)
	{
		return RC_BLOCK_NOT_FOUND;													/*File not found*/
	}
	else
	{
		currentPos = fHandle->curPagePos;
		int returnValue = fseek(aFilePtr, 0, SEEK_END);								/*file pointer is set to the end of the file*/
		if (returnValue == 0)
		{
			int currentPos = ftell(aFilePtr);
			if (fread(memPage, PAGE_SIZE, 1, aFilePtr) == -1)
			{
				return RC_FILE_NOT_FOUND;											/*file read not successfull*/
			}
			else
			{
				fHandle->curPagePos = currentPos / PAGE_SIZE;						/*get current page*/

				return RC_OK;
			}
		}
		else
		{
			return RC_FILE_NOT_FOUND;												/*File not found*/
		}
	}
}



/***********************************/
/* writing blocks to a page file   */
/***********************************/

RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *f;
	int len, i, curPage, relPos, diff, newPos;
	f = fopen(fHandle->fileName, "wb");         // opening the desired file
	len = ftell(f);                             // Get the position of pointer
	curPage = len / 4096; 				        // Get the value of current Page
	if (f==NULL){
		return RC_FILE_NOT_FOUND;
	}
	else{
		if (curPage>pageNum)
		{                        // compare the value of current page and given pagenumber, so that pointer can be moved to desired position
			diff = curPage - pageNum;                // getting the new page from the current paage
			newPos = 4096 * diff;                     // getting new position
			relPos = (curPage * 4096) - newPos;       // getting new position from beginning
			fseek(f, relPos, SEEK_SET);               // setting pointer to the new position
			fwrite(memPage, 1, strlen(memPage), f);  // writing at the new position

			fHandle->curPagePos = (ftell(f) / PAGE_SIZE) - 1;
			fHandle->totalNumPages = fHandle->totalNumPages + 1;
			fclose(f);
			return RC_OK;
		}
		else if (curPage<pageNum)
		{
			diff = pageNum - curPage;               // getting the new page from the current paage
			newPos = 4096 * diff;                     // getting new position
			relPos = (curPage * 4096) + newPos;       // getting new position from beginning
			fseek(f, relPos, SEEK_SET);               // setting pointer to the new position
			fwrite(memPage, 1, strlen(memPage), f);  // writing at the new position

			fHandle->curPagePos = (ftell(f) / PAGE_SIZE) - 1;
			fHandle->totalNumPages = fHandle->totalNumPages + 1;
			fclose(f);
			return RC_OK;
		}
		else
		{
			fseek(f, pageNum * 4096, SEEK_SET);         // setting pointer to the new position
			fwrite(memPage, 1, strlen(memPage), f);  // writing at the new

			fHandle->curPagePos = (ftell(f) / PAGE_SIZE) - 1;
			fHandle->totalNumPages = fHandle->totalNumPages + 1;
			fclose(f);
			return RC_OK;
		}

	}
}
RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *f;
	int len, i, curPage, relPos, diff, newPos;
	f = fopen(fHandle->fileName, "a");                         // opening the desired file
	if (f == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}
	else
	{
		len = ftell(f);                                   // Get the position of pointer
		int returnValue = fseek(f, len, SEEK_SET);                            // setting pointer to the new position
		if (returnValue == 0)
		{
			i = fwrite(memPage, 1, strlen(memPage), f);            // writing at the new position
			fHandle->curPagePos = (ftell(f) / PAGE_SIZE) - 1;
			fHandle->totalNumPages = fHandle->totalNumPages + 1;
			fclose(f);
			return RC_OK;
		}
		else
		{
			return RC_FILE_NOT_FOUND;
		}
	}	
}
RC appendEmptyBlock(SM_FileHandle *fHandle)
{
	FILE *f;
	int len;
	f = fopen(fHandle->fileName, "a");                         // opening the desired file
	int returnValue = fseek(f, 0, SEEK_END);                   // set the pointer to the end of the file
	if (returnValue == 0)
	{
		len = ftell(f);                                   // Get the position of pointer
		fHandle->totalNumPages++;					  // increase the number of pages by one
		fclose(f);
		return RC_OK;
	}
	else
	{
		return RC_FILE_NOT_FOUND;
	}
	
}
RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)
{
	FILE *f;
	f = fopen(fHandle->fileName, "a");                           // opening the desired file
	if (f == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}
	else
	{
		if (fHandle->totalNumPages < numberOfPages){                 // capacity check
			fHandle->totalNumPages = numberOfPages;                 // creating the page capacity equal to the number of pages
		}
		fclose(f);
		return RC_OK;
	}	
}