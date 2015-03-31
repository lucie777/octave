/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/12 14:37:09 $
 * $Source: /sys/cvs/rna/include/rna/inline/List.inline.cc,v $
 * $Revision: 1.2 $
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

RNA_INLINE List *
List::Create (Memory * s, Str tag, Type type, Rna car, List * cdr)
{
    List * result;

    result = (List *) s->alloc (sizeof (List));
#ifdef RNA_TYPECHECK
    result->_car._type = ANY;
#endif
    
    result->_tag = tag;
    result->_type = type;
    result->_car = car;
    result->_cdr = cdr;

    return result;

}


/******************************************************************\
 *                                                                *
 * Access - Tag                                                   *
 *                                                                *
\******************************************************************/

RNA_INLINE Str
List::getTag ()
{
    return _tag;
}

RNA_INLINE void
List::setTag (Str tag)
{
    _tag = tag;
}


/******************************************************************\
 *                                                                *
 * Access - Type                                                  *
 *                                                                *
\******************************************************************/
 
RNA_INLINE Type
List::getType ()
{
    return _type;
}
 
RNA_INLINE void
List::setType (Type type)
{
    _type = type;
}                                                                                                             


/******************************************************************\
 *                                                                *
 * Access - Car                                                   *
 *                                                                *
\******************************************************************/

RNA_INLINE Rna
List::getCar ()
{
    return _car;
}

RNA_INLINE void
List::setCar (Rna car)
{
    _car = car;
}


/******************************************************************\
 *                                                                *
 * Access - Cdr                                                   *
 *                                                                *
\******************************************************************/

RNA_INLINE List *
List::getCdr ()
{
    return _cdr;
}

RNA_INLINE void
List::setCdr (List * cdr)
{
    _cdr = cdr;
}


#endif /* RNA_DEFINE_INLINE_FUNCTIONS_NOW */
