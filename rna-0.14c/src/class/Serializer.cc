/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Serializer.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <string.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

Serializer * 
Serializer::Create (Stream * source)
{
    Serializer * result = 0;

    Bool maybe_ascii = true;
    Bool maybe_binary = true;
    for (int xx = 0;; xx++) {
	if (maybe_ascii)
	    if (RNA_ASCII_HEADER [xx] == '\0') {
		result = new AsciiSerializer;
		result->set_context (new StrContext);
		break;
	    }
	if (maybe_binary)
	    if (RNA_BINARY_HEADER [xx] == '\0') {
		result = new BinarySerializer;
		result->set_context (new IntContext);
		break;
	    }
	Char cc = source->next ();
	if (maybe_ascii && RNA_ASCII_HEADER [xx] != cc)
	    maybe_ascii = false;
	if (maybe_binary && RNA_BINARY_HEADER [xx] != cc)
	    maybe_binary = false;

	if (!maybe_ascii && !maybe_binary) 
	    return 0;
    }
    
    result->set_stream (source);
    result->set_memory (new Memory);

    return result;
}

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

Serializer::Serializer ()
    : _stream (0),
      _context (0),
      _memory (0),
      _parse (new Memory)
{
}

Serializer::~Serializer ()
{
    delete _parse;

    if (_stream)
	delete _stream;
    if (_context)
	delete _context;
    if (_memory)
	delete _memory;
}


/******************************************************************\
 *                                                                *
 * Access - Read                                                  *
 *                                                                *
\******************************************************************/

Stream * 
Serializer::get_stream ()
{
    return _stream;
}

Context * 
Serializer::get_context ()
{
    return _context;
}

Memory * 
Serializer::get_memory ()
{
    return _memory;
}


/******************************************************************\
 *                                                                *
 * Access - Write                                                 *
 *                                                                *
\******************************************************************/

void 
Serializer::set_stream (Stream * stream)
{
    _stream = stream;
}

void 
Serializer::set_context (Context * context)
{
    _context = context;
}

void 
Serializer::set_memory (Memory * memory)
{
    _memory = memory;
}

/******************************************************************\
 *                                                                *
 * Testing                                                        *
 *                                                                *
\******************************************************************/

Bool
Serializer::isAscii ()
{
    return false;
}

Bool
Serializer::isBinary ()
{
    return false;
}

StrContext * 
Serializer::strcontext ()
{
    RNA_SHOULD_NOT_REACH_HERE;

    return 0;
}

IntContext * Serializer::intcontext ()
{
    RNA_SHOULD_NOT_REACH_HERE;

    return 0;
}
