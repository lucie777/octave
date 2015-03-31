/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Empty.cc,v $
 * $Revision: 1.1.1.1 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

Empty *
Empty::Create (Memory * s)
{
    return (Empty *) Record::Create (s, _TypeInfo);
}

/******************************************************************\
 *                                                                *
 * TypeInfo                                                           *
 *                                                                *
\******************************************************************/

TypeDescriptor * Empty::_TypeInfo;

TypeDescriptor *
Empty::TypeInfo ()
{
    return _TypeInfo;
}

/******************************************************************\
 *                                                                *
 * Access                                                         *
 *                                                                *
\******************************************************************/

/******************************************************************\
 *                                                                *
 * Class Initialization                                           *
 *                                                                *
\******************************************************************/

void
Empty::Initialize ()
{
    // _TypeInfo = (TypeDescriptor 0 "" [])

    Int length = SIZE;
    
    _TypeInfo = (TypeDescriptor *) Memory::Main ()->alloc
	(sizeof (Record) + TypeDescriptor::SIZE * sizeof (Rna));
    Array * parts = (Array *) 
	Memory::Main ()->alloc (sizeof (Array) + length * sizeof (Rna));
    
    _TypeInfo->_typeinfo = TypeDescriptor::TypeInfo ();
    _TypeInfo->_attr = 0;
    
    _TypeInfo->_item [TypeDescriptor::IDX_NAME].ss = "Empty";
    _TypeInfo->_item [TypeDescriptor::IDX_SIZE].ii = length;
    _TypeInfo->_item [TypeDescriptor::IDX_FORMAT].ss = "";
    _TypeInfo->_item [TypeDescriptor::IDX_PARTS].array = parts;
    
    parts->_length = length;
    parts->_type = STR;
}

