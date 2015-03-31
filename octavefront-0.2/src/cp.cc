// -*-c++-*-

// Gordon's source file

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ov-usr-fcn.h"

#include "cp.h"
//#include "cp-dump.h"
#include "cp-air.h"

bool do_compile = false;

const char * dump_filename = NULL;

compiler_air * active_compiler = 0;

void
init_compilation (const char * output_filename, 
		  bool debug_mode, const char * output_format, const char * if_mode)
{
    printf ("init_compilation: %s, %s\n", output_filename, output_format);

    RNA::Initialization::Initialize ();
    RNA::Context * context = 0;
    context = Mouse::TypeInitialize (context);
    context = Mouse::ValueInitialize (context);
    context = Mouse::TreeInitialize (context);

    compiler_air::Initialize (context);

    active_compiler = new compiler_air (output_filename,
					debug_mode,
                                        compiler_air::str2fmt(output_format),
					compiler_air::str2ifmode(if_mode));
}

void 
rest_of_compilation (octave_user_function *fcn)
{
    if (dump_filename || do_compile)
	printf ("rest_of_compilation: %s\n",
		fcn->function_name ().c_str ());

#if 0    
    if (dump_filename) {
	FILE * f = fopen (dump_filename, "w");
	tree_dump td (f);
	fcn->accept (td);
	fclose (f);
    }
#endif

    if (do_compile)
	active_compiler->compile_function (fcn);
}

void 
finish_compilation ()
{
    printf ("finish_compilation\n");

    delete active_compiler;
    active_compiler = 0;
}

