# -*-Makefile-*-

# Copyright (c) 1998 Gordon Cichon et al.
# Distributed under the GNU GPL.

#
# $Author: gordon $
# $Date: 2002/09/01 18:36:13 $
# $Source: /sys/cvs/rna/build/lib/Makefile,v $
# $Revision: 1.1.1.1 $
#

include $(RNA_HOME)/build/Make.config

VPATH		= $(RNA_HOME)/src/lib \
		$(RNA_HOME)/include/rna/inline \
		$(RNA_HOME)/src/class \
		$(RNA_HOME)/src/record

CXXINCLUDE	= -I$(RNA_HOME)/include
CXXFLAGS	= -D_GNU_SOURCE -MD $(CXXINCLUDE) $(CXXDEBUG)

AR		= ar
ARFLAGS		= rv

# @@@ CLASSLIST @@@
sources		= \
	TypeDescriptor.cc \
	Record.cc\
	AsciiSerializer.cc \
	Array.cc \
	BinarySerializer.cc \
	CompressedStream.cc \
	Empty.cc \
	Error.cc \
	File.cc \
	List.cc \
	Initialization.cc \
	IntContext.cc \
	StrContext.cc \
	Position.cc \
	Rna.cc \
	Socket.cc \
	Memory.cc \
	Module.cc \
	Stream.cc \
	Context.cc \
	Serializer.cc \
	Variant.cc \
	Venue.cc \
	\
	Rna.inline.cc \
	Memory.inline.cc \
	Record.inline.cc \
	Array.inline.cc \
	List.inline.cc \
	Venue.inline.cc \
	\
	copy.cc \
	debug.cc \
	init.cc \
	read-a.cc \
	read-b.cc \
	write-a.cc \
	write-b.cc \
	\
	clone.inline.cc \
	sugar.inline.cc

OBJS		= $(sources:.cc=.o)

LIB		= $(RNA_HOME)/lib/librna.a

all: $(LIB)

$(LIB): $(OBJS)
	@if [ -f $(LIB) ] ; then \
		rm $(LIB) ;\
	fi
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)

-include $(sources:.cc=.d)

realclean: clean
	@echo "       " rm $(LIB)
	@for i in $(LIB) ; do \
	    if [ -f $$i ] ; then \
		rm $$i ;\
	    fi \
	done

clean:
	@echo "       " rm *.o *.d
	@for i in *.o *.d ; do \
	    if [ -f $$i ] ; then \
		rm $$i ;\
	    fi \
	done


