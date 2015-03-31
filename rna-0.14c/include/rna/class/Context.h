/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: cichon $
 * $Date: 2003/08/21 11:10:21 $
 * $Source: /sys/cvs/rna/include/rna/class/Context.h,v $
 * $Revision: 1.3 $
 */

#ifndef RNA_CLASS_CONTEXT_H
#define RNA_CLASS_CONTEXT_H

class Context
{
//protected:
public:

    // Types
    struct Patch
    {
	Word * use;
	Type * type;
	Patch *  next;
    };
    struct Binding
    {
	Rna handle;
	Bool resolved;
	Word definition;
	Type type;
	Patch * patch;
	Binding * next;

	Int depth_nesting;
	Int side_nesting;
	Int ref_count;
    };

public:

    // Constructors/Destructors
    Context ();
    Context (Context * parent);
    virtual ~Context ();

    // Handle Management
    virtual Type handle_type () const;
    virtual Rna create_handle (Word) const = 0;
    virtual Bool compare_handle (Rna handle1, Rna handle2) const;
    virtual Rna copy_handle (Rna handle) const;

    // Dictionary
    Bool includes (Rna handle);
    void enter (Rna handle, Rna definition, Type = RECORD);
    void remove (Rna handle);
    
    // Symbol Table
    class Binding * lookup (Rna handle, Bool allocate = true);
    class Binding * reverse_lookup (Rna definition, Type type = ANY, Bool allocate = true);
    void resolve (class Binding *, Word definition, Type type = ANY);
    void bind (class Binding *, Word * use, Type * type = 0);
    void relocate (Word * from, Word * to,
                   Type * from_type, Type * to_type);
    void clear_patches ();

    // Context
    Context * getParent ();
    Rna operator [] (Rna handle);
    Rna findTypeDescriptorByName (constStr name);

    // System Context Singletons
    static Context & System ();

    // Debugging
    void dump (FILE * f);

//protected:

    // Members
    Binding * _bindings;
    Memory  * _bindingspace;
    Context  * _parent;

    Memory * _patchspace;

    // Testing
    virtual StrContext * strcontext ();
    virtual IntContext * intcontext ();

private:
    // do not copy!
    Context (const Context &);
    Context & operator = (const Context &);

    // helper functions
    static int compare_name (const void * a1, const void * a2);
    static int compare_address (const void * a1, const void * a2);

    // Friends
    friend class AsciiSerializer;
    friend class BinarySerializer;
};

#endif /* RNA_CLASS_CONTEXT_H */
