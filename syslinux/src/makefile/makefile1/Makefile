###############################################################################
#最基本的makefile：单一层次目录，多目标，简单规则
###############################################################################

PRGMS = test1 test2 test3

INCL = 

CC = gcc

CFLAGS = -Wall -g -fPIC

DEFS = 

LIBS =

all:$(PRGMS)

test1:test1.o
	$(CC) $(LIBS) $(DEFS) $(INCL) $(CFLAGS) -o $@ $<
	@echo ' '

test2:test2.o
	$(CC) $(LIBS) $(DEFS) $(INCL) $(CFLAGS) -o $@ $<
	@echo ' '
	
test3:test3.o
	$(CC) $(LIBS) $(DEFS) $(INCL) $(CFLAGS) -o $@ $<
	@echo ' '

%.o:%.c
	$(CC) $(LIBS) $(DEFS) $(INCL) $(CFLAGS) -c $< -o $@ 

.PHONY:clean
clean:
	-rm -rf *.o $(PRGMS)


