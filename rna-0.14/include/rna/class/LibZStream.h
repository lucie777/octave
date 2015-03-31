/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/LibZStream.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_LIBZ_STREAM_H
#define RNA_CLASS_LIBZ_STREAM_H

#include <zlib.h>

class LibZStream : public StreamImpl
{
  public:

    typedef constString Mode;

    // constructors/destructors
    LibZStream (gzFile);
    LibZStream (const LibZStream &);
    const LibZStream & operator = (const LibZStream &);
    virtual void close ();

    // instance creation
    LibZStream (constString filename, Mode);
    LibZStream (int fd, Mode);

    // block IO
    virtual Size read (Data, Size);
    virtual Size write (Data, Size);

    // formatted output
    virtual Size vprintf (constString format, va_list ap);

    // string IO
    virtual Size puts (constString);
    virtual String gets (String, Size);

    // character IO
    virtual Check putc (Char);
    virtual Char getc ();

    // buffering
    virtual Check flush (FlushLevel);
    virtual Check setvbuf (BufferMode, Data, Size);

    // positioning
    virtual Check seek (Offset, SeekMode);
    virtual Offset tell ();

    // error handling
    virtual bool eof ();
    virtual bool error ();
    virtual constString strerror ();

  protected:
    gzFile z;
    bool have_error;
};

#endif /* RNA_CLASS_LIBZ_STREAM_H */
