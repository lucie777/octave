/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Initialization.cc,v $
 * $Revision: 1.1.1.1 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

Bool Initialization::Initialized = false;

void
Initialization::Initialize ()
{
    // Check if we are already initialized
    if (Initialized)
	return;
    //printf("%i\n",sizeof(Word));
    //RNA_ASSERT (sizeof(Word) == 4);

    Error::Initialize ();
    Memory::Initialize ();
    TypeDescriptor::Initialize ();
    Rna::Initialize ();
    Empty::Initialize ();
    Position::Initialize ();
    Variant::Initialize ();
    Module::Initialize ();
    StrContext::Initialize ();

# ifdef RNA_SYNTACTIC_SUGAR
    Venue::Initialize ();
# endif /* RNA_SYNTACTIC_SUGAR */

    // Initialization complete
    Initialized = true;
}
