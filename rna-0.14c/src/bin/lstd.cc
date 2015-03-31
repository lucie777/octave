/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2002/09/01 18:36:13 $
 * $Source: /sys/cvs/rna/src/bin/lstd.cc,v $
 * $Revision: 1.1.1.1 $
 */

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rna/rna.h"
using namespace RNA;

void 
usage (FILE * f, int exit_typeinfo)
{
    fprintf (f, "usage: %s [OPTIONS] FILE\n", 
	     program_invocation_short_name);
    fprintf (f, "    -i FILE      include other file\n");
    fprintf (f, "    -v           verbose output\n");
    fprintf (f, "    Lists the names of all Type-Descriptors in FILE\n");

    exit (exit_typeinfo);
}

int 
main (int argc, char ** argv)
{
    bool verbose = false;
    int c;
    if (argc == 1) 
	usage (stderr, EXIT_FAILURE);    

    Initialization::Initialize ();
    StrContext * wrapper = new StrContext ();

    for (opterr = 0; (c = getopt (argc, argv, "hi:v")) != -1;)
	switch (c) {
	case 'h':
	    usage (stderr, EXIT_SUCCESS);
	    break;
	case 'i':
	{
	    char * input_filename = optarg;
	    fprintf (stderr, "Including input file: %s\n", input_filename);
	    
	    File * file = new File (input_filename, "r");
	    Serializer * serializer = Serializer::Create (file);
	    serializer->set_context (wrapper);
	    while (!file->atEnd ())
		serializer->read (ANY);
	    serializer->set_context (0);	    
//	    delete serializer;
	}
	    break;
	Case 'v':
	    verbose = true;
	Default:
	    fprintf (stderr, "Unknown option -%c\n", c);
	    usage (stderr, EXIT_FAILURE);
	}

    StrContext * context = wrapper;
    for (int i = optind; i < argc; i++) {
	char * input_filename = argv[i];
	if (verbose)
	    fprintf (stderr, "Reading input file: %s\n", input_filename);
	
	File * file = new File (input_filename, "r");
	Serializer * serializer = Serializer::Create (file);
	serializer->set_context (new StrContext (context));
	while (!file->atEnd ())
	    serializer->read (ANY);
	context = (StrContext *) serializer->get_context ();
	serializer->set_context (0);
//	delete serializer;

	for (Context::Binding * binding = context->_bindings;
	     binding; binding = binding->next) {
	    char * label_name = binding->handle.readStr ();
	    if (binding->resolved) {
		if (binding->type == RECORD) {
		    Record * record = binding->definition.record;
		    if (record->getTypeInfo () == TypeDescriptor::TypeInfo ()) {
			TypeDescriptor * td = TypeDescriptor::Convert (record);
			char * type_name = td->get_name ().readStr ();
			if (strcmp (label_name, type_name) != 0)
			    fprintf (stderr, 
				     "warning: Record %s is declared with label %s\n",
				     type_name, label_name);
			if (verbose)
			    printf ("Record ");
			printf ("%s\n", type_name);
		    } else if  (record->getTypeInfo () == Variant::TypeInfo ()) {
			Variant * var = Variant::Convert (record);
			char * type_name = var->get_name ().readStr ();
			if (strcmp (label_name, type_name) != 0)
			    fprintf (stderr, 
				     "warning: Variant %s is declared with label %s\n",
				     type_name, label_name);
			if (verbose)
			    printf ("Variant ");
			printf ("%s\n", type_name);
		    }
		}
	    } else {
		if (verbose)
		    fprintf (stderr, "warning: unresolved label %s\n", label_name);
	    }
	}
    }

    return 0;
}
