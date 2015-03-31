/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Module.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_MODULE_H
#define RNA_CLASS_MODULE_H

class Module : public Record
{
public:

    // Constants
    static const Index IDX_NAME = 0;
    static const Index IDX_CONTENTS = 1;
    static const Size SIZE = 2;

    // Instance Creation
    static Module * Create (Memory * s, Rna name, Rna contents);
    
    // TypeInfo
    static TypeDescriptor * TypeInfo ();

    // Type Checking
    static Module * Convert (Record *);

    // Access - Read
    Rna get_name ();
    Rna get_contents ();

    // Access - Write
    void set_name (Rna);
    void set_contents (Rna);
    
    // Class Initialization
    static void Initialize ();

protected:
    
    // TypeInfo
    static TypeDescriptor * _TypeInfo;

    // Members
    Str _name;
    Array * _contents;
};

#endif /* RNA_CLASS_MODULE_H */
