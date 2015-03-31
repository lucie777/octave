/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Array.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <stdarg.h>
#include <string.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

Array *
Array::CreateVargs (Memory * s, Index length, Type type, Varg dummy, ...)
{
    Array * result;
    
    result = (Array *) s->alloc
	(sizeof (Array) + length * sizeof (Word));

    result->_length = length;
    result->_type = type;
    
    va_list ap;
    va_start (ap, dummy);
    for (int i = 0; i < length; i++)
	result->_item [i].varg = va_arg (ap, Varg);
    va_end (ap);

    return result;
}

Array *
Array::vCreate (Memory * s, Index length, Type type, Ref items)
{
    Array * result;
    
    result = (Array *) s->alloc
	(sizeof (Array) + length * sizeof (Word));

    result->_length = length;
    result->_type = type;
    
    for (int i = 0; i < length; i++) {
# ifdef RNA_TYPECHECK
	RNA_ASSERT (items [i]._type == type);
# endif /* RNA_TYPECHECK */
	result->_item [i] = items [i]._word;
    }

    return result;
}


