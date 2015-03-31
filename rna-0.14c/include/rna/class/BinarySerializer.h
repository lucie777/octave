/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/BinarySerializer.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_BINARYSERIALIZER_H
#define RNA_CLASS_BINARYSERIALIZER_H

class BinarySerializer : public Serializer
{
public:
    // Constructors/Destructors
    BinarySerializer ();
    ~BinarySerializer ();

    // Transport
    virtual Rna read (Type type = RECORD);
    virtual void write (Rna, Type type = RECORD);

    // Testing
    virtual Bool isBinary ();

protected:

    // Different types of Context, depending on
    // Serializer Type
    virtual IntContext * intcontext ();

    // Error handling
    void error (const char * message, ...);
};

#endif /* RNA_CLASS_BINARYSERIALIZER_H */


