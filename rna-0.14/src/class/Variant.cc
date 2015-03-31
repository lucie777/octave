/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Variant.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <stdlib.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

Variant * 
Variant::Create (Memory * s, Rna name, Rna choices)
{
    Variant * result = (Variant *) Record::Create (s, _TypeInfo);

    result->set (IDX_NAME, name);
    result->set (IDX_CHOICES, choices);

    return result;
}

/******************************************************************\
 *                                                                *
 * TypeInfo                                                           *
 *                                                                *
\******************************************************************/

TypeDescriptor *
Variant::TypeInfo ()
{
    return _TypeInfo;
}

TypeDescriptor * Variant::_TypeInfo;


/******************************************************************\
 *                                                                *
 * Type Checking                                                  *
 *                                                                *
\******************************************************************/

Variant *
Variant::Convert (Record * record) 
{
    if (record->getTypeInfo () == _TypeInfo)
	return (Variant *) record;
    else
	return 0;
}

/******************************************************************\
 *                                                                *
 * Access - Read                                                  *
 *                                                                *
\******************************************************************/

Rna
Variant::get_name ()
{
    return get (IDX_NAME);
}

Rna
Variant::get_choices ()
{
    return get (IDX_CHOICES);
}


/******************************************************************\
 *                                                                *
 * Access - Write                                                 *
 *                                                                *
\******************************************************************/

void Variant::set_name (Rna name)
{
    set (IDX_NAME, name);
}

void Variant::set_choices (Rna choices)
{
    set (IDX_CHOICES, choices);
}


/******************************************************************\
 *                                                                *
 * Class Initialization                                           *
 *                                                                *
\******************************************************************/

void
Variant::Initialize ()
{
    Int length = SIZE;
    
    _TypeInfo = (TypeDescriptor *) Memory::Main ()->alloc
	(sizeof (Record) + TypeDescriptor::SIZE * sizeof (Rna));
    Array * parts = (Array *) 
	Memory::Main ()->alloc (sizeof (Array) + length * sizeof (Rna));
	
    _TypeInfo->_typeinfo = TypeDescriptor::_TypeInfo;
    _TypeInfo->_attr = 0;
	
    _TypeInfo->_item [TypeDescriptor::IDX_NAME].ss = "Variant";
    _TypeInfo->_item [TypeDescriptor::IDX_SIZE].ii = length;
    _TypeInfo->_item [TypeDescriptor::IDX_FORMAT].ss = "sB";
    _TypeInfo->_item [TypeDescriptor::IDX_PARTS].array = parts;

    parts->_length = length;
    parts->_type = STR;

    parts->_item [0].ss = "name";
    parts->_item [1].ss = "choices";
}

