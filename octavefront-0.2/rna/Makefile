### We should really include this into the main configuration run ...

TOPDIR		=	..

#include $(TOPDIR)/Makeconf

GENCLASS    =   genclass

RNA_INCLUDE =	-i tree.rna -i type.rna -i value.rna

RNA_DIRS    =   build include lib src include/class include/inline
RNA_FILES	=	tree type value

RNA_LIBS	=	$(RNA_FILES:%=lib/lib%.a)

MAKEFILES	=	$(RNA_FILES:%=build/Makefile.%)


all:	$(RNA_DIRS) $(RNA_LIBS)

#$(MAKEFILES):	$(RNA_DIRS)
$(RNA_LIBS):	$(MAKEFILES)

$(RNA_DIRS):
	mkdir -p $@


.PHONY:	clean

clean:
	@for dir in $(RNA_DIRS); do \
		rm -rf $$dir ;\
	done

build/Makefile.%: %.rna
	cd build ;\
	RNA_DIR=.. $(GENCLASS) $(RNA_FILES:%=-i ../%.rna) -v ../$< ;\
	cd ..

lib/lib%.a: build/Makefile.%
	RNA_DIR=.. RNAFLAGS=-I$(RNA_HOME)/include \
	$(MAKE) -C build -f ../$< ../$@
