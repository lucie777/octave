/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/CompressedStream.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_COMPRESSEDSTREAM_H
#define RNA_CLASS_COMPRESSEDSTREAM_H

class CompressedStream : public Stream
{
# ifdef RNA_HAVE_ZLIB
public:

    // Constructors/Destrcutors
    CompressedStream ();
    virtual ~CompressedStream ();
    
    // Attaching to CompressedStreams
    Stream * getStream ();
    void setStream (Stream *);
    void detach ();

    // input - Binary
    virtual void read (Ptr buffer, Size size);
    
    // output - ASCII
    virtual void vprintf (const char * format, va_list ap);

    // output - Binary
    virtual void write (Ptr buffer, Size size);

    // position - Binary
    virtual Index tell ();
    virtual void seek (Index position);

    // flush
    virtual void flush ();

protected: 

    // input - ASCII - helper
    virtual int prim_getc ();

    // compression - helper
    virtual int get_fd ();

    // members
    Stream * _stream;
    gzFile _zf;

private:
    // do not copy!
    CompressedStream (const CompressedStream &);
    CompressedStream & operator = (const CompressedStream &);
    
# endif /* RNA_HAVE_ZLIB */
};

#endif /* RNA_CLASS_COMPRESSEDSTREAM_H */
