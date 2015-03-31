# -*-Makefile-*-

#
# $Author: cichon $
# $Date: 2003/07/08 07:30:45 $
# $Source: /sys/cvs/rna/build/bin/Makefile,v $
# $Revision: 1.2 $
#

# Copyright (c) 1998 Gordon Cichon et al.
# Distributed under the GNU GPL.

include $(RNA_HOME)/build/Make.config

VPATH		= $(RNA_HOME)/src/bin

CXXINCLUDE	= -I$(RNA_HOME)/include
CXXFLAGS	= -D_GNU_SOURCE -MD $(CXXINCLUDE) $(CXXDEBUG)

LDFLAGS		= -L$(RNA_HOME)/lib 
LDLIBS		= -lrna

sources		= \
	genclass.cc \
	lstd.cc \

PROGS	= $(sources:%.cc=$(RNA_HOME)/bin/%)

all: $(PROGS)

$(PROGS): $(RNA_HOME)/lib/librna.a

$(RNA_HOME)/bin/genclass: config.o emit.o naming.o

$(RNA_HOME)/bin/rna2at: rna2at.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) -o $@ $< -lATerm-dbg $(LOADLIBES) $(LDLIBS)

$(RNA_HOME)/bin/%: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) -o $@ $^ $(LOADLIBES) $(LDLIBS)

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


