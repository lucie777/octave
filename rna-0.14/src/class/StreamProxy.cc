/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/StreamProxy.cc,v $
 * $Revision: 1.1.1.1 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

StreamProxy::StreamProxy ()
    : delegatee (0)
{}
	
StreamProxy::StreamProxy (const StreamProxy & stream1)
    : delegatee (stream1.get_rep ())
{
    if (delegatee)
	delegatee->ref_count ++;
}

const StreamProxy & 
StreamProxy::operator = (const StreamProxy & stream1)
{
    if (delegatee)
	delegatee->ref_count --;
    if (delegatee->ref_count == 0)
	delete delegatee;
    
    delegatee = stream1.get_rep ();

    if (delegatee)
	delegatee->ref_count ++;    
}

StreamProxy::~StreamProxy ()
{
    if (delegatee)
	delegatee->ref_count --;
    if (delegatee->ref_count == 0)
	delete delegatee;

    delegatee = 0;
}

StreamProxy::StreamProxy (Stream * stream1)
    : delegatee (stream1->get_rep ())
{
    if (delegatee)
	delegatee->ref_count ++;
}

StreamProxy::StreamProxy (Stream & stream1)
    : delegatee (stream1.get_rep ())
{
    if (delegatee)
	delegatee->ref_count ++;
}

void 
StreamProxy::close ()
{
    RNA_ASSERT (delegatee);
    delegatee->close ();
}


/******************************************************************\
 *                                                                *
 * Block IO                                                       *
 *                                                                *
\******************************************************************/

Stream::Size 
StreamProxy::read (Data data, Size size)
{
    RNA_ASSERT (delegatee);
    return delegatee->read (data, size);
}

Stream::Size 
StreamProxy::write (Data data, Size size)
{
    RNA_ASSERT (delegatee);
    return delegatee->write (data, size);
}


/******************************************************************\
 *                                                                *
 * Formatted Output                                               *
 *                                                                *
\******************************************************************/

Stream::Size 
StreamProxy::vprintf (constString format, va_list ap)
{
    RNA_ASSERT (delegatee);
    return delegatee->vprintf (format, ap);
}


/******************************************************************\
 *                                                                *
 * String IO                                                      *
 *                                                                *
\******************************************************************/

Stream::Size 
StreamProxy::puts (constString s)
{
    RNA_ASSERT (delegatee);
    return delegatee->puts (s);
}

Stream::String 
StreamProxy::gets (String s, Size n)
{
    RNA_ASSERT (delegatee);
    return delegatee->gets (s, n);
}


/******************************************************************\
 *                                                                *
 * Character IO                                                   *
 *                                                                *
\******************************************************************/

Stream::Check 
StreamProxy::putc (Char c)
{
    RNA_ASSERT (delegatee);
    return delegatee->putc (c);
}

Stream::Char 
StreamProxy::getc ()
{
    RNA_ASSERT (delegatee);
    return delegatee->getc ();
}


/******************************************************************\
 *                                                                *
 * Buffering                                                      *
 *                                                                *
\******************************************************************/

Stream::Check 
StreamProxy::flush (FlushLevel level)
{
    RNA_ASSERT (delegatee);
    return delegatee->flush (level);
}

Stream::Check 
StreamProxy::setvbuf (BufferMode mode, Data data, Size size)
{
    RNA_ASSERT (delegatee);
    return delegatee->setvbuf (mode, data, size);
}


/******************************************************************\
 *                                                                *
 * Positioning                                                    *
 *                                                                *
\******************************************************************/

Stream::Check 
StreamProxy::seek (Offset offset, SeekMode mode)
{
    RNA_ASSERT (delegatee);
    return delegatee->seek (offset, mode);    
}

Stream::Offset 
StreamProxy::tell ()
{
    RNA_ASSERT (delegatee);
    return delegatee->tell ();    
}


/******************************************************************\
 *                                                                *
 * Error Handling                                                 *
 *                                                                *
\******************************************************************/

bool 
StreamProxy::eof ()
{
    RNA_ASSERT (delegatee);
    return delegatee->eof ();    
}

bool 
StreamProxy::error ()
{
    RNA_ASSERT (delegatee);
    return delegatee->error ();
}

Stream::constString 
StreamProxy::strerror ()
{
    RNA_ASSERT (delegatee);
    return delegatee->strerror ();
}


/******************************************************************\
 *                                                                *
 * Private                                                        *
 *                                                                *
\******************************************************************/

StreamImpl * 
StreamProxy::get_rep () const
{
    return delegatee;
}
