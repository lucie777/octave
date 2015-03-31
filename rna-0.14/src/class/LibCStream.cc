/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/LibCStream.cc,v $
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

LibCStream::LibCStream (FILE * f1)
    : StreamImpl (stream1),
      f (f1)
{}

LibCStream::LibCStream (const LibCStream & stream1)
    : StreamImpl (stream1),
      f (stream1.f)
{}

const LibCStream &
LibCStream::operator = (const LibCStream & stream1)
{
    if (f == stream1.f)
        return stream1;

    if (owner)
        fclose (f);

    f = stream1.f;
    owner = false;
    
    return stream1;
}

void
LibCStream::close ()
{
    fclose (f);
}


/******************************************************************\
 *                                                                *
 * Instance Creation                                              *
 *                                                                *
\******************************************************************/

LibCStream::LibCStream (constString filename, Mode mode)
    : StreamImpl (true)
{
    f = fopen (filename, mode);
}

LibCStream::LibCStream (int fd, Mode mode)
    : StreamImpl (true)
{
    f = fdopen (fd, mode);
}

LibCStream::LibCStream (Data data, Size size, Mode mode)
    : StreamImpl (true)
{
    f = fmemopen (data, size, mode);
}

LibCStream::LibCStream (Data * data, Size * size)
    : StreamImpl (true)
{
    f = open_memstream ((char **) data, size);
}

/******************************************************************\
 *                                                                *
 * IO                                                             *
 *                                                                *
\******************************************************************/

LibCStream::Size 
LibCStream::read (Data data, Size size)
{
    return fread (data, size, 1, f);
}

LibCStream::Size 
LibCStream::write (Data data, Size size)
{
    return fwrite (data, size, 1, f);
}

LibCStream::Size 
LibCStream::vprintf (constString format, va_list ap)
{
    return vfprintf (f, format, ap);
}

LibCStream::Size 
LibCStream::puts (constString s)
{
    return fputs (s, f);
}

LibCStream::String 
LibCStream::gets (String s, Size size)
{
    return fgets (s, size, f);
}

LibCStream::Check 
LibCStream::putc (Char c)
{
    return fputc (c, f);
}

LibCStream::Char 
LibCStream::getc ()
{
    return fgetc (f);
}


/******************************************************************\
 *                                                                *
 * Administrative                                                 *
 *                                                                *
\******************************************************************/

LibCStream::Check 
LibCStream::flush (FlushLevel level)
{
    if (level > FLUSH_SYNC)
	return fflush (f);
    else return 0;
}

LibCStream::Check 
LibCStream::setvbuf (BufferMode mode, Data buffer, Size size)
{
    int arg;
    switch (mode) {
    case BUFFER_FULL: arg = _IOFBF; break;
    case BUFFER_LINE: arg = _IOLBF; break;
    case BUFFER_NONE: arg = _IONBF; break;
    }
    return ::setvbuf (f, (char *) buffer, arg, size);
}

LibCStream::Check 
LibCStream::seek (Offset offset, SeekMode mode)
{
    return fseek (f, offset, mode);
}

LibCStream::Offset 
LibCStream::tell ()
{
    return ftell (f);
}

bool 
LibCStream::eof ()
{
    return feof (f);
}

bool 
LibCStream::error ()
{
    return ferror (f);
}

LibCStream::constString 
LibCStream::strerror ()
{
    return ::strerror (errno);
}

