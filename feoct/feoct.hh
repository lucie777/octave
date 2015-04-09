#ifndef feoct_hh
#define feoct_hh

class octave_user_function;

void feoct_init (const char * output_filename, 
		 bool debug_mode, const char * output_format, const char * if_mode);

void feoct_record (octave_user_function *fcn);

void feoct_end ();

#endif

