/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/File.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_FILE_H
#define RNA_CLASS_FILE_H

class File : public Stream
{
public:

    // Constructors/Destrcutors
    File ();
    File (const char * filename, const char * mode);
    File (FILE *, const char * filename = 0);
    virtual ~File ();
    
    // Attaching to Files
    FILE * getFile ();
    void setFile (FILE *, const char * filename = 0);
    void detach ();

    // input - Binary
    virtual void read (Ptr buffer, Size size);
    
    // output - ASCII
    virtual void vprintf (const char * format, va_list ap);

    // output - Binary
    virtual void write (Ptr buffer, Size size);

    // position - Binary
    virtual Index tell ();
    virtual void seek (Index position);

    // flush
    virtual void flush ();

protected: 

    // input - ASCII - helper
    virtual int prim_getc ();

    // compression - helper
    virtual int get_fd ();

    // members
    FILE * _f;

private:
    // do not copy!
    File (const File &);
    File & operator = (const File &);    
};

#endif /* RNA_CLASS_FILE_H */
