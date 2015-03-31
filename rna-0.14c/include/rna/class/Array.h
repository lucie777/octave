/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Array.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_ARRAY_H
#define RNA_CLASS_ARRAY_H

class Array
{
public:

    // New/Delete
    RNA_INLINE void * operator new (size_t, Memory &);
    RNA_INLINE void * operator new (size_t, Memory *);
    RNA_INLINE void operator delete (void *, size_t, Memory &);
    RNA_INLINE void operator delete (void *, size_t, Memory *);

    // Instance Creation
    static RNA_INLINE Array * Create (Memory * s, Index length, Type type);
    static Array * CreateVargs (Memory * s, Index length, Type type, Varg dummy, ...);
    static Array * vCreate (Memory * s, Index length, Type type, Ref items);

    // Access - Length
    RNA_INLINE Index getLength ();
    RNA_INLINE void setLength (Index);

    // Access - Type
    RNA_INLINE Type getType ();
    RNA_INLINE void setType (Type);

    // Access - main
    RNA_INLINE Rna at (Index);
    RNA_INLINE void atPut (Index, Rna value);

    // Copying
    RNA_INLINE Array * clone (Memory * s);
    Array * copy (Memory * s);

# ifdef RNA_SYNTACTIC_SUGAR
    // Access - Composite
    RNA_INLINE ArrayAccess operator () (Index);
# endif /* RNA_SYNTACTIC_SUGAR */
    
protected:

    // Members
    Index _length;
    Type _type;
    Word _item [0];

    // Friends
    friend class AsciiSerializer;

    friend class TypeDescriptor;
    friend class Empty;
    friend class Position;
    friend class Variant;
    friend class Module;
};

#endif /* RNA_CLASS_ARRAY_H */


