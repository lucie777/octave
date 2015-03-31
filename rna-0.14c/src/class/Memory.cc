/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Memory.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <stdlib.h>
#include <string.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Markers                                                        *
 *                                                                *
\******************************************************************/

Memory::Marker
Memory::mark ()
{
    return obstack_finish (& stack);
}

void
Memory::rewind (Marker marker)
{
    obstack_free (& stack, marker);
}


/******************************************************************\
 *                                                                *
 * Administrative                                                 *
 *                                                                *
\******************************************************************/

long int &
Memory::chunk_size ()
{
    return obstack_chunk_size (& stack);
}


/******************************************************************\
 *                                                                *
 * Main Singleton                                                 *
 *                                                                *
\******************************************************************/

Memory * Memory::_Main;

Memory *
Memory::Main ()
{
    return _Main;
}


/******************************************************************\
 *                                                                *
 * Class Initialization                                           *
 *                                                                *
\******************************************************************/

void
Memory::Initialize ()
{
    _Main = new Memory ();
}

