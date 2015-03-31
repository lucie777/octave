/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/AsciiSerializer.cc,v $
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

AsciiSerializer::AsciiSerializer ()
    : Serializer ()
{}

AsciiSerializer::~AsciiSerializer ()
{}


/******************************************************************\
 *                                                                *
 * Testing                                                        *
 *                                                                *
\******************************************************************/

Bool 
AsciiSerializer::isAscii ()
{
    return true;
}

StrContext * 
AsciiSerializer::strcontext ()
{
    return _context->strcontext ();
}


/******************************************************************\
 *                                                                *
 * Error handling                                                 *
 *                                                                *
\******************************************************************/

void 
AsciiSerializer::error (const char * message, ...)
{
    if (_stream->_filename)
	fprintf (stderr, "%s:", _stream->_filename);
    fprintf (stderr, "%d: (%d) ", _stream->_row, _stream->_col);
    
    va_list ap;
    va_start (ap, message);
    vfprintf (stderr, message, ap);
    va_end (ap);
    
    RNA_Catch ();

    exit (1);
}
