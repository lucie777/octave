/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/TypeDescriptor.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_TYPEDESCRIPTOR_H
#define RNA_CLASS_TYPEDESCRIPTOR_H

const Index INDEX_NOT_FOUND = -1;

class TypeDescriptor : public Record
{
public:

    // Constants
    static const Index IDX_NAME = 0;
    static const Index IDX_SIZE = 1;
    static const Index IDX_FORMAT = 2;
    static const Index IDX_PARTS = 3;
    static const Size SIZE = 4;

    // Instance Creation
    static TypeDescriptor * Create (Memory * s, Rna name, Rna size, Rna format, Rna parts);
    
    // TypeInfo
    static TypeDescriptor * TypeInfo ();

    // Type Checking
    static TypeDescriptor * Convert (Record *);

    // Access - Read
    Rna get_name ();
    Rna get_size ();
    Rna get_format ();
    Rna get_parts ();

    // Access - Write
    void set_name (Rna);
    void set_size (Rna);
    void set_format (Rna);
    void set_parts (Rna);
    
    // Class Initialization
    static void Initialize ();

    // Name Lookup
    Bool check ();
    Index lookup (constStr name) const;
    Type type (Index x) const;

protected:
    
    // TypeInfo
    static TypeDescriptor * _TypeInfo;

    // Friends
    friend class Context;
    friend class Position;
    friend class Variant;
    friend class Module;

    // Members
    Str _name;
    Int _size;
    Str _format;
    Array * _parts;
};

#endif /* RNA_CLASS_TYPEDESCRIPTOR_H */
