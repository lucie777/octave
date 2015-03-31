/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/BinarySerializer.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <stdarg.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

BinarySerializer::BinarySerializer ()
    : Serializer ()
{}

BinarySerializer::~BinarySerializer ()
{}


/******************************************************************\
 *                                                                *
 * Testing                                                        *
 *                                                                *
\******************************************************************/

Bool
BinarySerializer::isBinary () 
{
    return true;
}

IntContext * 
BinarySerializer::intcontext ()
{
    return _context->intcontext ();
}

/******************************************************************\
 *                                                                *
 * Error Handling                                                 *
 *                                                                *
\******************************************************************/

void 
BinarySerializer::error (const char * message, ...)
{
    if (_stream->_filename)
	fprintf (stderr, "%s:", _stream->_filename);
    
    va_list ap;
    va_start (ap, message);
    vfprintf (stderr, message, ap);
    va_end (ap);

    exit (1);
}

