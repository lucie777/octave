/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/include/rna/class/StreamImpl.h,v $
 * $Revision: 1.1.1.1 $
 */

#ifndef RNA_CLASS_STREAM_IMPL_H
#define RNA_CLASS_STREAM_IMPL_H

class StreamImpl : public Stream
{
public:
    // constructors/destructors
    StreamImpl ();
    StreamImpl (bool owner);
    StreamImpl (const StreamImpl &);
    const StreamImpl & operator = (const StreamImpl &);

    virtual ~StreamImpl ();

    // cleanup
    virtual void close () = 0;

protected:
    
    bool owner;
    int ref_count; // used by StreamProxy

    friend class StreamProxy;

    virtual class StreamImpl * get_rep () const;
};

#endif /* RNA_CLASS_STREAM_IMPL_H */
