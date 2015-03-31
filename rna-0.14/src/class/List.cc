/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/List.cc,v $
 * $Revision: 1.1.1.1 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Linked List                                                    *
 *                                                                *
\******************************************************************/

List *
List::find (Str tag)
{
    List * ref;
    for (ref = this; ref; ref = ref->_cdr)
	if (strcmp (tag, ref->_tag) == 0) 
	    return ref;
    return 0;
}

void
List::add (List * item, List * & list)
{
    item->_cdr = list;
    list = item;
}

List *
List::remove (Str tag, List * & list)
{
    List * * ref;
    for (ref = & list; *ref; ref = & (*ref)->_cdr)
	if (strcmp (tag, (*ref)->_tag) == 0) {
	    List * result = *ref;
	    *ref = (*ref)->_cdr;
	    result->_cdr = 0;
	    return result;
	}
    return 0;
}

void
List::walk (Bool (* function) (List * item,
				Rna & user_data),
	     Rna & user_data)
{
    for (List * s = this; s; s = s->_cdr)
	if (!function (s, user_data))
	    return;
}
