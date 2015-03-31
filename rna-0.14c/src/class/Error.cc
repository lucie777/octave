/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 14:47:56 $
 * $Source: /sys/cvs/rna/src/class/Error.cc,v $
 * $Revision: 1.2 $
 */

#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Error Handling                                                 *
 *                                                                *
\******************************************************************/

void
Error::Message (const char * file, int line, int col, const char * format, ...)
{
    va_list ap;
    va_start (ap, format);
    vMessage (file, line, col, format, ap);
    va_end (ap);
}

static void
print_position (const char * file, int line, int col)
{
    if (file) {
	fprintf (stderr, "%s:", file);
	if (line != INDEX_NOT_FOUND) {
	    fprintf (stderr, "%d:", line);
	    if (col != INDEX_NOT_FOUND) {
		fprintf (stderr, " (col=%d)", col);
	    }
	}
	fprintf (stderr, " ");
    }
}

void
Error::vMessage (const char * file, int line, int col, const char * format, va_list ap)
{
    char * buffer = 0;
    vasprintf (& buffer, format, ap);
    
    print_position (file, line, col);
    bool header_pending = false;
    for (char * p = buffer; *p; p++)
	switch (*p) {
	case '\n': 
	    fputc (*p, stderr);
	    header_pending = true;
	Default: 
	    if (header_pending) {
		print_position (file, line, col);
		header_pending = false;
	    }
	    fputc (*p, stderr);
	}
    fputc ('\n', stderr);
    fflush (stderr);

    free (buffer);

    RNA_Catch ();

    exit (EXIT_FAILURE);
}


/******************************************************************\
 *                                                                *
 * Assertions                                                     *
 *                                                                *
\******************************************************************/

void
Error::AssertFail (const char * expr, const char * file, int line)
{
    fprintf (stderr, "%s:%d: (%s) Assertion %s failed.\n",
	     file, line, program_invocation_short_name, expr);

    RNA_Catch ();

    abort ();
}

void
Error::ShouldNotReachHere (const char * file, int line)
{
    fprintf (stderr, "%s:%d: (%s) Should not reach here.\n",
	     file, line, program_invocation_short_name);

    RNA_Catch ();

    abort ();
}

/******************************************************************\
 *                                                                *
 * Catching Items                                                 *
 *                                                                *
\******************************************************************/

void
Error::Watch (Word item)
{
    RNA_Watch (item);
}

void
Error::Use (Word item)
{
    RNA_TryCatch (item);
}

/******************************************************************\
 *                                                                *
 * Other Stuff                                                    *
 *                                                                *
\******************************************************************/

Error * Error::_Instance;

void
Error::Initialize ()
{
    _Instance = new Error;
}

Error * 
Error::Instance ()
{
    return _Instance;
}
