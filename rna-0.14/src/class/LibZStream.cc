/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/LibZStream.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <errno.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

LibZStream::LibZStream (gzFile z1)
    : z (z1),
      have_error (false)
{}

LibZStream::LibZStream (const LibZStream & stream1)
    : StreamImpl (stream1),
      z (stream1.z),
      have_error (false)
{}

const LibZStream &
LibZStream::operator = (const LibZStream & stream1)
{
    z = stream1.z;
    owner = false;
    have_error = stream1.have_error;
    
    return stream1;
}

void
LibZStream::close ()
{
    gzclose (z);
}


/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

LibZStream::LibZStream (constString filename, Mode mode)
    : StreamImpl (true)
{
    z = gzopen (filename, mode);
    have_error = (z == 0);
}

LibZStream::LibZStream (int fd, Mode mode)
    : StreamImpl (true)
{
    z = gzdopen (fd, mode);
    have_error = (z == 0);
}

/******************************************************************\
 *                                                                *
 * IO                                                             *
 *                                                                *
\******************************************************************/

LibZStream::Size 
LibZStream::read (Data data, Size size)
{
    int result = gzread (z, data, size);
    have_error = (result < 0);
    return result;
}

LibZStream::Size 
LibZStream::write (Data data, Size size)
{
    int result = gzwrite (z, data, size);
    have_error = (result == 0);
    return result;
}

LibZStream::Size 
LibZStream::vprintf (constString format, va_list ap)
{
    char * buffer = 0;
    vasprintf (&buffer, format, ap);
    Size result = puts (buffer);
    free (buffer);
    return result;
}

LibZStream::Size 
LibZStream::puts (constString s)
{
    int result = gzputs (z, s);
    have_error = (result == -1);
    return result;
}

LibZStream::String 
LibZStream::gets (String s, Size size)
{
    char * result = gzgets (z, s, size);
    have_error = (result == Z_NULL);
    return result;
}

LibZStream::Check 
LibZStream::putc (Char c)
{
    int result = gzputc (z, c);
    have_error = (result == -1);
    return result;
}

LibZStream::Char 
LibZStream::getc ()
{
    int result = gzgetc (z);
    have_error = (result == -1);
    return result;
}


/******************************************************************\
 *                                                                *
 * Administrative                                                 *
 *                                                                *
\******************************************************************/

LibZStream::Check 
LibZStream::flush (FlushLevel level)
{
    int flush;
    switch (level) {
    case FLUSH_NONE: flush = Z_NO_FLUSH; break;
    case FLUSH_SYNC: flush = Z_SYNC_FLUSH; break;
    case FLUSH_FULL: flush = Z_FULL_FLUSH; break;
    case FLUSH_FILE: /* @@@ TBD @@@ */ flush = Z_FULL_FLUSH; break;
    case FLUSH_DISK: /* @@@ TBD @@@ */ flush = Z_FULL_FLUSH; break;
    default: RNA_SHOULD_NOT_REACH_HERE;
    }
    return gzflush (z, flush);
}

LibZStream::Check 
LibZStream::setvbuf (BufferMode mode, Data buffer, Size size)
{
    /* not implemented yet */
    RNA_SHOULD_NOT_REACH_HERE;
    return 0;
}

LibZStream::Check 
LibZStream::seek (Offset offset, SeekMode mode)
{
    int result = gzseek (z, offset, mode);
    have_error = (result == -1);
    return result;
}

LibZStream::Offset 
LibZStream::tell ()
{
    int result = gztell (z);
    have_error = (result == -1);    
    return result;
}

bool 
LibZStream::eof ()
{
    return gzeof (z);
}

bool 
LibZStream::error ()
{
    /* @@@ TBD @@@ */
    return have_error && !eof ();
}

LibZStream::constString 
LibZStream::strerror ()
{
    int errnum;
    constString result = gzerror (z, & errnum);
    if (errnum != Z_ERRNO)
        return result;
    else
        return ::strerror (errno);
}

