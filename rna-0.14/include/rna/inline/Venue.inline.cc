/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/inline/Venue.inline.cc,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_INSIDE_HEADER
# define RNA_IMPLEMENTATION
# include "rna/rna.h"
#endif /* RNA_INSIDE_HEADER */

#ifdef RNA_DEFINE_INLINE_FUNCTIONS_NOW

# ifdef RNA_SYNTACTIC_SUGAR

/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

RNA_INLINE
Venue::Venue ()
    : _memory (new Memory),
      _owner (true)
{
    Push (this);
}

RNA_INLINE
Venue::Venue (Memory * memory)
    : _memory (memory),
      _owner (false)
{
    Push (this);
}

RNA_INLINE
Venue::~Venue ()
{
    Pop (this);
    if (_owner)
	delete _memory;
}


/******************************************************************\
 *                                                                *
 * Stack Management                                               *
 *                                                                *
\******************************************************************/

RNA_INLINE void
Venue::Push (Venue * venue)
{
    VenueEntry * entry (new VenueEntry);

    entry->current = venue;
    entry->next = _Stack;

    _Stack = entry;
}

RNA_INLINE void
Venue::Pop (Venue * check)
{
    VenueEntry * entry (_Stack);

    RNA_ASSERT (entry);
    RNA_ASSERT (!check || entry->current == check);

    _Stack = entry->next;

    delete entry;
}

RNA_INLINE Venue *
Venue::Current ()
{
    RNA_ASSERT (_Stack);

    return _Stack->current;
}


/******************************************************************\
 *                                                                *
 * Access                                                         *
 *                                                                *
\******************************************************************/

RNA_INLINE Memory *
Venue::getMemory ()
{
    return _memory;
}

RNA_INLINE void
Venue::setMemory (Memory * memory)
{
    _memory = memory;
}

RNA_INLINE void
Venue::setOwnership (Bool owner)
{
    _owner = owner;
}


# endif /* RNA_SYNTACTIC_SUGAR */

#endif /* RNA_DEFINE_INLINE_FUNCTIONS_NOW */
