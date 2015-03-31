/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/07/16 20:18:50 $
 * $Source: /sys/cvs/rna/src/lib/write-a.cc,v $
 * $Revision: 1.6 $
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define RNA_IMPLEMENTATION
#include "rna/rna.h"

/******************************************************************\
 *                                                                *
 * AsciiSerializer - Transport                                 *
 *                                                                *
\******************************************************************/

void
AsciiSerializer::write (Rna rna, Type type)
{
    _stream->printf ("%s\n\n", RNA_ASCII_HEADER);

    scan_item (rna, type, 0, 0);

    Snippets sns (_stream);
    write_item (rna, type, sns, 0, 0);
    sns.flush ();
}


/******************************************************************\
 *                                                                *
 * AsciiSerializer - 1. scan nesting levels and ref counts     *
 *                                                                *
\******************************************************************/

void 
AsciiSerializer::scan_item (Rna rna, Type type,
			    Int side_nesting, Int depth_nesting)
{
    int i, length;
    Context::Binding * entry;

# ifdef RNA_TYPECHECK
    RNA_ASSERT (rna._type == ANY || rna._type == type);
# endif /* RNA_TYPECHECK */

    switch (type) {
    case BOOL:
    case INT:
    case FLOAT:
    case PTR: 
	break;
    case STR: 
    case RECORD:
    case ARRAY:
	if (rna._word.record) {
	    entry = _context->reverse_lookup (rna, type);
	    Bool resolved = false;

	    if (entry->ref_count > 0)
		resolved = true;
	    entry->ref_count++;

	    if (entry->side_nesting == -1 
		|| side_nesting < entry->side_nesting) {
		entry->side_nesting = side_nesting;
		resolved = false;
	    }

	    if (!resolved)
	    switch (type) {
	    case RECORD:
		length = rna._word.record->_typeinfo->_item[TypeDescriptor::IDX_SIZE].ii;
		for (i = 0; i < length; i++) {
# ifndef RNA_TYPECHECK
		    Rna item (rna._word.record->_item [i]);
# else /* RNA_TYPECHECK */
		    Rna item (rna._word.record->_item [i], rna._word.record->_typeinfo->type(i));
# endif /* RNA_TYPECHECK */
		    scan_item (item, rna._word.record->_typeinfo->type(i), 
			       side_nesting, depth_nesting);
		}
		for (List * s = rna._word.record->_attr; s; s = s->_cdr) {
		    scan_item (s->_car, s->_type, 
			       side_nesting+1, depth_nesting);
		}
		break;
	    case ARRAY:
		length = rna._word.array->_length;
		for (i = 0; i < length; i++) {
# ifndef RNA_TYPECHECK
		    Rna item (rna._word.array->_item [i]);
# else /* RNA_TYPECHECK */
		    Rna item (rna._word.array->_item [i], rna._word.array->_type);
# endif /* RNA_TYPECHECK */
		    scan_item (item, rna._word.array->_type, 
			       side_nesting, depth_nesting);
		}
		break;
	    case STR:
		/* do nothing */;
	        break;
	    default:
		RNA_SHOULD_NOT_REACH_HERE;
	    }
	}
	break;
    default:
	RNA_SHOULD_NOT_REACH_HERE;
    }
}


/******************************************************************\
 *                                                                *
 * AsciiSerializer - 2. emit items                             *
 *                                                                *
\******************************************************************/

void
AsciiSerializer::write_item (Rna rna, Type type, Snippets & sns, 
			     Int side_nesting, Int depth_nesting)
{
    Context::Binding * entry;

# ifdef RNA_TYPECHECK
    RNA_ASSERT (rna._type == ANY || rna._type == type);
# endif /* RNA_TYPECHECK */

    switch (type) {
    case BOOL:
	sns.printf ("%s", rna._word.bb ? "true" : "false"); 
	break;
    case INT:
	sns.printf ("%d", rna._word.ii); 
	break;
    case FLOAT:
	sns.printf ("%g", rna._word.ff); 
	break;
    case PTR: 
	sns.printf ("<%p>", rna._word.pp); 
	break;
    case STR:
	if (rna._word.ss) {
	    sns.printf ("\"");
	    for (char * s = rna._word.ss; *s != '\0'; s++)
		switch (*s) {
		case '\'': case '\"':
		    sns.printf ("\\%c", *s);
		    break;
		case '\n':
		    sns.printf ("\n");
		    break;
		default:
		    if (isprint (*s))
			sns.printf ("%c", *s);
		    else
			sns.printf("\\x%2.2x", *s & 0xff);
		    break;
		}
	    sns.printf ("\"");
	} else 
	    sns.printf ("null");
	break;
    case RECORD:
    case ARRAY:
	if (rna._word.ss) {
	    entry = _context->reverse_lookup (rna, type);
	    
	    RNA_ASSERT (entry->side_nesting <= side_nesting);
	    RNA_ASSERT (entry->ref_count > 0);
	    if (!entry->resolved && side_nesting == entry->side_nesting) {
		_context->resolve (entry, rna._word, type);
		entry->type = type;

		if (entry->ref_count > 1)
		    sns.printf ("%s: ", entry->handle._word.ss);
		write_composite_item (rna, type, sns, 
				      side_nesting, depth_nesting);
	    } else {
		RNA_ASSERT (entry->type == type);
		sns.printf ("%s", entry->handle._word.ss);
	    }
	} else
	    switch (type) {
	    case STR: sns.printf ("null"); break;
	    case RECORD: sns.printf ("()"); break;
	    case ARRAY: sns.printf ("[]"); break;
	    default: RNA_SHOULD_NOT_REACH_HERE;
	    }
	break;
    default:
	RNA_SHOULD_NOT_REACH_HERE;
    }
}


