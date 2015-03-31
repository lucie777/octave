/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Serializer.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_SERIALIZER_H
#define RNA_CLASS_SERIALIZER_H

#define RNA_ASCII_HEADER  "# RNA 1.0 ASCII"
#define RNA_BINARY_HEADER "# RNA 1.0 binary"
#define RNA_POSITION_TAG  "position"

class Serializer
{
public:

    // Instance Creation
    static Serializer * Create (Stream * source);

    // Constructors/Destructors
    Serializer ();
    virtual ~Serializer ();

    // Access - Read
    Stream * get_stream ();
    Context * get_context ();
    Memory * get_memory ();

    // Access - Write
    void set_stream (Stream *);
    void set_context (Context *);
    void set_memory (Memory *);

    // Transport
    virtual Rna read (Type type = RECORD) = 0;
    virtual void write (Rna, Type type = RECORD) = 0;

    // Testing
    virtual Bool isAscii ();
    virtual Bool isBinary ();

protected:

    // Different types of Context, depending on
    // Serializer Type
    virtual StrContext * strcontext ();
    virtual IntContext * intcontext ();

    // Members
    Stream * _stream;
    Context * _context;
    Memory * _memory;
    Memory * _parse;

private:
    // do not copy!
    Serializer (const Serializer &);
    Serializer & operator = (const Serializer &);    
};

#endif /* RNA_CLASS_SERIALIZER_H */
