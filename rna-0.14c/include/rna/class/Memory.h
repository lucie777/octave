/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Memory.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_MEMORY_H
#define RNA_CLASS_MEMORY_H

class Memory
{
public:

    // Types
    typedef size_t Size;
    typedef void * Ptr;
    
    // Constructors/Destructors
    RNA_INLINE Memory ();
    RNA_INLINE ~Memory ();

    // Allocation - One Time
    RNA_INLINE Ptr alloc (Size);
    RNA_INLINE Str alloc0 (constStr);

    // Allocation - Incremental
    RNA_INLINE void start ();
    RNA_INLINE void grow1 (const Char);
    RNA_INLINE void grow (void *, Size);
    RNA_INLINE void grow0 (constStr);
    RNA_INLINE void blank (Size);
    RNA_INLINE void * finish ();
    RNA_INLINE void * finish0 ();

    // Marker
    typedef void * Marker;
    Marker mark ();
    void rewind (Marker);

    // Administrative
    long int & chunk_size ();

    // Main Singleton
    static Memory * Main ();

    // Allocation - Incremental - Composite
    RNA_INLINE void startRecord (TypeDescriptor * typeinfo, List * attr = 0);
    RNA_INLINE void startArray (Size size, Type type);
    RNA_INLINE void grow (Rna item);
    RNA_INLINE Record * finishRecord ();
    RNA_INLINE Array * finishArray ();

protected:

    // Members
    struct obstack stack;

    // Static Members
    static Memory * _Main;

private:

    // Friends
    friend class Initialization;
    friend class StrContext;

    // Class Initialization
    static void Initialize ();

    // do not copy objects of this class
    Memory (const Memory &);
    Memory operator = (const Memory &);
};

#endif /* RNA_CLASS_MEMORY_H */
