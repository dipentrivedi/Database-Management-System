#include "buffer_mgr.h"
#include "storage_mgr.h"
#include<stdio.h>
#include "buffer_mgr_stat.h"
#include "dt.h"
#include "dberror.h"
#include "stdlib.h"
#include "string.h"
int tt = 0;
int flag = 0;

#define MAKE_BUFFER_POOL_STAT()				\
	((init_buffer_pool *)malloc(sizeof(init_buffer_pool)))

#define MAKE_BUFFER_STAT()				\
	((buffer_stat *)malloc(sizeof(buffer_stat)))

#define MAKE_BUFFER_FIFO()				\
	((buffer_Fifo *)malloc(sizeof(buffer_Fifo)))

//#define MAKE_PAGE_HANDLE()				\
	//((BM_PageHandle *)malloc(sizeof(BM_PageHandle)))

#define MAKE_BUFFER_LRU()				\
	((buffer_lru *)malloc(sizeof(buffer_lru)))

#define MAKE_BUFFER_CLOCK()				\
	((buffer_Clock *)malloc(sizeof(buffer_Clock)))


typedef struct page_handle {
	int page;
} page_handle;

typedef struct init_buffer_pool {
	int readIO;
	int writeIO;
	PageNumber *pageNum_stat;
	int *fixCounts_stat;
	bool *dirtyPages_stat;
	} init_buffer_pool;

typedef struct buffer_stat {
	void *buf;
	struct init_buffer_pool *pool_Stat;
	struct buffer_Fifo *buff_Fifo;
	SM_FileHandle *fh;
	struct buffer_lru *buff_lru;
struct buffer_Clock *buff_Clock;
  page_handle *page_handle;
} buffer_stat;

typedef struct buffer_Clock{
	int flag_Clock;
	int pagePosClock;
	int pos;
} buffer_Clock;

typedef struct buffer_Fifo{
	int flag;
	int r;
	int f;
	int s;
	int p;
} buffer_Fifo;

typedef struct buffer_lru
{
	int age[5];
	int data[10];

}buffer_lru;

// Buffer Manager Interface Pool Handling
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName,
	const int numPages, ReplacementStrategy strategy,
	void *stratData)
{
	SM_FileHandle *fh = (SM_FileHandle *)malloc(PAGE_SIZE);
	memset(fh, 0, PAGE_SIZE);

	buffer_stat *stat;
	init_buffer_pool *pool_init;
	buffer_Fifo *buff_Fifo_Start;
	buffer_lru *buff_lru_start;
buffer_Clock *buff_clock_start;

	//page_handle *page_handle_Start;
	PageNumber* pageNumArray;
	int* fixCountsArray;
	bool* dirtyMarked;
	int i = 0;
	pool_init = MAKE_BUFFER_POOL_STAT();
	stat = MAKE_BUFFER_STAT();
	buff_Fifo_Start = MAKE_BUFFER_FIFO();
	buff_lru_start = MAKE_BUFFER_LRU();
buff_clock_start = MAKE_BUFFER_CLOCK();
	
	
	//page_handle_Start = MAKE_PAGE_HANDLE();
	// initStore buffer manager 
	bm->pageFile = (char*)pageFileName;
	bm->numPages = numPages;
	bm->strategy = strategy;
	
	//initialize the pool_stat members
	pageNumArray = (PageNumber *)malloc(sizeof (PageNumber)* bm->numPages);
	fixCountsArray = (int *)malloc(sizeof (int)*numPages);
	dirtyMarked = (bool *)malloc(sizeof (bool)* numPages);
	for (i = 0; i < bm->numPages; i++)
	{
		pageNumArray[i] = NO_PAGE;
		fixCountsArray[i] = 0;
		dirtyMarked[i] = false;
	}
	pool_init->readIO = 0;
	pool_init->writeIO = 0;
	pool_init->pageNum_stat = pageNumArray;
	pool_init->dirtyPages_stat = dirtyMarked;
	pool_init->fixCounts_stat = fixCountsArray;

	buff_clock_start->flag_Clock = 0;
	buff_clock_start->pos = 0;
	buff_clock_start->pagePosClock = 0;
	stat->buff_Clock = buff_clock_start;

	buff_Fifo_Start->flag = 0;
	buff_Fifo_Start->r = 0;
	buff_Fifo_Start->f = 0;
	//buff_Fifo_Start->s = 0;
	buff_Fifo_Start->p = 0;
	int t;
	for (t = 0; t <= 10; t++)
	{
		buff_lru_start->data[t] = 0;
	}

	for (t = 0; t <= 5; t++)
	{
		buff_lru_start->age[t] = 0;
	}
	//page_handle_Start->page = NO_PAGE;

	stat->fh = fh;
	//stat->buf = strt_fifo;
	stat->pool_Stat = pool_init;
	stat->buff_Fifo = buff_Fifo_Start;
	stat->buff_lru = buff_lru_start;

	bm->mgmtData = stat;
	return RC_OK;
}

