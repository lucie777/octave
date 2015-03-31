/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/08/20 14:47:55 $
 * $Source: /sys/cvs/rna/include/rna/class/Error.h,v $
 * $Revision: 1.2 $
 */

#ifndef RNA_CLASS_ERROR_H
#define RNA_CLASS_ERROR_H

class Error
{
public:
    
    // Error handling
    void Message (const char * file, int line, int col, const char * format, ...);
    void vMessage (const char * file, int line, int col, const char * format, va_list ap);

    // Assertions
    void AssertFail (const char * expr, const char * file, int line);
    void ShouldNotReachHere (const char * file, int line);

    // Catching items
    void Watch (Word item);
    void Use (Word item);

    // Singleton
    static Error * Instance ();

protected:

    // Static Members
    static Error * _Instance;

private:
    // Class Initialization
    static void Initialize ();

    // Friends
    friend class Initialization;
};

#endif /* RNA_CLASS_ERROR_H */

