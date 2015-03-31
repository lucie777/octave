/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/inline/Memory.inline.cc,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_INSIDE_HEADER
# define RNA_IMPLEMENTATION
# include "rna/rna.h"
#endif /* RNA_INSIDE_HEADER */

#ifdef RNA_DEFINE_INLINE_FUNCTIONS_NOW

/******************************************************************\
 *                                                                *
 * Utilities                                                      *
 *                                                                *
\******************************************************************/

static inline void * 
obstack_chunk_alloc (size_t size)
{
    void * result = ::malloc (size);

# ifdef RNA_DEBUG
    memset (result, 0x40, size);
# endif /* RNA_DEBUG */

    return result;
}

static inline void 
obstack_chunk_free (void * ptr)
{
    ::free (ptr);
}

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

RNA_INLINE 
Memory::Memory ()
{
    obstack_init (& stack);
}

RNA_INLINE 
Memory::~Memory ()
{
    obstack_free (& stack, 0);
}


/******************************************************************\
 *                                                                *
 * Allocation                                                     *
 *                                                                *
\******************************************************************/

RNA_INLINE Memory::Ptr
Memory::alloc (Size size)
{
    return obstack_alloc (& stack, size);
}

RNA_INLINE Str
Memory::alloc0 (constStr ss)
{
    Str result = (Str) alloc (strlen (ss)+1);
    strcpy (result, ss);
    return result;
}


/******************************************************************\
 *                                                                *
 * Allocation - Incremental                                       *
 *                                                                *
\******************************************************************/

RNA_INLINE void
Memory::start ()
{}

RNA_INLINE void
Memory::grow1 (const char c)
{
    obstack_1grow (& stack, c);
}

RNA_INLINE void
Memory::grow (Ptr ptr, Size size)
{
    obstack_grow (& stack, ptr, size);
}

RNA_INLINE void
Memory::grow0 (constStr ss)
{
    obstack_grow (& stack, ss, strlen(ss));
}

RNA_INLINE void
Memory::blank (Size size)
{
    obstack_blank (& stack, size);
}

RNA_INLINE Memory::Ptr
Memory::finish ()
{
    return obstack_finish (& stack);
}

RNA_INLINE Memory::Ptr
Memory::finish0 ()
{
    obstack_1grow (& stack, '\0');    
    return obstack_finish (& stack);
}


/******************************************************************\
 *                                                                *
 * Allocation - Incremental - Composite                           *
 *                                                                *
\******************************************************************/

RNA_INLINE void
Memory::startRecord (TypeDescriptor * typeinfo, List * attr)
{
    start ();
    grow (&typeinfo, sizeof (Word));
    grow (&attr, sizeof (Word));
}

RNA_INLINE void
Memory::startArray (Size size, Type type)
{
    start ();
    grow (&size, sizeof (Word));
    grow (&type, sizeof (Word));
}

RNA_INLINE void
Memory::grow (Rna item)
{
    grow (&item._word, sizeof (Word));    
}

RNA_INLINE Record * 
Memory::finishRecord ()
{
    return (Record *) finish ();
}

RNA_INLINE Array * 
Memory::finishArray ()
{
    return (Array *) finish ();
}


#endif /* RNA_DEFINE_INLINE_FUNCTIONS_NOW */
