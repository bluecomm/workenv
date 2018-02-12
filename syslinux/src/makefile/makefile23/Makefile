###############################################################################
#多目录：只用一个Makefile文件(总控Makefile)，生成的目标文件存放在单独的目录下
#可以是多目标或单目标
###############################################################################

PRGM = main

SPATH += sub1 \
	sub2 \
	.

SRCS = $(foreach dir,$(SPATH),$(wildcard $(dir)/*.c))

INCL += -Ih



#OBJ_DIR = ./

#OBJ_PATH = $(foreach dir,$(SPATH),$(addprefix $(OBJ_DIR)/,$(dir)))
#OBJS = $(addprefix $(OBJ_DIR)/,$(subst .c,.o,$(SRCS)))

OBJ_PATH = $(SPATH)
OBJS = $(subst .c,.o,$(SRCS))


#MAKE_OBJ_DIR := $(shell mkdir -p $(OBJ_PATH))

CC = gcc

CFLAGS = -Wall -g -fPIC

DEFS =

all:$(PRGM)

$(PRGM):$(OBJS)
	@echo 'Building target: $@'
	$(CC) $(DEFS) $(INCL) $(CFLAGS) -o $@ $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

DEPS=$(OBJS:.o=.d)
-include $(DEPS)

%.o:%.c %.d
	$(CC) $(DEFS) $(INCL) $(CFLAGS) -c $< -o $@ 
	 
#%.d:%.c
#	set -e; rm -f $@; \%.o
#	$(CC) $(DEFS) $(CFLAGS) $(INCL) -MM $< >$@.$$$$; \
#	sed 's,.*\.o:,$(OBJ_DIR)/$*o $@ : ,g' < $@.$$$$ > $@; \
#	rm -f $@.$$$$

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) $(DEFS) $(CFLAGS) $(INCL) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
	
.PHONY:clean
clean:
	-rm $(DEPS) $(OBJS) $(PRGM)
