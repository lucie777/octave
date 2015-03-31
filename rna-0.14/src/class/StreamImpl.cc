/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/class/StreamImpl.cc,v $
 * $Revision: 1.1.1.1 $
 */

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Constructors/Destructors                                       *
 *                                                                *
\******************************************************************/

StreamImpl::StreamImpl ()
    : owner (true),
      ref_count (0)
{}

StreamImpl::StreamImpl (bool owner1)
    : owner (owner1),
      ref_count (0)
{}

StreamImpl::StreamImpl (const StreamImpl & stream1)
    : Stream (),
      owner (stream1.owner),
      ref_count (0)
{}

const StreamImpl &
StreamImpl::operator = (const StreamImpl & stream1)
{
    return stream1;
}

StreamImpl::~StreamImpl ()
{
    if (owner)
	close ();
}

StreamImpl * 
StreamImpl::get_rep () const
{
    return this;
}
