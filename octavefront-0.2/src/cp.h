// Compiler header file

#if !defined (octave_compiler_h)
#define octave_compiler_h 1

class octave_user_function;

// TRUE means we run inside the compiler.
// (--compile)
extern bool do_compile;

// Filename for syntax tree dump
// NULL means we don't dump the tree.
// (--dump-tree)
extern const char * dump_filename;

void init_compilation (const char * output_filename, 
		       bool debug_mode, const char * output_format, const char * if_mode);

void rest_of_compilation (octave_user_function *fcn);

void finish_compilation ();

#endif

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/

