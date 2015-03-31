/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/IntContext.cc,v $
 * $Revision: 1.1.1.1 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

IntContext::IntContext ()
    : Context ()
{
}

IntContext::IntContext (Context * parent)
    : Context (parent)
{
}

IntContext::~IntContext ()
{
}


/******************************************************************\
 *                                                                *
 * Handle Management                                              *
 *                                                                *
\******************************************************************/

Type
IntContext::handle_type () const
{
    return INT;
}

Rna
IntContext::create_handle (Word word) const
{
# ifndef RNA_TYPECHECK
    return Rna (word);
# else /* RNA_TYPECHECK */
    return Rna (word, INT);
# endif /* RNA_TYPECHECK */
}

Bool
IntContext::compare_handle (Rna handle1, Rna handle2) const
{
    return handle1 == handle2;
}

Rna
IntContext::copy_handle (Rna handle) const
{
    return handle;
}


/******************************************************************\
 *                                                                *
 * Testing                                                        *
 *                                                                *
\******************************************************************/

IntContext * 
IntContext::intcontext ()
{
    return this;
}
