PRGM = main

CC = /home/mj/tmp/freescale-2011.03/bin/powerpc-linux-gnu-gcc
#CC = gcc
CPPFGLAGS = -Wall -static

SRC_DIR = .
SRCS = $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.c))
OBJS = $(subst .c,.o,$(SRCS))

INC_DIR = .
INCL = $(foreach d,$(sort $(INC_DIR)),-I$d)

all:$(PRGM)

$(PRGM):$(OBJS)
	$(CC) $(DEFS) $(INCL) $(CPPFGLAGS) -o $@ $(OBJS)

DEPS=$(OBJS:.o=.d)
-include $(DEPS)

%.o:%.c %.d
	$(CC) $(DEFS) $(INCL) $(CPPFGLAGS) -c $< -o $@ 
	
%.d:%.c
	@set -e; rm -f $@; \
	$(CC) $(DEFS) $(CPPFGLAGS) $(INCL) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


.PHONY:clean cleanall

clean:
	-rm $(DEPS) $(OBJS)

cleanall:
	-rm $(DEPS) $(OBJS) $(PRGM)

