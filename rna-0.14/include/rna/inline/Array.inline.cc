/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: cichon $
 * $Date: 2003/07/13 13:34:59 $
 * $Source: /sys/cvs/rna/include/rna/inline/Array.inline.cc,v $
 * $Revision: 1.3 $
 */

#ifndef RNA_INSIDE_HEADER
# define RNA_IMPLEMENTATION
# include "rna/rna.h"
#endif /* RNA_INSIDE_HEADER */

#ifdef RNA_DEFINE_INLINE_FUNCTIONS_NOW

/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

RNA_INLINE Array *
Array::Create (Memory * s, Index length, Type type)
{
    Array * result;
    
    result = (Array *) s->alloc
	(sizeof (Array) + length * sizeof (Word));

    result->_length = length;
    result->_type = type;
    
    return result;
}

/******************************************************************\
 *                                                                *
 * Access - Length                                                *
 *                                                                *
\******************************************************************/

RNA_INLINE Index 
Array::getLength ()
{
    if (this == 0)
	return 0;
    return _length;
}

RNA_INLINE void
Array::setLength (Index length)
{
    _length = length;
}


/******************************************************************\
 *                                                                *
 * Access - Type                                                  *
 *                                                                *
\******************************************************************/

RNA_INLINE Type
Array::getType ()
{
    if (this == 0)
	return STOP;
    return _type;
}

RNA_INLINE void
Array::setType (Type type)
{
    _type = type;
}


/******************************************************************\
 *                                                                *
 * Access - main                                                  *
 *                                                                *
\******************************************************************/

RNA_INLINE Rna
Array::at (Index xx)
{
    RNA_ASSERT (0 <= xx && xx < _length);

# ifndef RNA_TYPECHECK    
    return Rna (_item [xx]);
# else /* RNA_TYPECHECK */
    return Rna (_item [xx], _type);
# endif /* RNA_TYPECHECK */
}

RNA_INLINE void
Array::atPut (Index xx, Rna value)
{
    RNA_ASSERT (0 <= xx && xx < _length);
    
# ifdef RNA_TYPECHECK
    RNA_ASSERT (_type == ANY
		|| _type == value._type
		|| ((_type == RECORD || _type == ARRAY)
		    && value._type == ANY 
		    && value._word.record == 0));
# endif /* RNA_TYPECHECK */
    
    _item [xx] = value._word;
}


#endif /* RNA_DEFINE_INLINE_FUNCTIONS_NOW */
