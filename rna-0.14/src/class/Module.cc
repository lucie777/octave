/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Module.cc,v $
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

Module * 
Module::Create (Memory * s, Rna name, Rna contents)
{
    Module * result = (Module *) Record::Create (s, _TypeInfo);

    result->set (IDX_NAME, name);
    result->set (IDX_CONTENTS, contents);

    return result;
}

/******************************************************************\
 *                                                                *
 * TypeInfo                                                           *
 *                                                                *
\******************************************************************/

TypeDescriptor *
Module::TypeInfo ()
{
    return _TypeInfo;
}

TypeDescriptor * Module::_TypeInfo;


/******************************************************************\
 *                                                                *
 * Type Checking                                                  *
 *                                                                *
\******************************************************************/

Module *
Module::Convert (Record * record) 
{
    if (record->getTypeInfo () == _TypeInfo)
	return (Module *) record;
    else
	return 0;
}

/******************************************************************\
 *                                                                *
 * Access - Read                                                  *
 *                                                                *
\******************************************************************/

Rna
Module::get_name ()
{
    return get (IDX_NAME);
}

Rna
Module::get_contents ()
{
    return get (IDX_CONTENTS);
}


/******************************************************************\
 *                                                                *
 * Access - Write                                                 *
 *                                                                *
\******************************************************************/

void Module::set_name (Rna name)
{
    set (IDX_NAME, name);
}

void Module::set_contents (Rna contents)
{
    set (IDX_CONTENTS, contents);
}


/******************************************************************\
 *                                                                *
 * Class Initialization                                           *
 *                                                                *
\******************************************************************/

void
Module::Initialize ()
{
    Int length = SIZE;
    
    _TypeInfo = (TypeDescriptor *) Memory::Main ()->alloc
	(sizeof (Record) + TypeDescriptor::SIZE * sizeof (Rna));
    Array * parts = (Array *) 
	Memory::Main ()->alloc (sizeof (Array) + length * sizeof (Rna));
	
    _TypeInfo->_typeinfo = TypeDescriptor::_TypeInfo;
    _TypeInfo->_attr = 0;
	
    _TypeInfo->_item [TypeDescriptor::IDX_NAME].ss = "Module";
    _TypeInfo->_item [TypeDescriptor::IDX_SIZE].ii = length;
    _TypeInfo->_item [TypeDescriptor::IDX_FORMAT].ss = "sB";
    _TypeInfo->_item [TypeDescriptor::IDX_PARTS].array = parts;

    parts->_length = length;
    parts->_type = STR;

    parts->_item [0].ss = "name";
    parts->_item [1].ss = "contents";
}

