/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Venue.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_VENUE_H
#define RNA_CLASS_VENUE_H

# ifdef RNA_SYNTACTIC_SUGAR

class Venue {
public:

    // Instance Creation
    RNA_INLINE Venue ();
    RNA_INLINE Venue (Memory *);
    RNA_INLINE ~Venue ();

    // Stack Management
    static RNA_INLINE void Push (Venue *);
    static RNA_INLINE void Pop (Venue * check = 0);
    static RNA_INLINE Venue * Current ();

    // Access
    RNA_INLINE Memory * getMemory ();
    RNA_INLINE void setMemory (Memory *);
    RNA_INLINE void setOwnership (Bool);

    // Class Initialization
    static void Initialize ();

protected:

    // Types
    struct VenueEntry {
	Venue * current;
	VenueEntry * next;
    };

    // Members
    Memory * _memory;
    Bool _owner;

    // Static Members
    static struct VenueEntry * _Stack;
};

# endif /* RNA_SYNTACTIC_SUGAR */

#endif /* RNA_CLASS_VENUE_H */
