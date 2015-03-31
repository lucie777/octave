/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/StrContext.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_STRCONTEXT_H
#define RNA_CLASS_STRCONTEXT_H

class StrContext : public Context
{
public:

    // Constructors/Destructors
    StrContext ();
    StrContext (Context * parent);
    virtual ~StrContext ();

    // Handle Management
    virtual Type handle_type () const;
    virtual Rna create_handle (Word) const;
    virtual Bool compare_handle (Rna handle1, Rna handle2) const;
    virtual Rna copy_handle (Rna handle) const;

    // System StrContext Singleton
    static StrContext * System ();

    // Nametable
    static void SetTable (Rna);

protected:

    // Static Members
    static StrContext * _System;
    static Int _Serial;
    static Array * _Table;

private:

    // Friends
    friend class Initialization;
    friend class Context;

    // Class Initialization
    static void Initialize ();

    // Testing
    virtual StrContext * strcontext ();
};

#endif /* RNA_CLASS_STRCONTEXT_H */
