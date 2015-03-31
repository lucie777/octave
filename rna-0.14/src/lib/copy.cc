/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/lib/copy.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * Copying RNA                                                    *
 *                                                                *
\******************************************************************/

Record *
Record::copy (Memory * s)
{
    return Rna (this).copy (s, RECORD)._word.record;
}

Array *
Array::copy (Memory * s)
{
    return Rna (this).copy (s, ARRAY)._word.array;
}

List *
List::copy (Memory * s)
{
    return Rna (this).copy (s, LIST)._word.list;
}

Rna 
Rna::copy (Memory * s, Type type)
{
#if 1
    RNA_SHOULD_NOT_REACH_HERE;
    // I don't know why Suse 7.0 does not have fmemopen in its libc
    return SELF;
#else 
    char * buffer = 0;
    size_t size = 0;

    Rna result = Rna::Null ();

    {
	FILE * f = open_memstream (&buffer, &size);
	File * file = new File;
	file->setFile (f, "<copying>");

	IntContext * intcontext = new IntContext;
    
	BinarySerializer * serializer = new BinarySerializer;
	serializer->set_stream (file);
	serializer->set_context (intcontext);
	serializer->set_memory (s);
    
	serializer->write (SELF, type);

	delete serializer;
	delete intcontext;
	delete file;
    }
    
    {
	FILE * f = fmemopen (buffer, size, "r");
	File * file = new File;
	file->setFile (f, "<copying>");

	IntContext * intcontext = new IntContext;
    
	BinarySerializer * serializer = new BinarySerializer;
	serializer->set_stream (file);
	serializer->set_context (intcontext);
	serializer->set_memory (s);
    
	result = serializer->read (type);

	delete serializer;
	delete intcontext;
	delete file;
    }    

    return result;
#endif
}

