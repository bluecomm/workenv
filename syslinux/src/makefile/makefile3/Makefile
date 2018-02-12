###############################################################################
#多目录：每个源文件目录下有自己的Makefile
##########################################################################

SUBDIRS= subdir1 subdir2

.PHONY:default all clean $(SUBDIRS)

default:all

all clean:
# this 'make' cmd will cause the Makefile be read again by 'make'
	$(MAKE) $(SUBDIRS) TARGET=$@

$(SUBDIRS):print
	$(MAKE) -C $@ $(TARGET)
#
#            
#
print:
	@echo "--- MAKE  =$(MAKE)"
	@echo "--- TARGET=$(TARGET)"

