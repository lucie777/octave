#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "feoct.hh"
#include "ov-usr-fcn.h"

void feoct_init (const char * output_filename, 
		 bool debug_mode, const char * output_format, const char * if_mode){

};

void feoct_record (octave_user_function *fcn){
  printf("------------PARSER: %s\n", fcn->name().c_str());
};

void feoct_end (){

};

