
PRGMS =sqlite-shell test test1 test_bin
OBJS =shell.o sqlite3.o test.o test_bin.o test1.o

INCL =-I.

CC =gcc
#CC=/home/mj/tmp/lin26-bsp-d0801.R11/freescale-2011.03/bin/powerpc-linux-gnu-gcc

CFLAGS = 
#-Wall -g -fPIC

DEFS = 

LIBS =-lpthread -ldl



all:$(PRGMS)

sqlite-shell:shell.o sqlite3.o
	@echo 'Building target: $@'
	$(CC) $(LIBS) $(DEFS) $(INCL) $(CFLAGS) -o $@ shell.o sqlite3.o
	@echo 'Finished building target: $@'
	@echo ' '

test:sqlite3.o test.o
	@echo 'Building target: $@'
	$(CC) $(LIBS) $(DEFS) $(INCL) $(CFLAGS) -o $@ sqlite3.o test.o
	@echo 'Finished building target: $@'
	@echo ' '

test1:sqlite3.o test1.o
	@echo 'Building target: $@'
	$(CC) $(LIBS) $(DEFS) $(INCL) $(CFLAGS) -o $@ sqlite3.o test1.o
	@echo 'Finished building target: $@'
	@echo ' '
	
test_bin: sqlite3.o test_bin.o
	@echo 'Building target: $@'
	$(CC) $(LIBS) $(DEFS) $(INCL) $(CFLAGS) -o $@ sqlite3.o test_bin.o
	@echo 'Finished building target: $@'
	@echo ' '
	
%.o:%.c
	$(CC) $(LIBS) $(DEFS) $(INCL) $(CFLAGS) -c $< -o $@ 

.PHONY:clean
clean:
	-rm -f $(OBJS) $(PRGMS)


