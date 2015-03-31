/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/Socket.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_SOCKET_H
#define RNA_CLASS_SOCKET_H

class Socket : public Stream
{
public:

    // Constructors/Destrcutors
    Socket ();
    virtual ~Socket ();
    
    // Attaching to Sockets
    int getSocket ();
    void setSocket (int, const char * filename = 0);

    // input - Binary
    virtual void read (Ptr buffer, Size size);
    
    // output - ASCII
    virtual void vprintf (const char * format, va_list ap);

    // output - Binary
    virtual void write (Ptr buffer, Size size);

    // position - Binary
    virtual Index tell ();
    virtual void seek (Index position);

protected: 

    // input - ASCII - helper
    virtual int prim_getc ();

    // compression - helper
    virtual int get_fd ();

    // members
    int _fd;
};

#endif /* RNA_CLASS_SOCKET_H */
