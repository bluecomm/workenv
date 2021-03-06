#????????????????????????????????????
###############################################
#待实现目标：
#指定一个目标文件夹，创建层次目录，生成的目标文件的目录结构与源文件目录结构一致
################################################


#1. 使用环境
#GNU make 3.8.1或者以上版本
#sed
#gcc/binutils
#cygwin或者Linux

#2. 关键变量
#CROSS_COMPILE：交叉编译器前缀
#OPTIMIZE：关于优化的编译参数
#WARNINGS：关于warning的编译参数
#DEFS：    关于宏定义的编译参数
#EXTRA_CFLAGS：其它的编译参数
#$(OPTIMIZE) $(WARNINGS) $(DEFS) $(EXTRA_CFLAGS)共同构成了传给gcc的编译参数。

#INC_DIR：头文件目录
#SRC_DIR：源文件目录（当前目录.是被默认包含的）
#OBJ_DIR：object文件的输出目录
#EXTRA_SRC：源文件列表
#EXCLUDE_FILES：exclude文件列表
#
#SUFFIX：源文件的后缀名
#TARGET：最终的目标程序名
#TARGET_TYPE：目标程序的类型。
#可以定义的类型为：ar（static library），so（shared library）和app（application）。
#
#最终Makefile的行为是：
#在$(SRC_DIR)定义的每个目录中查找后缀为$(SUFFIX)的文件，并加上$(EXTRA_SRC)中的文件，然后排除掉
#$(EXCLUDE_FILES)中的文件，获得本工程定义的源文件列表。对于每一个源文件，编译生成的一个.o文件和一个.d文件
#（依赖文件），放在$(OBJ_DIR)目录下。最终生成的目标文件为$(TARGET)。
#此Makefile已经充分考虑到文件之间的依赖关系，即，如果某个头文件发生改变，当运行make的时候，所有依赖于它的
#源文件都将被重新编译。
#
#附件是一个例子，演示了如何处理不同目录，如何处理不同目标类型，以及如何exclude文件。
###############################################################################
#
# A smart Makefile template for GNU/LINUX programming
#
# Author: PRC (ijkxyz AT msn DOT com)
# Date:   2011/06/17
#
# Usage:
#   $ make           Compile and link (or archive)
#   $ make clean     Clean the objectives and target.
###############################################################################

CROSS_COMPILE =
OPTIMIZE := -O2
WARNINGS := -Wall -Wno-unused -Wno-format
DEFS     :=
EXTRA_CFLAGS := 

INC_DIR   = h
SRC_DIR   = sub1 sub2
OBJ_DIR   = obj
EXTRA_SRC =
EXCLUDE_FILES =

SUFFIX       = c
TARGET       := main
TARGET_TYPE  := app


#####################################################################################
#Do not change any part of them unless you have understood this script very well .This is a kind remind.                                                           ######################################################################################

#FUNC#  Add a new line to the input stream.
define add_newline
$1

endef

#FUNC# set the variable `src-x' according to the input $1
define set_src_x
src-$1 = $(filter-out $4,$(foreach d,$2,$(wildcard $d/*.$1)) $(filter %.$1,$3))

endef

#FUNC# set the variable `obj-x' according to the input $1
define set_obj_x
obj-$1 = $(patsubst %.$1,$3%.o,$(notdir $2))

endef

#VAR# Get the uniform representation of the object directory path name
ifneq ($(OBJ_DIR),)
prefix_objdir  = $(shell echo $(OBJ_DIR)|sed 's:\(\./*\)*::')
prefix_objdir := $(filter-out /,$(prefix_objdir)/)
endif

GCC      := $(CROSS_COMPILE)gcc
G++      := $(CROSS_COMPILE)g++
SRC_DIR := $(sort . $(SRC_DIR))
inc_dir = $(foreach d,$(sort $(INC_DIR)),-I$d)

#--# Do smart deduction automatically
$(eval $(foreach i,$(SUFFIX),$(call set_src_x,$i,$(SRC_DIR),$(EXTRA_SRC),$(EXCLUDE_FILES))))
$(eval $(foreach i,$(SUFFIX),$(call set_obj_x,$i,$(src-$i),$(prefix_objdir))))
$(eval $(foreach f,$(EXTRA_SRC),$(call add_newline,vpath $(notdir $f) $(dir $f))))
$(eval $(foreach d,$(SRC_DIR),$(foreach i,$(SUFFIX),$(call add_newline,vpath %.$i $d))))

all_objs = $(foreach i,$(SUFFIX),$(obj-$i))
all_srcs = $(foreach i,$(SUFFIX),$(src-$i))

CFLAGS       = $(EXTRA_CFLAGS) $(WARNINGS) $(OPTIMIZE) $(DEFS)
TARGET_TYPE := $(strip $(TARGET_TYPE))

ifeq ($(filter $(TARGET_TYPE),so ar app),)
$(error Unexpected TARGET_TYPE '$(TARGET_TYPE)')
endif

ifeq ($(TARGET_TYPE),so)
CFLAGS  += -fpic -shared
LDFLAGS += -shared
endif

PHONY = all .mkdir clean info

all: .mkdir $(TARGET)

define cmd_o
$(obj-$1): $2%.o: %.$1
	$(GCC) $(inc_dir) -Wp,-MT,$@ -Wp,-MMD,$@.d $(CFLAGS) -c -o $@ $<

endef

#call cms_o, c, obj
#$(obj-c): obj%.o: %.c
#	$(GCC) -Ih -Wp,-MT
#$(eval $(foreach i,$(SUFFIX),$(call cmd_o,$i,$(prefix_objdir))))
$(obj-c): obj/%.o: %.c
	$(GCC) $(inc_dir) -Wp,-MT,$@ -Wp,-MMD,$@.d $(CFLAGS) -c -o $@ $<

ifeq ($(TARGET_TYPE),ar)
$(TARGET): AR := $(CROSS_COMPILE)ar
$(TARGET): $(all_objs)
	rm -f $@
	$(AR) rcvs $@ $(all_objs)
else
$(TARGET): LD = $(if $(strip $(src-cpp) $(src-cc) $(src-cxx)),$(G++),$(GCC))
$(TARGET): $(all_objs)
	$(LD) $(inc_dir) $(LDFLAGS) $(all_objs) -o $@
endif

.mkdir:
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi

clean:
	rm -f $(prefix_objdir)*.o $(TARGET)

-include $(patsubst %.o,%.o.d,$(all_objs))

.PHONY: $(PHONY)

info:
	@echo 'TARGET: ' $(TARGET)
	@echo 'all_objs: ' $(all_objs)
	@echo 'all_srcs: ' $(all_srcs)
	@echo 'src-c: '    $(src-c)
	@echo 'obj-c: '    $(obj-c)
	@echo 'prefix_objdir: ' $(prefix_objdir)
