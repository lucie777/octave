/* -*-c++-*- */

/* Copyright (c) 1998 Gordon Cichon et al. */
/* Distributed under the GNU GPL. */

/*
 * $Author: gordon $
 * $Date: 2003/07/16 20:18:50 $
 * $Source: /sys/cvs/rna/samples/addrbook.cc,v $
 * $Revision: 1.2 $
 */

#include <stdio.h>

#include <rna/rna.h>

main ()
{
    printf ("Hello, world!\n");
    
    // ------------------------------------------

    RNA::Initialization::Initialize ();

    // ------------------------------------------

    RNA::File * addrbook = new RNA::File ("addrbook.rna", "r");
    RNA::Memory * mem = new RNA::Memory;
    RNA::StrContext * context = new RNA::StrContext;

    RNA::AsciiSerializer * input = new RNA::AsciiSerializer;
    input->set_stream (addrbook);
    input->set_memory (mem);
    input->set_context (context);

    // ------------------------------------------

    RNA::File * file = new RNA::File;
    file->setFile (stdout);    
    setvbuf (stdout, 0, _IONBF, 0);

    RNA::AsciiSerializer * output = new RNA::AsciiSerializer;
    output->set_stream (file);
    output->set_memory (new RNA::Memory);
    output->set_context (new RNA::StrContext);

    // ------------------------------------------

    RNA::Rna typeinfos = input->read (RNA::ARRAY);
    printf ("TypeDescriptors read in:\n");
    output->write (typeinfos, RNA::ARRAY);
    printf ("\n\n");
    
    // ------------------------------------------

    RNA::Rna addresses = input->read (RNA::RECORD);
    printf ("Addresses read in:\n");
    output->write (addresses, RNA::RECORD);
    printf ("\n\n");
     
    // ------------------------------------------

    delete input;
    delete output;
    
    // ------------------------------------------

    return 0;
}
