***********************************************************                    Programming assignment 1		              
Project team: spate149, npate104, npate105, dtrived5      
***********************************************************


Project Team  
 
1) Smruti Patel 		A20346239 
2) Nirali Patel 		A20345459 
3) Nisha Patel  		A20346064 
4) Dipen Trivedi 	A20346363 

<-Contents->

1) Work Split up 
2) Running Instruction
3) Function description
4) List of added new Test Cases




1) Work Split up process:

We did split the project into 3 modules amongst us.
->Manipulation (Basic Operations)
->Write Operations
->Read Operations


2) Running Instructions:

1. In the Linux terminal, navigate to the directory of the assignment contents 
2. make  
3. make run 
4. make testAssign2 
5. make run2 


3) Function Description :


i. Function Name: CreatePageFile
		Version:1.0.0	
		Expected arguments: char *fileName

       1) Check if the file already exists
2) If it is present, throw an error message that the file is already present.
3) If it is not already present, create the file and allocate size of one PAGE to it.


ii.	Function Name: OpenPageFile
			Version:1.0.0	
			Expected arguments: char *fileName, SM_FileHandle 						   *fHandle

       1) Check if the file with the provided file name exists.
       2) If it does not exist, throw an error.
3) If it exists, check for the total number of pages that the file has.
4) After opening the file, initiate the structure elements needed.



iii.	Function Name: ClosePageFile
			Version:1.0.0	
			Expected arguments: SM_FileHandle *fHandle

       1) Close the file and return a success message upon success.
2) If the file could not be located, return an appropriate error message.


iv.	Function Name: DestroyPageFile
			Version:1.0.0	
			Expected arguments: char *fileName
       
       1) Check if the file is present, and remove the file.
       2) Upon success, return a success message.
       3) Upon failure, return a failure message.


v.		Function Name: writeBlock 
			Version:1.0.0
	Expected arguments: int pageNum, SM_FileHandle 					*fHandle, SM_PageHandle memPage
       
       1) Check if the file is present.
       2) Get the current Position of the file.	
       3) Write Contents to the file.
       4) Close the file


vi.	Function Name: appendEmptyBlock
			Version:1.0.0
			Expected arguments: SM_FileHandle *fHandle
       
       1) Check if the file is present.
       2) Check for the total number of pages.
       3) Add one page and print'\0' in the new empty block.


vii.	Function Name: writeCurrentBlock
			Version:1.0.0
	Expected arguments: SM_FileHandle *fHandle, 					SM_PageHandle memPage
       
       1) Check if file is present.
       2) Write current block based on absolute position
       
viii.	Function Name: ensureCapacity
		Version:1.0.0	
Expected arguments: int numberOfPages, SM_FileHandle 					*fHandle

1) Try to locate the specified file, upon failure return an appropriate error message.
Upon success, continue.
       2) Calculate the number of pages that the file could 	accommodate.
3) If the file's memory is insufficient, calculate the memory needed to make sure that the file has enough capacity and allocate the same.
4) If the file's memory is sufficient, provide an appropriate message.


ix.	Function Name: readBlock
       	Version:1.0.0	
	Expected arguments: int pageNum, SM_FileHandle 					*fHandle, SM_PageHandle memPage

1)Check if the file already exists with the help of file descriptor in file handler
2)Move the file descriptors position to the page requested in pageNum.
3)Read the content of length 4096 bytes and load to the memory specified in mempage


x.	Function Name: getBlockPos
       Version:1.0.0	
       Expected arguments: SM_FileHandle *fHandle

1)Check if the file already exists with the help of file descriptor in file handler
       2)Get the current page position with the help of file 		handler


xi.	Function Name: readFirstBlock
       Version:1.0.0	
Expected arguments: SM_FileHandle *fHandle, SM_PageHandle memPage

1)Check if the file already exists with the help of file descriptor in file handler
2)Move the file descriptor in the file handler to the first page of the file
       3)Read the content to mempage.


xii.	Function Name: readLastBlock
       Version:1.0.0	
Expected arguments: SM_FileHandle *fHandle, SM_PageHandle memPage

1)Check if the file already exists with the help of file descriptor in file handler
2)Move the file descriptor in the file handler to the last page of the file
       3)Read the content to mempage.


xiii.	Function Name: readCurrentBlock
       Version:1.0.0	
Expected arguments: SM_FileHandle *fHandle, SM_PageHandle memPage

1)Check if the file already exists with the help of file descriptor in file handler
2)Read the content to mempage of the current page position in the file handler with the help of file descriptor present in the file handler.

xiv.	Function Name: readPreviousBlock
       Version:1.0.0	
Expected arguments: SM_FileHandle *fHandle, SM_PageHandle memPage

1)Check if the file already exists  or trying to access non existing page in the file  with the help of file descriptor and page position  in file handler
2)Read the content to mempage of the previuous page position in the file handler with the help of file descriptor present in the file handler.


xv.	Function Name: readNextBlock
       Version:1.0.0	
Expected arguments: SM_FileHandle *fHandle, SM_PageHandle memPage

1)Check if the file already exists  or trying to access non existing page in the file  with the help of file descriptor and page position  in file handler
2)Read the content to mempage of the next page position in the file handler with the help of file descriptor present in the file handler.







4) List of added new TestCases:

We have added 6 new test cases for the following functions:
i. AppendEmptyBlock
ii. WriteCurrentBlock
iii. WriteBlock
iv. ReadBlock
v. ReadLastBlock
vi. ReadPreviousBlock
vii. ReadCurrentBlock
viii. EnsureCapacity



