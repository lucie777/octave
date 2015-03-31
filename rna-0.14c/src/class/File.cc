/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: cichon $
 * $Date: 2003/07/13 10:05:41 $
 * $Source: /sys/cvs/rna/src/class/File.cc,v $
 * $Revision: 1.2 $
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

File::File ()
    : Stream (),
      _f (0)
{
}

File::File (const char * filename, const char * mode)
    : Stream (),
      _f (0)
{
    FILE * f = fopen (filename, mode);
    
    if (f == 0) {
	fprintf (stderr, "%s: cannot open file %s\n",
		 program_invocation_short_name, filename);
	perror (program_invocation_short_name);

	RNA_ERROR ("Cannot open file\n");
    }
//    filename, strerror (errno);
    
    setFile (f, filename);
}

File::File (FILE * f, const char * filename)
    : Stream (),
      _f (0)
{
    setFile (f, filename);
}

File::~File ()
{
    if (_f)
	fclose (_f);
}


/******************************************************************\
 *                                                                *
 * Attaching to Files                                             *
 *                                                                *
\******************************************************************/

FILE *
File::getFile ()
{
    return _f;
}

void
File::setFile (FILE * g, const char * filename)
{
    _f = g;
    
    if (_filename)
	free (_filename);
    if (filename)
	_filename = strdup (filename);
    else
	_filename = 0;
}

void
File::detach ()
{
    if (_have_lookahead)
	ungetc (_lookahead, _f);

    _f = 0;

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
File::prim_getc ()
{
    return fgetc (_f);
}


/******************************************************************\
 *                                                                *
 * Input - Binary                                                 *
 *                                                                *
\******************************************************************/

void
File::read (Ptr buffer, Size size)
{
    size_t result = fread (buffer, size, 1, _f);
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
File::vprintf (const char * format, va_list ap)
{
    vfprintf (_f, format, ap);
}

/******************************************************************\
 *                                                                *
 * Output - Binary                                                *
 *                                                                *
\******************************************************************/

void
File::write (Ptr buffer, Size size)
{
    size_t result = fwrite (buffer, size, 1, _f);
    RNA_ASSERT (result == 1);
}

/******************************************************************\
 *                                                                *
 * Position - Binary                                              *
 *                                                                *
\******************************************************************/

Index
File::tell ()
{
    return ftell (_f);
}

void
File::seek (Index position)
{
    fseek (_f, position, SEEK_SET);
}

/******************************************************************\
 *                                                                *
 * Compression - Helper                                           *
 *                                                                *
\******************************************************************/

int
File::get_fd ()
{
    return fileno (_f);
}


/******************************************************************\
 *                                                                *
 * Flush                                                          *
 *                                                                *
\******************************************************************/

void
File::flush ()
{
    fflush (_f);
}
