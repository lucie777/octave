/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 12:36:48 $
 * $Source: /sys/cvs/rna/src/class/StrContext.cc,v $
 * $Revision: 1.2 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

StrContext::StrContext ()
    : Context ()
{
}

StrContext::StrContext (Context * parent)
    : Context (parent)
{
}

StrContext::~StrContext ()
{
}


/******************************************************************\
 *                                                                *
 * Name Table                                                     *
 *                                                                *
\******************************************************************/

Array * StrContext::_Table = 0;

void
StrContext::SetTable (Rna rna)
{
    _Table = rna.readArray ();

    if (!rna.isNull ()) {
	RNA_ASSERT (_Table->getLength () >= 2);
	RNA_ASSERT (_Table->getType () == STR);
    }
}

/******************************************************************\
 *                                                                *
 * Handle Management                                              *
 *                                                                *
\******************************************************************/

Type
StrContext::handle_type () const
{
    return STR;
}

Rna
StrContext::create_handle (Word word) const
{
    if (_Table) {
	_bindingspace->start ();

	Int size = _Table->getLength ();
	for (int i = _Serial ++; i > 0; i /= size) {
	    char * s = _Table->at (i % size).readStr ();
	    
	    _bindingspace->grow (s, strlen (s));
	}

	return (Str) _bindingspace->finish0 ();
    } else {
	char * buffer = (char *) 
	    _bindingspace->alloc (1+sizeof(Rna)*2+1);
	sprintf (buffer, "L%lx", word.ii);
	return Rna (buffer);
    }
}

Bool
StrContext::compare_handle (Rna handle1, Rna handle2) const
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (handle1._type == STR && handle2._type == STR);
# endif /* !RNA_TYPECHECK */

    return strcmp (handle1._word.ss, handle2._word.ss) == 0;
}

Rna
StrContext::copy_handle (Rna handle) const
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (handle._type == STR);
# endif /* RNA_TYPECHECK */

    return Rna (_bindingspace->alloc0 (handle._word.ss));
}


/******************************************************************\
 *                                                                *
 * System StrContext Singleton                                  *
 *                                                                *
\******************************************************************/

StrContext * StrContext::_System;

StrContext *
StrContext::System ()
{
    return _System;
}


/******************************************************************\
 *                                                                *
 * Class Initialization                                           *
 *                                                                *
\******************************************************************/

Int StrContext::_Serial;

void
StrContext::Initialize ()
{
    _System = new StrContext (0);

    _System->enter ("TypeDescriptor", TypeDescriptor::TypeInfo (), RECORD);
    _System->enter ("Empty", Empty::TypeInfo (), RECORD);
    _System->enter ("Variant", Variant::TypeInfo (), RECORD);
    _System->enter ("Module", Module::TypeInfo (), RECORD);
    _System->enter ("Position", Position::TypeInfo (), RECORD);
    _System->enter ("null", Rna::Null (), ANY);

    _System->enter ("true", true, BOOL);
    _System->enter ("false", false, BOOL);
   
    _System->enter ("nil", (RNA::Record *) 0, RECORD);

    // Label Creation
    _Serial = 1;
    
    // Name Table
    _Table = Array::Create (Memory::Main (), 12, STR);
    static char * table_names [] = {
	"gordon", "dido", "pix", "kalinka",

	"adriane", "nicole", "birgit", "barbara", "marianne", 
	"samira", "sandra", "susanne"
    };
    for (int i = 0; i < 12; i++)
	_Table->atPut (i, table_names [i]);
}

/******************************************************************\
 *                                                                *
 * Testing                                                        *
 *                                                                *
\******************************************************************/

StrContext * 
StrContext::strcontext ()
{
    return this;
}
