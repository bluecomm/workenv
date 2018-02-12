###############################################################################
#多目录：只用一个Makefile文件(总控Makefile)，生成的目标文件存放在单独的目录下
#可以是多目标或单目标
###############################################################################

PRGM = main

SPATH += sub1 \
	sub2 \
	.

SRCS = $(foreach dir,$(SPATH),$(wildcard $(dir)/*.c))

INCL += -I./h
	   
OBJ_DIR = ./bin

#OBJ_PATH = $(foreach dir,$(subst .,,$(SPATH)),$(addprefix $(OBJ_DIR)/,$(dir)))
#OBJS = $(subst ..,,$(addprefix $(OBJ_DIR),$(subst .c,.o,$(SRCS))))

OBJ_PATH = $(foreach dir,$(SPATH),$(addprefix $(OBJ_DIR)/,$(dir)))
OBJS = $(addprefix $(OBJ_DIR)/,$(subst .c,.o,$(SRCS)))

MAKE_OBJ_DIR := $(shell mkdir -p $(OBJ_PATH))

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

$(OBJ_DIR)/%.o:%.c $(OBJ_DIR)/%.d
	$(CC) $(DEFS) $(INCL) $(CFLAGS) -c $< -o $@ 
	 
$(OBJ_DIR)/%.d:%.c
	set -e; rm -f $@; \
	$(CC) $(DEFS) $(CFLAGS) $(INCL) -MM $< >$@.$$$$; \
	sed 's,.*\.o:,$(OBJ_DIR)/$*o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY:clean
clean:
	rm -rf $(OBJ_DIR) $(PRGM)
