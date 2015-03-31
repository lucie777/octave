/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 14:47:55 $
 * $Source: /sys/cvs/rna/include/rna/inline/Rna.inline.cc,v $
 * $Revision: 1.2 $
 */

#ifndef RNA_INSIDE_HEADER
# define RNA_IMPLEMENTATION
# include "rna/rna.h"
#endif /* RNA_INSIDE_HEADER */

#ifdef RNA_DEFINE_INLINE_FUNCTIONS_NOW

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

RNA_INLINE 
Rna::Rna ()
# ifdef RNA_TYPECHECK
    : _type (ANY)
# endif /* RNA_TYPECHECK */
{
# ifdef RNA_DEBUG
    _word.ii = 0x42424242;
# endif
}

RNA_INLINE 
Rna::Rna (const Rna & rna)
    :_word (rna._word)
# ifdef RNA_TYPECHECK
     , _type (rna._type)
# endif /* RNA_TYPECHECK */
{}

RNA_INLINE Rna 
Rna::operator = (const Rna & rna)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == rna._type);
    _type = rna._type;
# endif /* RNA_TYPECHECK */

    _word = rna._word;
    RNA_USE(_word);
    
    return SELF;
}

RNA_INLINE 
Rna::~Rna ()
{
    RNA_USE(_word);
# ifdef RNA_TYPECHECK
    _type = STOP;
# endif /* RNA_TYPECHECK */
# ifdef RNA_DEBUG
    _word.ii = 0x42424242;
# endif
}

#ifndef RNA_TYPECHECK
Rna::Rna (Word word)
    :_word (word)
{}
#else /* RNA_TYPECHECK */
Rna::Rna (Word word, Type type)
    :_word (word),
     _type (type)
{}
#endif /* RNA_TYPECHECK */


/******************************************************************\
 *                                                                *
 * Instance Creation                             @@@ TYPELIST @@@ *
 *                                                                *
\******************************************************************/

Rna::Rna (Bool bb)
# ifdef RNA_TYPECHECK
    :_type (BOOL)
# endif /* RNA_TYPECHECK */
{
    _word.bb = bb;
}

Rna::Rna (Char cc)
# ifdef RNA_TYPECHECK
    :_type (CHAR)
# endif /* RNA_TYPECHECK */
{
    _word.cc = cc;
}

Rna::Rna (Int ii)
# ifdef RNA_TYPECHECK
    :_type (INT)
# endif /* RNA_TYPECHECK */
{
    _word.ii = ii;
}

Rna::Rna (Float ff)
# ifdef RNA_TYPECHECK
    :_type (FLOAT)
# endif /* RNA_TYPECHECK */
{
    _word.ff = ff;
}

Rna::Rna (Ptr pp)
# ifdef RNA_TYPECHECK
    :_type (PTR)
# endif /* RNA_TYPECHECK */
{
    _word.pp = pp;
}

Rna::Rna (Ref rr)
# ifdef RNA_TYPECHECK
    :_type (REF)
# endif /* RNA_TYPECHECK */
{
    _word.rr = rr;
}

Rna::Rna (Str ss)
# ifdef RNA_TYPECHECK
    :_type (STR)
# endif /* RNA_TYPECHECK */
{
    _word.ss = ss;
    RNA_USE(_word);
}

Rna::Rna (Record * record)
# ifdef RNA_TYPECHECK
    :_type (RECORD)
# endif /* RNA_TYPECHECK */
{
    _word.record = record;
    RNA_USE(_word);
}

Rna::Rna (Array * array)
# ifdef RNA_TYPECHECK
    :_type (ARRAY)
# endif /* RNA_TYPECHECK */
{
    _word.array = array;
    RNA_USE(_word);
}

Rna::Rna (List * list)
# ifdef RNA_TYPECHECK
    :_type (LIST)
# endif /* RNA_TYPECHECK */
{
    _word.list = list;
    RNA_USE(_word);
}


/******************************************************************\
 *                                                                *
 * Acess - Read                                  @@@ TYPELIST @@@ *
 *                                                                *
\******************************************************************/

RNA_INLINE Bool
Rna::readBool ()
{
# ifdef RNA_TYPECHECK
    if (_type == ANY)
	_type = BOOL;
    RNA_ASSERT (_type == BOOL);
# endif /* RNA_TYPECHECK */

    return _word.bb;
}

RNA_INLINE Char
Rna::readChar ()
{
# ifdef RNA_TYPECHECK
    if (_type == ANY)
	_type = CHAR;
    RNA_ASSERT (_type == CHAR);
# endif /* RNA_TYPECHECK */

    return _word.cc;
}

RNA_INLINE Int
Rna::readInt ()
{
# ifdef RNA_TYPECHECK
    if (_type == ANY)
	_type = INT;
    RNA_ASSERT (_type == INT);
# endif /* RNA_TYPECHECK */

    return _word.ii;
}

RNA_INLINE Float
Rna::readFloat ()
{
# ifdef RNA_TYPECHECK
    if (_type == ANY)
	_type = FLOAT;
    RNA_ASSERT (_type == FLOAT);
# endif /* RNA_TYPECHECK */

    return _word.ff;
}

