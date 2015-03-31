/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Venue.cc,v $
 * $Revision: 1.1.1.1 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

# ifdef RNA_SYNTACTIC_SUGAR

/******************************************************************\
 *                                                                *
 * Static Members                                                 *
 *                                                                *
\******************************************************************/

struct RNA::Venue::VenueEntry * Venue::_Stack = 0;


/******************************************************************\
 *                                                                *
 * Class Initialization                                           *
 *                                                                *
\******************************************************************/

void 
Venue::Initialize ()
{
    VenueEntry * entry = new VenueEntry;

    entry->current = new Venue (Memory::Main ());
    entry->next = 0;

    _Stack = entry;
}

# endif /* RNA_SYNTACTIC_SUGAR */