void
AsciiSerializer::write_composite_item (Rna rna, Type type, Snippets & sns, 
				       Int side_nesting, Int depth_nesting)
{
    int i, length;

# ifdef RNA_TYPECHECK
    RNA_ASSERT (rna._type == ANY || rna._type == type);
# endif /* RNA_TYPECHECK */

    switch (type) {
    case STR: 
	sns.printf ("\"%s\"", rna._word.ss);
	break;
    case RECORD:
	sns.indent ("(");
	sns.printf ("%s", rna._word.record->_typeinfo->_item[TypeDescriptor::IDX_NAME].ss);
	length = rna._word.record->_typeinfo->_item[TypeDescriptor::IDX_SIZE].ii;
	for (i = 0; i < length; i++) {
	    sns.separator ();
# ifndef RNA_TYPECHECK
	    Rna item (rna._word.record->_item [i]);
# else /* RNA_TYPECHECK */
	    Rna item (rna._word.record->_item [i], rna._word.record->_typeinfo->type(i));
# endif /* RNA_TYPECHECK */
	    write_item (item, rna._word.record->_typeinfo->type(i),
			sns, side_nesting, depth_nesting);
	}
	write_attr_list (rna._word.record->_attr, 
			 sns, side_nesting, depth_nesting);
	sns.unindent (")");
	break;
    case ARRAY:
	sns.indent ("[");
	length = rna._word.array->_length;
	for (i = 0; i < length; i++) {
# ifndef RNA_TYPECHECK
	    Rna item (rna._word.array->_item [i]);
# else /* RNA_TYPECHECK */
	    Rna item (rna._word.array->_item [i], rna._word.array->_type);
# endif /* RNA_TYPECHECK */
	    write_item (item, rna._word.array->_type, 
			sns, side_nesting, depth_nesting);
	    if (i < length-1)
		sns.separator ();
	}
	sns.unindent ("]");
	break;
    default:
	RNA_SHOULD_NOT_REACH_HERE;
    }
}

static int
my_compare (const void * a, const void * b)
{
    List * s1 (*(List **) a);
    List * s2 (*(List **) b);
    
    return strcmp (s1->getTag (), s2->getTag ());
}

void 
AsciiSerializer::write_attr_list (List * list, Snippets & sns, 
				  Int side_nesting, Int depth_nesting)
{
    // count list elements
    int length = 0;
    for (List * s = list; s; s = s->_cdr)
	length++;

    // copy elements into an array
    List ** array = (List **) malloc ((length+1) * sizeof (List *));
    int i = 0;
    for (List * s = list; s; s = s->_cdr)
	if (strcmp (s->_tag, "position") != 0)
	    array [i++] = s;
	else
	    length--;
    array [i] = 0;
	
    // sort array alphabetically
    qsort (array, length, sizeof (List *), my_compare);

    // print contents in order
    for (i = 0; i < length; i++) {
	List * s = array [i];
	sns.separator ();
	sns.printf ("%s=", s->_tag);
	write_item (s->_car, s->_type, sns, side_nesting+1, depth_nesting);
    }

    free (array);
}

/******************************************************************\
 *                                                                *
 * AsciiSerializer - 3, handle snippets                        *
 *                                                                *
\******************************************************************/

AsciiSerializer::Snippets::Snippets (Stream * s)
    : mode (RETAINED),
      start (0),
      current (0),
      current_length (0),
      current_indent (0),
      stream (s)
{}

AsciiSerializer::Snippets::~Snippets ()
{
    flush ();
}

void 
AsciiSerializer::Snippets::indent (char * begin_marker)
{
    current_indent ++;
    printf ("%s", begin_marker);
}

void 
AsciiSerializer::Snippets::unindent (char * end_marker)
{
    printf ("%s", end_marker);
    current_indent --;
    RNA_ASSERT (current_indent >= 0);
}

void
AsciiSerializer::Snippets::separator ()
{
    stream->printf ("\n%*s", 4*current_indent, "");
    stream->flush ();
}

void 
AsciiSerializer::Snippets::printf (const char * format, ...) 
{
    va_list ap;
    
    va_start (ap, format);

    stream->vprintf (format, ap);
    stream->flush ();
     
    va_end (ap);
}

void 
AsciiSerializer::Snippets::flush ()
{
}