RC shutdownBufferPool(BM_BufferPool *const bm)
{
	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	buffer_Fifo *aBuff_Fifo = start->buff_Fifo;
	buffer_lru *aBuff_lru = start->buff_lru;

	if (bu->dirtyPages_stat)
		bu->writeIO++;
	
	closePageFile(start->fh);
	
	free(start->fh);
	
	free(bu);
	
	free(aBuff_Fifo);
	
	
	return RC_OK; 
}


RC pinPage(BM_BufferPool *const bm, BM_PageHandle *const page,	const PageNumber pageNum)
{
	SM_PageHandle ph;
	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	buffer_Fifo *aBuff_Fifo = start->buff_Fifo;
	buffer_lru *aBuff_lru = start->buff_lru;
buffer_Clock *aBuff_Clock = start->buff_Clock;
	int i;
	PageNumber *pgArr = bu->pageNum_stat;
	int *fcArr = bu->fixCounts_stat;
	bool *dirtyArr = bu->dirtyPages_stat; 

	ph = (SM_PageHandle)malloc(PAGE_SIZE);
	memset(ph, 0, PAGE_SIZE);

	page->data = malloc(sizeof(char)* 512);
	switch (bm->strategy)
	{
	case RS_FIFO:
		
		page->pageNum = pageNum;
		sprintf(page->data, "%s-%i", "Page", page->pageNum);
		for (i = 0; i < bm->numPages; i++)
		{
			if (pageNum == pgArr[i]) //for page already in buffer
			{
				aBuff_Fifo->s = i;
				aBuff_Fifo->flag = 1;
				page->pageNum = pageNum;
				//readBlock(pageNum, stat->fh, ph);
				break;
			}

		}
		if (aBuff_Fifo->flag == 0)
		{
			if (aBuff_Fifo->r < bm->numPages) //first time insertions
			{
				pgArr[aBuff_Fifo->r] = pageNum;
				fcArr[aBuff_Fifo->r]++;
				dirtyArr[aBuff_Fifo->r] = false;
				aBuff_Fifo->r++;
				page->pageNum = pageNum;
				bu->readIO++;
				//aBuff_Fifo->s++;
			}
			else
			{
				if (aBuff_Fifo->f < bm->numPages) //rest all insertions
				{
					if (fcArr[aBuff_Fifo->f] > 0)
					{
						aBuff_Fifo->f++;
						pgArr[aBuff_Fifo->f] = pageNum;
						fcArr[aBuff_Fifo->f]++;
						dirtyArr[aBuff_Fifo->f] = false;
						aBuff_Fifo->f++;
						bu->readIO++;
						page->pageNum = pageNum;
					}
					else
					{
						pgArr[aBuff_Fifo->f] = pageNum;
						fcArr[aBuff_Fifo->f]++;
						dirtyArr[aBuff_Fifo->f] = false;
						aBuff_Fifo->f++;
						bu->readIO++;
						page->pageNum = pageNum;
					}
				}
				else
				{
					aBuff_Fifo->f = 0;
					pgArr[aBuff_Fifo->f] = pageNum;
					fcArr[aBuff_Fifo->f]++;
					dirtyArr[aBuff_Fifo->f] = false;
					aBuff_Fifo->f++;
					bu->readIO++;
					page->pageNum = pageNum;
				}
			}
		}
		else
		{
			pgArr[aBuff_Fifo->s] = pageNum;
			fcArr[aBuff_Fifo->s]++;
			dirtyArr[aBuff_Fifo->s] = false;
			aBuff_Fifo->flag = 0;
			page->pageNum = pageNum;
		}
		
		break;
	case RS_LRU:

		// insertions if data is already in buffer
		if (tt == 5)
		{
			int s = 0, flag = 0,t;
			for (t = 0; t < bm->numPages; t++)
			{
				
				if (pgArr[t]== pageNum)
				{
					s = t;
					flag = 1;
					break;
				}
			}
			if (flag == 1)
			{
				int r;
				for (r = 0; r < bm->numPages; r++)
				{
					if (s == r)
					{
						aBuff_lru->age[r] = 1;
						//printf("\nAge of %d element===%d", r, aBuff_lru->age[r]);
						
					}

					else
					{
						aBuff_lru->age[r] = aBuff_lru->age[r] + 1;
						//printf("\nAge of %d element===%d", r, aBuff_lru->age[r]);
					}

				}
			}

			//insertion while new data comes
			else
			{
				int max, tempmax = 0;
				max = aBuff_lru->age[0];
                                int k;
				for (k = 0; k < bm->numPages; k++)
				{
					if (aBuff_lru->age[k] > max)
					{
						max = aBuff_lru->age[k];
						tempmax = k;
						//printf("\n tempmax= %d", tempmax);
					}
					
				}
				bu->readIO++;

				pgArr[tempmax] = pageNum;
int s;
				for (s = 0; s < bm->numPages; s++)
				{
					if (s == tempmax)
					{

						aBuff_lru->age[tempmax] = 1;
						//printf("\nAge of %d element===%d", s, aBuff_lru->age[s]);
					}

					else
					{
						aBuff_lru->age[s]++;
						//printf("\nAge of %d element===%d", s, aBuff_lru->age[s]);
					}
				}

			}

		}

		//first time insertion
		else
		{
			//pgArr[aBuff_lru->data[i]] = pageNum;
			pgArr[tt] = pageNum;
			bu->readIO++;
			


			if (tt == 0)
			{
				aBuff_lru->age[tt] = 1;
				
			}


			else
			{

				int x = 0;
				for (x = tt; x > 0; x--)
				{
					aBuff_lru->age[x] = aBuff_lru->age[x - 1];
					
				}
				aBuff_lru->age[0]++;
				


			}
		tt++;
		}
		break;
case RS_CLOCK:

		
		for (i = 0; i < bm->numPages; i++)
		{
			if (pageNum == pgArr[i]) //for page already in buffer
			{
				aBuff_Clock->pos= i;
				aBuff_Clock->flag_Clock = 1;
				fcArr[i]++;
				page->pageNum = pageNum;
				break;
			}
		}
		if (aBuff_Clock->flag_Clock == 0)
		{
			
			if (aBuff_Clock->pagePosClock < bm->numPages)
			{
				pgArr[aBuff_Clock->pagePosClock] = pageNum;
				fcArr[aBuff_Clock->pagePosClock]++;
				
				aBuff_Clock->pagePosClock++;
				page->pageNum = pageNum;
				
				
			}
			else
			{
				
				aBuff_Clock->pagePosClock = 0;
				for (i = 0; i < bm->numPages; i++)
				{
					fcArr[aBuff_Clock->pagePosClock] = 0;
				}

				pgArr[aBuff_Clock->pagePosClock] = pageNum;
				fcArr[aBuff_Clock->pagePosClock]++;
				
				aBuff_Clock->pagePosClock++;
				page->pageNum = pageNum;
				
			}
		}
		else
		{
			
			pgArr[aBuff_Clock->pos] = pageNum;
			fcArr[aBuff_Clock->pos]++;
			dirtyArr[aBuff_Clock->pos] = false;
			aBuff_Clock->flag_Clock = 0;
			page->pageNum = pageNum;
		}

		break;

	default:
		return RC_BUFFER_UNDEFINED_STRATEGY;
	}
	return RC_OK;
}


