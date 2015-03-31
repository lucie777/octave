/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/CompressedStream.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

#ifndef RNA_HAVE_ZLIB

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

CompressedStream::CompressedStream ()
    : Stream (),
      _stream (0),
      _zf (0)
{
}

CompressedStream::~CompressedStream ()
{
    if (_stream)
	gzclose (_zf);
}


/******************************************************************\
 *                                                                *
 * Attaching to CompressedStreams                                 *
 *                                                                *
\******************************************************************/

Stream *
CompressedStream::getStream ()
{
    return _stream;
}

void
CompressedStream::setStream (Stream * stream)
{
    if (_stream)
	gzclose (_zf);
    
    _stream = stream;

    _zf = gzdopen (_stream->get_fd (), "rw");
    
    if (_filename)
	free (_filename);
    if (_stream->_filename)
	_filename = strdup (_stream->_filename);
    else
	_filename = 0;
    _row = _stream->_row;
    _col = _stream->_col;
}

void
CompressedStream::detach ()
{
    gzclose (_zf);

    _stream = 0;
    _zf = 0;

    if (_filename)
	free (_filename);
    _filename = 0;
}


/******************************************************************\
 *                                                                *
 * Input - ASCII                                                  *
 *                                                                *
\******************************************************************/

int
CompressedStream::prim_getc ()
{
    return gzgetc (_zf);
}


/******************************************************************\
 *                                                                *
 * Input - Binary                                                 *
 *                                                                *
\******************************************************************/

void
CompressedStream::read (Ptr buffer, Size size)
{
    size_t result = gzread (_zf, buffer, size);
    if (result == 0) {
	_have_lookahead = true;
	_lookahead = EOF;
    }
}

/******************************************************************\
 *                                                                *
 * Output - ASCII                                                 *
 *                                                                *
\******************************************************************/

void
CompressedStream::printf (const char * format, ...)
{
    char * buffer = 0;
    va_list ap;

    va_start (ap, format);
    int len = vasprintf (&buffer, format, ap);
    va_end (ap);

    gzwrite (_zf, buffer, len);
    free (buffer);
}

/******************************************************************\
 *                                                                *
 * Output - Binary                                                *
 *                                                                *
\******************************************************************/

void
CompressedStream::write (Ptr buffer, Size size)
{
    size_t result = gzwrite (_zf, buffer, size);
    RNA_ASSERT (result == 1);
}

/******************************************************************\
 *                                                                *
 * Position - Binary                                              *
 *                                                                *
\******************************************************************/

Index
CompressedStream::tell ()
{
    return gztell (_zf);
}

void
CompressedStream::seek (Index position)
{
    gzseek (_zf, position, SEEK_SET);
}

/******************************************************************\
 *                                                                *
 * Compression - Helper                                           *
 *                                                                *
\******************************************************************/

int
CompressedStream::get_fd ()
{
    return _stream->get_fd ();
}


/******************************************************************\
 *                                                                *
 * Flush                                                          *
 *                                                                *
\******************************************************************/

void
CompressedStream::flush ()
{
    gzflush (_zf, Z_SYNC_FLUSH);
}

#endif /* RNA_HAVE_ZLIB */
