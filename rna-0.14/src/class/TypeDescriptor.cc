/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/TypeDescriptor.cc,v $
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

TypeDescriptor * 
TypeDescriptor::Create (Memory * s, Rna name, Rna size, Rna format, Rna parts)
{
    TypeDescriptor * result = (TypeDescriptor *) Record::Create (s, _TypeInfo);

    result->set (IDX_NAME, name);
    result->set (IDX_SIZE, size);
    result->set (IDX_FORMAT, format);
    result->set (IDX_PARTS, parts);

    return result;
}

/******************************************************************\
 *                                                                *
 * TypeInfo                                                           *
 *                                                                *
\******************************************************************/

TypeDescriptor *
TypeDescriptor::TypeInfo ()
{
    return _TypeInfo;
}

TypeDescriptor * TypeDescriptor::_TypeInfo;


/******************************************************************\
 *                                                                *
 * Type Checking                                                  *
 *                                                                *
\******************************************************************/

TypeDescriptor *
TypeDescriptor::Convert (Record * record) 
{
    if (record->getTypeInfo () == _TypeInfo)
	return (TypeDescriptor *) record;
    else
	return 0;
}

/******************************************************************\
 *                                                                *
 * Access - Read                                                  *
 *                                                                *
\******************************************************************/

Rna
TypeDescriptor::get_name ()
{
    return get (IDX_NAME);
}

Rna
TypeDescriptor::get_size ()
{
    return get (IDX_SIZE);
}

Rna
TypeDescriptor::get_format ()
{
    return get (IDX_FORMAT);
}

Rna
TypeDescriptor::get_parts ()
{
    return get (IDX_PARTS);
}


/******************************************************************\
 *                                                                *
 * Access - Write                                                 *
 *                                                                *
\******************************************************************/

void TypeDescriptor::set_name (Rna name)
{
    set (IDX_NAME, name);
}

void TypeDescriptor::set_size (Rna size)
{
    set (IDX_SIZE, size);
}

void TypeDescriptor::set_format (Rna format)
{
    set (IDX_FORMAT, format);
}

void TypeDescriptor::set_parts (Rna parts)
{
    set (IDX_PARTS, parts);
}


/******************************************************************\
 *                                                                *
 * Class Initialization                                           *
 *                                                                *
\******************************************************************/

void
TypeDescriptor::Initialize ()
{
    Int length = SIZE;
    
    _TypeInfo = (TypeDescriptor *) Memory::Main ()->alloc
	(sizeof (Record) + length * sizeof (Rna));
    Array * parts = (Array *) 
	Memory::Main ()->alloc (sizeof (Array) + length * sizeof (Rna));
	
    _TypeInfo->_typeinfo = _TypeInfo;
    _TypeInfo->_attr = 0;
	
    _TypeInfo->_item [IDX_NAME].ss = "TypeDescriptor";
    _TypeInfo->_item [IDX_SIZE].ii = length;
    _TypeInfo->_item [IDX_FORMAT].ss = "sisB";
    _TypeInfo->_item [IDX_PARTS].array = parts;

    parts->_length = length;
    parts->_type = STR;

    parts->_item [0].ss = "name";
    parts->_item [1].ss = "size";
    parts->_item [2].ss = "format";
    parts->_item [3].ss = "parts";
}


/******************************************************************\
 *                                                                *
 * Name Lookup                                                    *
 *                                                                *
\******************************************************************/

Bool
TypeDescriptor::check ()
{
    int i, j;

    // check memory
    if (this == 0) {
	error ("consistency error: typedescriptor does not exist\n");
	return false;
    }

    // check name
    Str name = _item [IDX_NAME].ss;
    if (name == 0) {
	error ("consistency error: name is NULL\n");
	return false;
    }
    
    // check size
    Int size = _item [IDX_SIZE].ii;
    if (size < 0) {
	error ("consistency error: size<0: %d\n", size);
	return false;
    }

    // check format
    Str format = _item [IDX_FORMAT].ss;
    if (format == 0) {
	if (size != 0) {
	    error ("consistency error: format is NULL\n");
	    return false;
	}
    } else {
	if (strlen (format) != size) {
	    error ("consistency error: format length %d does not match size %d\n",
		   strlen (format),size);
	    return false;
	}
	for (i = 0; i < size; i++) {
	    Type t = (Type) format [i];
	    switch (t) {
		// @@@ TYPELIST @@@
	    case BOOL: 
	    case CHAR:
	    case INT:
	    case FLOAT:
	    case PTR:
	    case REF:
	    case STR:
	    case RECORD:
	    case ARRAY:
	    case LIST:
		break;
	    default:
		error ("consistency error: unknown type \'%c\' in format[%d]\n",
		       t, i);
		return false;
	    }
	}
    }

    // check parts
    Array * parts = _item [IDX_PARTS].array;
    if (parts == 0) {
	if (size != 0) {
	    error ("consistency error: parts is null\n");
	    return false;
	}
    } else {
	if (parts->_length != size) {
	    error ("consistency error: parts length %d does not match size %d\n",
		   parts->_length, size);
	    return false;
	}
	if (parts->_type != STR) {
	    error ("consistency error: parts type \'%c\' is not string\n",
		   parts->_type);
	    return false;
	}
	for (i = 0; i < size; i++) {
	    const Str part = parts->_item [i].ss;
	    if (part == 0) {
		error ("consistency error: part[%d] name is NULL\n", i);
		return false;
	    }
	    for (j = 0; j < i; j++)
		if (strcmp (part, parts->_item [j].ss) == 0) {
		    error ("consistency error: part[%d] and part[%d] have equal name \"%s\"\n", 
			   i, j, part);
		    return false;
		}
	}
    }

    return true;
}

Index 
TypeDescriptor::lookup (constStr name) const
{
    Size size = _item [IDX_SIZE].ii;
    
    for (int i = 0; i < size; i++)
	if (strcmp (name, _item [IDX_PARTS].array->_item [i].ss) == 0)
	    return i;

    return INDEX_NOT_FOUND;
}

Type 
TypeDescriptor::type (Index xx) const
{
    Size size = _item [IDX_SIZE].ii;

    RNA_ASSERT (0 <= xx && xx < size);

    return (Type) _item [IDX_FORMAT].ss [xx];

}