RC unpinPage(BM_BufferPool *const bm, BM_PageHandle *const page)
{
	
	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	buffer_Fifo *ab = start->buff_Fifo;
	PageNumber *pgArr = bu->pageNum_stat;
	int *fcArr = bu->fixCounts_stat;
	bool *dirtyArr = bu->dirtyPages_stat;

	int i=0;

	for (i = 0; i < bm->numPages; i++)
	{
		if (pgArr[i] ==page->pageNum && fcArr[i] >= 1) //check for dirty bit before unpinning
		{
			if (dirtyArr[i]){
				bu->writeIO++;
			}
			fcArr[i]--;
			break;
		}
	}
	return RC_OK;
}

RC markDirty(BM_BufferPool *const bm, BM_PageHandle *const page){

	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	PageNumber *pgArr = bu->pageNum_stat;
	int *fcArr = bu->fixCounts_stat;
	bool *dirtyArr = bu->dirtyPages_stat;
	int i = 0;
	for (i = 0; i < bm->numPages; i++)
	{
		if (pgArr[i] == page->pageNum)
		{
			dirtyArr[i] = true;
		}
	}
	return RC_OK;
}


RC forcePage(BM_BufferPool *const bm, BM_PageHandle *const page)
{
	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	buffer_Fifo *ab = start->buff_Fifo;
	PageNumber *pgArr = bu->pageNum_stat;
	bool *dirtyArr = bu->dirtyPages_stat;

	int i = 0;
	
	for (i = 0; i < bm->numPages; i++)
	{
		if (pgArr[i] == page->pageNum)
		{
			if (dirtyArr[i]){
				bu->writeIO++;
			}
			break;
		}
	}
	return RC_OK;
}

RC forceFlushPool(BM_BufferPool *const bm)
{
	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	buffer_Fifo *ab = start->buff_Fifo;
	PageNumber *pgArr = bu->pageNum_stat;
	int *fcArr = bu->fixCounts_stat;
	bool *dirtyArr = bu->dirtyPages_stat;

	int i = 0;

	switch (bm->strategy)
	{
	case RS_FIFO:
		for (i = 0; i < bm->numPages; i++)
		{
			if (dirtyArr[i])
			{
				dirtyArr[i] = false;
				//bu->writeIO++;
			}
		}
		break;
	default:
		return RC_BUFFER_UNDEFINED_STRATEGY;
	}
	return RC_OK;
}

// Statistics Interface
PageNumber *getFrameContents(BM_BufferPool *const bm)
{
	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	return bu->pageNum_stat;
}

bool *getDirtyFlags(BM_BufferPool *const bm)
{
	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	return bu->dirtyPages_stat;
}

int *getFixCounts(BM_BufferPool *const bm)
{
	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	return bu->fixCounts_stat;
}

int getNumReadIO(BM_BufferPool *const bm)
{
	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	return bu->readIO;
}
int getNumWriteIO(BM_BufferPool *const bm)
{
	buffer_stat *start = bm->mgmtData;
	init_buffer_pool  *bu = start->pool_Stat;
	return bu->writeIO;
}
