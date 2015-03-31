/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/lib/debug.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define RNA_IMPLEMENTATION
#include <rna/rna.h>

#if 1
extern "C" {
#else
} // outwit "smart" syntax indentation
#endif

void 
RNA_Catch ()
{
    fprintf (stderr, "%s: RNA: gotcha!\n", 
	     program_invocation_short_name);
}

void 
pp (RNA_RNA rna, RNA_TYPE type)
{
    fpp (stdout, rna, type);
    fflush (stdout);
}

void
fpp (FILE * f, RNA_RNA rna, RNA_TYPE type)
{
#ifdef RNA_TYPECHECK
    if (rna.getType () != type) {
	fprintf (f, "Warning: requested type '%c' and actual type '%c' mismatch!\n",
		type, rna.getType ());
    }
#endif

    switch (type) {
    case ANY:
	fprintf (f, "\t*undefined*");
    Case BOOL:
	if (rna.readBool ())
	    fprintf (f, "\ttrue");
	else
	    fprintf (f, "\tfalse");
    Case CHAR: 
    {
	Char cc = rna.readChar ();
	fprintf (f, "\t\'%c\' (\'\\x%2.2x\')", cc, cc);
    } 
    Case INT:
    {
	Int ii = rna.readInt ();
	fprintf (f, "\t%d (0x%8.8x)", ii, ii);
    } 
    Case FLOAT:
    {
	Float ff = rna.readFloat ();
	fprintf (f, "\t%g (0x%8.8x)", ff, ff);
    } 
    Case PTR:
    {
	Ptr pp = rna.readPtr ();
	fprintf (f, "\t%p", pp);
    } 
    Case REF:
    {
	Ref rr = rna.readRef ();
	fprintf (f, "\t%p", rr);
    } 
    Case STR:
    {
	Str ss = rna.readStr ();
	fprintf (f, "\t\"%s\" (%p)", ss, ss);
    } 
    Case RECORD:
	if (!rna.isNull()) {
	    TypeDescriptor * typeinfo = rna.readRecord ()->getTypeInfo ();
	    Size size = typeinfo->get_size ();
	    fprintf (f, "\tRecord (%p): typeinfo=%p (%s) size=%d\n", 
		    rna.getWord ().ii, typeinfo, 
		    typeinfo->get_name ().readStr (), size);
	    for (int i = 0; i < size; i++) {
		Type type = typeinfo->type (i);
		fprintf (f, "\t%d: type=%c ", i, type);
		if (type != RECORD)
		    fpp (f, rna.readRecord ()->get (i), type);
		else 
		    fprintf (f, "\t%p\n", rna.readRecord ()->get (i).getWord ().pp);
	    }      
	} else {
	    fprintf (f, "\tRecord (NULL)\n");
	}
    Case ARRAY:
	if (!rna.isNull()) {
	    Size length = rna.readArray ()->getLength ();
	    Type type1 = rna.readArray ()->getType ();
	    fprintf (f, "\tArray (%p): length=%d type=%c\n", 
		    rna.getWord ().ii, length, type1);
	    for (int i = 0; i < length; i++) {
		fprintf (f, "\t%d: ", i);
		if (type1 != ARRAY)
		    fpp (f, rna.readArray ()->at (i), type1);
		else 
		    fprintf (f, "\t%p\n", rna.readArray ()->at (i).getWord ().pp);
	    }
	} else {
	    fprintf (f, "\tArray (NULL)\n");
	}
    Default:
	fprintf (f, "\tError: unknown type '%c'\n", type);
    }
    fprintf (f, "\n");
}

#if 0
{ // outwit "smart" syntax indentation
#else
}
#endif

int 
Context::compare_name (const void * a1, const void * a2)
{
    Binding * b1 = * (Binding **) a1;
    Binding * b2 = * (Binding **) a2;

    return strcmp (b1->handle.readStr (), b2->handle.readStr ());
}

int 
Context::compare_address (const void * a1, const void * a2)
{
    Binding * b1 = * (Binding **) a1;
    Binding * b2 = * (Binding **) a2;

    if (b1->definition.ii < b2->definition.ii)
	return -1;
    else if (b1->definition.ii > b2->definition.ii)
	return 1;
    else
	return 0;
}

void
Context::dump (FILE * f)
{
    Binding * b; 
    int i;

    int count = 0;
    for (b = _bindings; b; b = b->next)
	count++;

    Binding ** table = (Binding **) malloc (count * sizeof (Binding *));
    for (b = _bindings, i = 0; b; b = b->next, i++)
	table [i] = b;

    RNA_ASSERT (handle_type () == STR);

    qsort (table, count, sizeof (Binding *), compare_name);
    fprintf (f, "Symbol Table by Name:\n");
    fprintf (f, "---------------------\n");
    for (i = 0; i < count; i++)
	fprintf (f, "%s: %p (%c)\n", 
		 table [i]->handle.readStr (),
		 table [i]->definition.ii,
		 table [i]->type);
    fprintf (f, "\n");    

    qsort (table, count, sizeof (Binding *), compare_address);
    fprintf (f, "Symbol Table by Address:\n");
    fprintf (f, "------------------------\n");
    for (i = 0; i < count; i++)
	fprintf (f, "%p: %s (%c)\n", 
		 table [i]->definition.ii,
		 table [i]->handle.readStr (),
		 table [i]->type);
    fprintf (f, "\n");    

    free (table);
}

