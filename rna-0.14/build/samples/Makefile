# -*-Makefile-*-

#
# $Author: gordon $
# $Date: 2003/07/16 20:18:49 $
# $Source: /sys/cvs/rna/build/samples/Makefile,v $
# $Revision: 1.2 $
#

# Copyright (c) 1998 Gordon Cichon et al.
# Distributed under the GNU GPL.

include $(RNA_HOME)/build/Make.config

VPATH		= . $(RNA_HOME)/samples

CXXINCLUDE	= -I$(RNA_HOME)/include
CXXFLAGS	= -D_GNU_SOURCE -MD $(CXXINCLUDE) $(CXXDEBUG)

LDFLAGS		= -L$(RNA_HOME)/lib 
LDLIBS		= -lrna

sources		= \
	addrbook.cc

PROGS	= $(sources:%.cc=$(RNA_HOME)/samples/%)

all: $(PROGS) test

test: $(PROGS)
	cd $(RNA_HOME)/samples ;\
	for prog in $(PROGS) ; do \
		$$prog ;\
	done

$(PROGS): $(RNA_HOME)/lib/librna.a

$(RNA_HOME)/bin/%: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) -o $@ $< $(LOADLIBES) $(LDLIBS)

-include $(sources:.cc=.d)

realclean: clean
	@echo "       " rm $(PROGS)
	@for i in $(PROGS) ; do \
	    if [ -f $$i ] ; then \
		rm $$i ;\
	    fi \
	done

clean:
	@echo "       " rm *.o *.d *.a
	@for i in *.o *.d *.a ; do \
	    if [ -f $$i ] ; then \
		rm $$i ;\
	    fi \
	done


