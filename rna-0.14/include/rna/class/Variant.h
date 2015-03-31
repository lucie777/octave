/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Variant.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_VARIANT_H
#define RNA_CLASS_VARIANT_H

class Variant : public Record
{
public:

    // Constants
    static const Index IDX_NAME = 0;
    static const Index IDX_CHOICES = 1;
    static const Size SIZE = 2;

    // Instance Creation
    static Variant * Create (Memory * s, Rna name, Rna choices);
    
    // TypeInfo
    static TypeDescriptor * TypeInfo ();

    // Type Checking
    static Variant * Convert (Record *);

    // Access - Read
    Rna get_name ();
    Rna get_choices ();

    // Access - Write
    void set_name (Rna);
    void set_choices (Rna);
    
    // Class Initialization
    static void Initialize ();

protected:
    
    // TypeInfo
    static TypeDescriptor * _TypeInfo;

    // Members
    Str _name;
    Array * _choices;
};

#endif /* RNA_CLASS_VARIANT_H */
