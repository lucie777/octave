/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/lib/write-b.cc,v $
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
 * Writing RNA                                                    *
 *                                                                *
\******************************************************************/

static inline void 
write32 (Stream * s, int value)
{
    s->write (&value, 4);
}

static inline void 
write_zero (Stream * s, int count)
{
    int zeros [2] = { 0, 0 };

    RNA_ASSERT (0 <= count && count < 8);

    s->write (zeros, count);
}

void
BinarySerializer::write (Rna rna, Type type)
{
#if 0
    int i, length;
    Context::Binding * entry;

    switch (type) {
    case BOOL:
    case INT:
    case FLOAT:
    case PTR: 
	write32 (_stream. rna._word.ii);
	break;
    case STR: 
	write32 (_stream, rna._word.ii);
	if (!rna.isNull ()) {
	    entry = _context->reverse_lookup (rna);
	    write32 (_stream, entry->handle._word.ii);
	    
	    if (!entry->resolved) {
		_context->resolve (entry, rna);
		entry->type = type;

		Size len = strlen (rna._word.ss);
		write32 (_stream, len);
		_stream->write (rna._word.ss, len);
		write_zero (_stream, 4 - len % 4);
	    } else {
		RNA_ASSERT (entry->type == type);
	    }
	}
	break;
    case RECORD:
	write32 (_stream, rna._word.ii);
	if (!rna.isNull ()) {
	    entry = _context->reverse_lookup (rna);
	    write32 (_stream, entry->handle._word.ii);
	    
	    if (!entry->resolved) {
		_context->resolve (entry, rna);
		entry->type = type;

		write (rna._word.record->_typeinfo, RECORD);

		length = rna._word.record->_typeinfo->size ();
		for (i = 0; i < length; i++)
		    write (rna._word.record->_item [i], 
			   rna._word.record->_typeinfo->type(i));
		for (List * s = rna._word.record->_attr; s; s = s->_cdr) {
		    emit ("\n"); emit_indent ();
		    emit ("%s=", s->_tag);
		    write (s->_car, RECORD);
		}
		emit (")"); unindent ();
	    } else {
		RNA_ASSERT (entry->type == type);
	    }
	}
	break;
    case ARRAY:
	if (!rna.isNull ()) {
	    entry = _context->reverse_lookup (rna);
	    
	    if (!entry->resolved) {
		_context->resolve (entry, rna);
		entry->type = type;

		emit ("%s: ", entry->handle._word.ss);
		emit ("["); indent ();
		length = rna._word.array->_length;
		for (i = 0; i < length; i++) {
		    write (rna._word.array->_item [i], rna._word.array->_type);
		    if (i < length-1) {
			emit ("\n"); emit_indent (); }
		}
		emit ("]"); unindent ();
	    } else {
		assert (entry->type == type);
		emit ("%s", entry->handle._word.ss);
	    }
	} else 
	    emit ("[]");
	break;
    default:
	assert (0);
    }
    flush ();    
#endif
}
