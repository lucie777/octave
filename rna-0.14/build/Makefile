# -*-Makefile-*-

# Copyright (c) 1998 Gordon Cichon et al.
# Distributed under the GNU GPL.

#
# $Author: gordon $
# $Date: 2002/09/01 18:36:13 $
# $Source: /sys/cvs/rna/build/Makefile,v $
# $Revision: 1.1.1.1 $
#

include $(RNA_HOME)/build/Make.config

DIRS	= lib bin samples

all:
	@for i in $(DIRS) ; do \
		echo make -C $$i $@ ;\
		make -C $$i $@ ;\
		if [ $$? -ne 0 ] ; then \
			exit $$? ;\
		fi ;\
	done

clean:
	@for i in $(DIRS) ; do \
		echo make -C $$i $@ ;\
		make -C $$i $@ ;\
	done

realclean:
	@for i in $(DIRS) ; do \
		echo make -C $$i $@ ;\
		make -C $$i $@ ;\
	done
