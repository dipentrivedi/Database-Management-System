***********************************************************                    Programming assignment 2		              
Project team: spate149, npate104, npate105, dtrived5      
***********************************************************


Project Team  
1) Smruti Patel 		A20346239 
2) Nirali Patel 		A20345459 
3) Nisha Patel  		A20346064 
4) Dipen Trivedi  	A20346363 

<-Contents->

1) Work Split up 
2) Running Instruction
3) Function description


1) Work Split up process:

We did split the project into 3 modules amongst us.
->Initilization of Buffer (Basic Operations)
->FIFO
->LRU
->Clock


2) Running Instructions:

1. In the Linux terminal, navigate to the directory of the assignment contents 
//clean before running
2. Make
3. Make run
4. Make testAssign2_1
5. Make run2



3) Function Description :


i. Function Name: InitBufferPool
		Version:1.0.0	
		Expected arguments: BM_BufferPool *const bm, 
		const char *const pageFileName,
		const int numPages, ReplacementStrategy strategy,
		void *stratData

	1) Initialize the page frames depending on the buffer pool 	num pages.
	2) Dynamic memory allocation of page handle and buffer pool 	is done.
	


ii.	Function Name: ShutDownBufferPool
		Version:1.0.0	
		Expected arguments: BM_BufferPool *const bm

	1) Get the buffer page details for the pool to be 	shutdowned.
	2) check for any dirty page frames.If present , write them 	to the disc.
 	3) Free the resources associated with the pool.



iii.	Function Name: forceFlushPool
			Version:1.0.0	
			Expected arguments: BM_BufferPool *const bm

	1) Get the buffer page detail for the current pool.
 	2) Check for any dirty page frames.If present write them to 	the disc.


iv.	Function Name: pinPage
			Version:1.0.0	
			Expected arguments: BM_BufferPool *const bm, 					BM_PageHandle *const page,	const PageNumber 				pageNum
       
	1) Initialize the contents of the buffer and the structure 	with the current request of the pageNum.
	2) Check for the appropriate page replacement strategy.
	4) Get the appropriate replacement page by invoking the 	corresponding strategy.
	5) Replace that page with the new page and update the 	replacement parameters.



v.	Function Name: unpinPage
			Version:1.0.0
			Expected arguments: BM_BufferPool *const bm, 					BM_PageHandle *const page 
       
	1) Check if the page to be unpinned has fixcounts>=0 and 	pageNum is same as asked.
	2) If not throw an error
	3) If fixcounts==0, check if the page is dirty.
	4) If dirty==TRUE write it down to the disk.
	

vi.	Function Name: markDirty
			Version:1.0.0
			Expected arguments: BM_BufferPool *const bm, 					BM_PageHandle *const page
       
	1) Get a page handle.
	2) Mark the frame/page at the given handle as dirty.
	3) Return appropriate message.



vii.	Function Name: forcePage
			Version:1.0.0
			Expected arguments: BM_BufferPool *const bm, 					BM_PageHandle *const page
       
	1) Get the frame to be written to the file on disk.
	2) check if the page is dirty and if it is true, write the 	page contents to the file on disk
	3) Return appropriate message.

viii.	Function Name: getFrameContents
			Version:1.0.0	
			Expected arguments: BM_BufferPool *const bm

	1) Get all the pages frames for the given buffer pool.
	2) Read the page number of corresponding frames , put them 	in an array and return to the client.


ix.	Function Name: getDirtyFlags	
			Version:1.0.0
			Expected arguments: BM_BufferPool *const bm
       		
	1) Get all the pages frames for the given buffer pool.
	2) Read the dirty flage of corresponding frames , put them 	in an array and return to the client.


x.	Function Name: getFixCounts
     			Version:1.0.0	
			Expected arguments: BM_BufferPool *const bm

	1) Get all the pages frames for the given buffer pool.
	2) Read the fixcounts of corresponding frames , put them in 	an array and return to the client.



xi.	Function Name: getNumReadIO
       		Version:1.0.0	
			Expected arguments: BM_BufferPool *const bm

	1) Get the buffer node containg the buffer pool.
	2) returns the number of pages that have been read from disk 	since a buffer pool has been initialized.


xii.	Function Name: getNumWriteIO
       		Version:1.0.0	
			Expected arguments: BM_BufferPool *const bm

	1) Get the buffer node containg the buffer pool.
	2) returns the number of pages that have been write from 	disk 	since a buffer pool has been initialized.

xiii.	Function Name: testFIFO
			Version:1.0.0	
		
	1) Checks if the buffer is empty or not.
	2) If frame is not empty then set the value of flag=1.
	3) If frame is empty, get the perticular frame from the disk 	and store it in the buffer, increment the fixCount.
	3) Replace the first frame with new request if fixcount=0 	and  increment the fixcount.
	4) In Replacement of the frame with new request if fixcount=	1 then move to the next frame and replace that frame with 	new request and increment the fixcount.


xiv.	Function Name: testLRU
   		    Version:1.0.0	

	1) Checks if the buffer is empty or not.
	2) If buffer is empty, take the page from the disk and store 	it in the buffer, increment the age. 
	3) If buffer is not empty then search the requested page in 	the bufffer and if it is there than set flag=1 and change 	the value of age=1.
	4) If buffer is not empty then search the requested page in 	the buffer and if it is not there then go to the disk and 	take the page and then replace it with the frame that has 	max age value which also means that it is the Least Recently     	Used page.And set the age=1 for that frame and 	increment 	the age of all other frames.
	

xv. Function Name: testClock
    Version: 1.0.0	
	

	1) Checks if the buffer is empty or not.
	2) If buffer is empty, take the page from the disk and store 	it in the buffer, and increment the fix count. 
	3) If the page exists in the buffer, then we give it a chance into buffer for one more time and we increment the current fix count value. 	4) Similarly, we unpin the page currently asked for and return appropriate message to client.

Note: To make our make file run we have added “RETURN 0” in the end of our int main (void) method which returns 0 with successful code.


