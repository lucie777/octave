/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Record.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_RECORD_H
#define RNA_CLASS_RECORD_H

class Record 
{
public:

    // New/Delete
    RNA_INLINE void * operator new (size_t, Memory &);
    RNA_INLINE void * operator new (size_t, Memory *);
    RNA_INLINE void operator delete (void *, size_t, Memory &);
    RNA_INLINE void operator delete (void *, size_t, Memory *);
# ifdef RNA_SYNTACTIC_SUGAR
    RNA_INLINE void * operator new (size_t);
    RNA_INLINE void operator delete (void *, size_t);
# endif /* RNA_SYNTACTIC_SUGAR */

    // Instance Creation
    RNA_INLINE static Record * Create (Memory * s, TypeDescriptor * typeinfo, List * attr = 0);
    static Record * CreateVarg (Memory * s, TypeDescriptor * typeinfo, List * attr, Varg dummy, ...); 
    static Record * vCreate (Memory * s, TypeDescriptor * typeinfo, List * attr, const Varg * items);
# ifdef RNA_SYNTACTIC_SUGAR
    RNA_INLINE static Record * Create (TypeDescriptor * typeinfo, List * attr = 0);
    static Record * CreateVarg (TypeDescriptor * typeinfo, List * attr, Varg dummy, ...); 
    static Record * vCreate (TypeDescriptor * typeinfo, List * attr, const Varg * items);
# endif /* RNA_SYNTACTIC_SUGAR */

    // Access - typeinfo
    RNA_INLINE TypeDescriptor * getTypeInfo ();
    RNA_INLINE void setTypeInfo (TypeDescriptor *);

    // Access - attr
    RNA_INLINE List * getAttr ();
    RNA_INLINE void setAttr (List *);

    // Access - main
    RNA_INLINE Rna get (Index index);
    RNA_INLINE void set (Index index, Rna value);

    // Copying
    RNA_INLINE Record * clone (Memory * s);
    Record * copy (Memory * s);

    // Error Handling
    void error (constStr format, ...);

# ifdef RNA_SYNTACTIC_SUGAR
    // Access - Composite
    RNA_INLINE RecordAccess operator () (Index);
    RNA_INLINE RecordAccess operator () (Str);
    RNA_INLINE RecordAccess operator [] (Str);
# endif /* RNA_SYNTACTIC_SUGAR */

protected:

    // Members
    TypeDescriptor * _typeinfo;
    List * _attr;
    Word _item [0];

private:

    // Friends
    friend class AsciiSerializer;
    friend class Context;

    friend class TypeDescriptor;
    friend class Empty;
    friend class Position;
    friend class Variant;
    friend class Module;
};

#endif /* RNA_CLASS_RECORD_H */
