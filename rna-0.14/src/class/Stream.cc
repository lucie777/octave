/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: cichon $
 * $Date: 2003/07/10 06:54:44 $
 * $Source: /sys/cvs/rna/src/class/Stream.cc,v $
 * $Revision: 1.2 $
 */

#include <stdlib.h>
#include <string.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

Stream::Stream ()
    : _filename (0),
      _row (0),
      _col (0),
      _have_lookahead (false),
      _lookahead (EOF)
{
}

Stream::~Stream ()
{
    flush ();
    if (_filename)
	free (_filename);
}


/******************************************************************\
 *                                                                *
 * Input - ASCII                                                  *
 *                                                                *
\******************************************************************/

void 
Stream::check_lookahead ()
{
    if (!_have_lookahead) {

	_lookahead = prim_getc ();

	_have_lookahead = true;
    }	
}

Bool
Stream::atEnd ()
{
    check_lookahead ();

    return _lookahead == EOF;
}

Char
Stream::peek ()
{
    check_lookahead ();

    return _lookahead;
}

Char
Stream::next ()
{
    check_lookahead ();

    _have_lookahead = false;

    switch (_lookahead) {
    case EOF:
    case '\0':
    case '\r':
	/* do nothing */;
        break;
    case '\t':
	_col += 8;
	break;
    case '\n':
	_col = 0;
	_row++;
	break;
    default:
	_col++;
	break;
    }

    return _lookahead;
}

/******************************************************************\
 *                                                                *
 * Position - ASCII                                               *
 *                                                                *
\******************************************************************/

Position * 
Stream::getPosition (Memory * s)
{
    return Position::Create (s, _filename, _row, _col);
}

void 
Stream::setPosition (Position * pos)
{
    _filename = strdup (pos->get_filename ());
    _row = pos->get_row ();
    _col = pos->get_column ();
}


/******************************************************************\
 *                                                                *
 * Output - ASCII                                                 *
 *                                                                *
\******************************************************************/

void
Stream::printf (const char * format, ...)
{
    va_list ap;
    va_start (ap, format);
     
    vprintf (format, ap);

    va_end (ap);
}


/******************************************************************\
 *                                                                *
 * Flush                                                          *
 *                                                                *
\******************************************************************/

void
Stream::flush ()
{
}

/******************************************************************\
 *                                                                *
 * Error Handling                                                 *
 *                                                                *
\******************************************************************/

void
Stream::error (const char * format, ...)
{
    va_list ap;
    va_start (ap, format);
    Error::Instance ()->vMessage (_filename, _row, _col, format, ap);
    va_end (ap);
}