RNA_INLINE Ptr
Rna::readPtr ()
{
# ifdef RNA_TYPECHECK
    if (_type == ANY)
	_type = PTR;
    RNA_ASSERT (_type == PTR);
# endif /* RNA_TYPECHECK */

    return _word.pp;
}

RNA_INLINE Ref
Rna::readRef ()
{
# ifdef RNA_TYPECHECK
    if (_type == ANY)
	_type = REF;
    RNA_ASSERT (_type == REF);
# endif /* RNA_TYPECHECK */

    return _word.rr;
}

RNA_INLINE Str
Rna::readStr ()
{
# ifdef RNA_TYPECHECK
    if (_type == ANY)
	_type = STR;
    RNA_ASSERT (_type == STR);
# endif /* RNA_TYPECHECK */

    return _word.ss;
}

RNA_INLINE Record *
Rna::readRecord ()
{
# ifdef RNA_TYPECHECK
    if (_type == ANY)
	_type = RECORD;
    RNA_ASSERT (_type == RECORD);
# endif /* RNA_TYPECHECK */

    RNA_USE(_word);
    return _word.record;
}

RNA_INLINE Array *
Rna::readArray ()
{
# ifdef RNA_TYPECHECK
    if (_type == ANY)
	_type = ARRAY;
    RNA_ASSERT (_type == ARRAY);
# endif /* RNA_TYPECHECK */

    RNA_USE(_word);
    return _word.array;
}

RNA_INLINE List *
Rna::readList ()
{
# ifdef RNA_TYPECHECK
    if (_type == ANY)
	_type = LIST;
    RNA_ASSERT (_type == LIST);
# endif /* RNA_TYPECHECK */

    RNA_USE(_word);
    return _word.list;
}


/******************************************************************\
 *                                                                *
 * Type Checking                                                  *
 *                                                                *
\******************************************************************/

RNA_INLINE Word 
Rna::getWord () const
{
    RNA_USE(_word);
    return _word;
}


/******************************************************************\
 *                                                                *
 * Acess - Write                                 @@@ TYPELIST @@@ *
 *                                                                *
\******************************************************************/

RNA_INLINE void
Rna::writeBool (Bool bb)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == BOOL);
    _type = BOOL;
# endif /* RNA_TYPECHECK */

    _word.bb = bb;
}

RNA_INLINE void
Rna::writeChar (Char cc)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == CHAR);
    _type = CHAR;
# endif /* RNA_TYPECHECK */

    _word.cc = cc;
}

RNA_INLINE void
Rna::writeInt (Int ii)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == INT);
    _type = INT;
# endif /* RNA_TYPECHECK */

    _word.ii = ii;
}

RNA_INLINE void
Rna::writeFloat (Float ff)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == FLOAT);
    _type = FLOAT;
# endif /* RNA_TYPECHECK */

    _word.ff = ff;
}

RNA_INLINE void
Rna::writePtr (Ptr pp)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == PTR);
    _type = PTR;
# endif /* RNA_TYPECHECK */

    _word.pp = pp;
}

RNA_INLINE void
Rna::writeRef (Ref rr)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == REF);
    _type = REF;
# endif /* RNA_TYPECHECK */

    _word.rr = rr;
}

RNA_INLINE void
Rna::writeStr (Str ss)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == STR);
    _type = STR;
# endif /* RNA_TYPECHECK */

    _word.ss = ss;
    RNA_USE(_word);
}

RNA_INLINE void
Rna::writeRecord (Record * record)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == RECORD);
    _type = RECORD;
# endif /* RNA_TYPECHECK */

    _word.record = record;
    RNA_USE(_word);
}

RNA_INLINE void
Rna::writeArray (Array * array)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == ARRAY);
    _type = ARRAY;
# endif /* RNA_TYPECHECK */

    _word.array = array;
    RNA_USE(_word);
}

RNA_INLINE void
Rna::writeList (List * list)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY || _type == LIST);
    _type = LIST;
# endif /* RNA_TYPECHECK */

    _word.list = list;
    RNA_USE(_word);
}


/******************************************************************\
 *                                                                *
 * Comparing                                                      *
 *                                                                *
\******************************************************************/

RNA_INLINE Bool
Rna::operator == (Rna r)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == r._type || 
		_word.ii == 0 || r._word.ii == 0);
# endif /* RNA_TYPECHECK */

    RNA_USE(_word);
    RNA_USE(r._word);
    return _word.ii == r._word.ii;
}

RNA_INLINE Bool
Rna::operator != (Rna r)
{
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == r._type || 
		_word.ii == 0 || r._word.ii == 0);
# endif /* RNA_TYPECHECK */

    RNA_USE(_word);
    RNA_USE(r._word);
    return _word.ii != r._word.ii;
}


/******************************************************************\
 *                                                                *
 * Null Singleton                                                 *
 *                                                                *
\******************************************************************/

RNA_INLINE Rna
Rna::Null ()
{
    return _Null;
}

RNA_INLINE Bool
Rna::isNull () const
{
    return _word.record == 0;
}


#endif /* RNA_DEFINE_INLINE_FUNCTIONS_NOW */
