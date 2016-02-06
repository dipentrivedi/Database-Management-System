CC = gcc
CFLAG = -I -Wall


default : testAssign1


testAssign1: test_assign1_1.o storage_mgr.o dberror.o
		$(CC) $(CFLAG) -o testAssign1 test_assign1_1.o storage_mgr.o dberror.o -lm
		
testAssign2: test_assign1_2.o storage_mgr.o dberror.o
		$(CC) $(CFLAG) -o testAssign2 test_assign1_2.o storage_mgr.o dberror.o -lm		
  
test_assign1_1.o: test_assign1_1.c dberror.h storage_mgr.h test_helper.h
		$(CC) $(CFLAG) -c test_assign1_1.c -lm
		
test_assign1_2.o: test_assign1_2.c dberror.h storage_mgr.h test_helper.h
		$(CC) $(CFLAG) -c test_assign1_2.c -lm
		
storage_mgr.o: storage_mgr.c storage_mgr.h
		$(CC) $(CFLAG) -c storage_mgr.c -lm
		
dberror.o: dberror.c dberror.h
		$(CC) $(CFLAG) -c dberror.c
		
clean:	$(RM) testAssign1 testAssign2 *.o *~

run:
	./testAssign1
	
run2:
	./testAssign2
