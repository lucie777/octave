#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "feoct_monit.hh"
#include "feoct.hh"
#include "ov-usr-fcn.h"

void feoct_init (){
  printf("------------INIT MAIN LOOP--------------------- \n");
};

void feoct_record (octave_user_function *fcn){
  printf("------------PARSER: %s\n", fcn->name().c_str());

  const char * monitFilename = (fcn->name()+".feoct").c_str();

  FILE * f = fopen (monitFilename, "w");
  tree_monit td (f);
  fcn->accept (td);
  fclose (f);
};

void feoct_end (){
  printf("------------END MAIN LOOP--------------------- \n");
};

