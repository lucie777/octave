#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "feoct_monit.hh"
#include "feoct.hh"
#include "ov-usr-fcn.h"
#include "pt-all.h"

void feoct_init (){
  printf("------------INIT MAIN LOOP--------------------- \n");
};

void feoct_record (octave_user_function *fcn){
  printf("------------PARSER: %s\n", fcn->name().c_str());

  FILE *dumpfile;
  if(1){
    const char * monitFilename = (fcn->name()+".feoct").c_str();
    dumpfile = fopen (monitFilename, "w");
    tree_monit td (dumpfile);
    fcn->accept (td);
  }
  else{
    const char * monitFilename = (fcn->name()+".tpc").c_str();
    dumpfile = fopen (monitFilename, "w");

    std::ostringstream buf;
    tree_print_code tpc (buf);
    fcn->accept (tpc);
    fprintf (dumpfile, "%s\n", (buf.str()).c_str());
    //cout<<buf.str()<<endl;
  }
  fclose (dumpfile);
  printf("------------PRINT: %s\n", fcn->name().c_str());

};

void feoct_end (){
  printf("------------END MAIN LOOP--------------------- \n");
};

