/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/IntContext.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_INTCONTEXT_H
#define RNA_CLASS_INTCONTEXT_H

class IntContext : public Context
{
public:

    // Constructors/Destructors
    IntContext ();
    IntContext (Context * parent);
    virtual ~IntContext ();

    // Handle Management
    virtual Type handle_type () const;
    virtual Rna create_handle (Word) const;
    virtual Bool compare_handle (Rna handle1, Rna handle2) const;
    virtual Rna copy_handle (Rna handle) const;

    // Testing
    virtual IntContext * intcontext ();
};

#endif /* RNA_CLASS_INTCONTEXT_H */
