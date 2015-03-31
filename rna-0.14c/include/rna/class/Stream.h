/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Stream.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_STREAM_H
#define RNA_CLASS_STREAM_H

class Stream 
{
public:

    // Constructors
    Stream ();
    virtual ~Stream ();

    // input - ASCII
    Bool atEnd ();
    Char peek ();
    Char next ();

    // input - Binary
    virtual void read (Ptr buffer, Size size) = 0;
    
    // output - ASCII
    virtual void printf (const char * format, ...);
    virtual void vprintf (const char * format, va_list ap) = 0;

    // output - Binary
    virtual void write (Ptr buffer, Size size) = 0;

    // position - ASCII
    Position * getPosition (Memory *);
    void setPosition (Position *);

    // position - Binary
    virtual Index tell () = 0;
    virtual void seek (Index position) = 0;

    // flush
    virtual void flush ();

    // Error Handling
    void error (constStr format, ...);
    
protected:

    // position - ASCII - helper
    Str _filename;
    Int _row;
    Int _col;
    
    // input - ASCII - helper
    Bool _have_lookahead;
    int _lookahead;
    void check_lookahead ();
    virtual int prim_getc () = 0;

    // compression - helper
    virtual int get_fd () = 0;

private:
    // do not copy!
    Stream (const Stream &);
    Stream & operator = (const Stream &);
    
    // friends
    friend class AsciiSerializer;
    friend class BinarySerializer;
    friend class CompressedStream; // @@@ protected bug
};

#endif /* RNA_CLASS_STREAM_H */
