/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Position.cc,v $
 * $Revision: 1.1.1.1 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

Position * 
Position::Create (Memory * s, Rna filename, Rna row, Rna column)
{
    Position * result = (Position *) Record::Create (s, _TypeInfo);

    result->set (IDX_FILENAME, filename);
    result->set (IDX_ROW, row);
    result->set (IDX_COLUMN, column);

    return result;
}


/******************************************************************\
 *                                                                *
 * TypeInfo                                                           *
 *                                                                *
\******************************************************************/

TypeDescriptor * Position::_TypeInfo;

TypeDescriptor *
Position::TypeInfo ()
{
    return _TypeInfo;
}


/******************************************************************\
 *                                                                *
 * Access - Read                                                  *
 *                                                                *
\******************************************************************/

Str Position::get_filename ()
{
    return get (IDX_FILENAME).readStr ();
}

Int Position::get_row ()
{
    return get (IDX_ROW).readInt ();
}

Int Position::get_column ()
{
    return get (IDX_COLUMN).readInt ();
}



/******************************************************************\
 *                                                                *
 * Access - Write                                                 *
 *                                                                *
\******************************************************************/

void 
Position::set_filename (Rna filename)
{
    set (IDX_FILENAME, filename);
}

void 
Position::set_row (Rna row)
{
    set (IDX_ROW, row);
}

void 
Position::set_column (Rna column)
{
    set (IDX_COLUMN, column);
}


/******************************************************************\
 *                                                                *
 * Position Singleton                                             *
 *                                                                *
\******************************************************************/

void
Position::Initialize ()
{
    // _TypeInfo = (Position 3 "sii" ["filename" "row" "column"])

    Int length = SIZE;
    
    _TypeInfo = (TypeDescriptor *) Memory::Main ()->alloc
	(sizeof (Record) + length * sizeof (Rna));
    Array * parts = (Array *) 
	Memory::Main ()->alloc (sizeof (Array) + length * sizeof (Rna));
    
    _TypeInfo->_typeinfo = TypeDescriptor::_TypeInfo;
    _TypeInfo->_attr = 0;
    
    _TypeInfo->_item [TypeDescriptor::IDX_NAME].ss = "Position";
    _TypeInfo->_item [TypeDescriptor::IDX_SIZE].ii = length;
    _TypeInfo->_item [TypeDescriptor::IDX_FORMAT].ss = "sii";
    _TypeInfo->_item [TypeDescriptor::IDX_PARTS].array = parts;
    
    parts->_length = length;
    parts->_type = STR;
    
    parts->_item [0].ss = "filename";
    parts->_item [1].ss = "row";
    parts->_item [2].ss = "column";
}

