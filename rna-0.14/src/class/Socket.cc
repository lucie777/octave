/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/Socket.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

Socket::Socket ()
    : Stream (),
      _fd (0)
{
}

Socket::~Socket ()
{
}


/******************************************************************\
 *                                                                *
 * Attaching to Sockets                                             *
 *                                                                *
\******************************************************************/

int
Socket::getSocket ()
{
    return _fd;
}

void
Socket::setSocket (int fd, const char * filename)
{
    _fd = fd;
    
    if (_filename)
	free (_filename);
    if (filename)
	_filename = strdup (filename);
    else
	_filename = 0;
}


/******************************************************************\
 *                                                                *
 * Input - ASCII                                                  *
 *                                                                *
\******************************************************************/

int
Socket::prim_getc ()
{
    char item;
    ssize_t result = ::read (_fd, &item, sizeof (char));

    switch (result) {
    case 1:
	return item;
    case 0:
	return EOF;
    default:
	RNA_SHOULD_NOT_REACH_HERE;
	return 0;
    }
}


/******************************************************************\
 *                                                                *
 * Input - Binary                                                 *
 *                                                                *
\******************************************************************/

void
Socket::read (Ptr buffer, Size size)
{
    size_t result = ::read (_fd, buffer, size);
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
Socket::vprintf (const char * format, va_list ap)
{
    char * buffer = 0;
    int len = vasprintf (&buffer, format, ap);

    ::write (_fd, buffer, len);
    free (buffer);
}

/******************************************************************\
 *                                                                *
 * Output - Binary                                                *
 *                                                                *
\******************************************************************/

void
Socket::write (Ptr buffer, Size size)
{
    size_t result = ::write (_fd, buffer, size);
    RNA_ASSERT (result == 1);
}

/******************************************************************\
 *                                                                *
 * Position - Binary                                              *
 *                                                                *
\******************************************************************/

Index
Socket::tell ()
{
    return lseek (_fd, 0, SEEK_CUR);
}

void
Socket::seek (Index position)
{
    lseek (_fd, position, SEEK_SET);
}

/******************************************************************\
 *                                                                *
 * Compression - Helper                                           *
 *                                                                *
\******************************************************************/

int
Socket::get_fd ()
{
    return _fd;
}
