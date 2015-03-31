/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Record.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <stdarg.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

Record *
Record::CreateVarg (Memory * s, TypeDescriptor * typeinfo, List * attr, Varg dummy, ...)
{
    Record * result;

    Index size = typeinfo->_item[TypeDescriptor::IDX_SIZE].ii;
    result = (Record *) s->alloc
	(sizeof (Record) + size * sizeof (Rna));
    
    result->_typeinfo = typeinfo;
    result->_attr = attr;

    va_list ap;
    va_start (ap, dummy);
    for (int i = 0; i < size; i++)
	result->_item [i].varg = va_arg (ap, Varg);
    va_end (ap);

    return result;
}

Record *
Record::vCreate (Memory * s, TypeDescriptor * typeinfo, List * attr, const Varg * items)
{
    Record * result;

    Index size = typeinfo->_item[TypeDescriptor::IDX_SIZE].ii;
    result = (Record *) s->alloc
	(sizeof (Record) + size * sizeof (Rna));
    
    result->_typeinfo = typeinfo;
    result->_attr = attr;

    for (int i = 0; i < size; i++)
	result->_item [i].varg = items [i];

    return result;
}


/******************************************************************\
 *                                                                *
 * Error Handling                                                 *
 *                                                                *
\******************************************************************/

void 
Record::error (constStr format, ...)
{
    constStr file;
    Int line, column;
    
    List * annotation = _attr->find ("position");
    if (annotation) {
	Record * position = annotation->_car.readRecord ();
	
	file = position->_item[Position::IDX_FILENAME].ss;
	line = position->_item[Position::IDX_ROW].ii;
	column = position->_item[Position::IDX_COLUMN].ii;
    } else {
	file = _typeinfo->_item [TypeDescriptor::IDX_NAME].ss;
	line = INDEX_NOT_FOUND;
	column = INDEX_NOT_FOUND;
    }
    
    va_list ap;
    va_start (ap, format);
    Error::Instance ()->Message (file, line, column, format, ap);
    va_end (ap);
}

