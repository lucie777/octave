// -*-c++-*-

// Gordon's source file

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined(WITH_ATERM)
extern "C" {
#include <aterm2.h>
}
#endif  // WITH_ATERM

#include "ov-usr-fcn.h"

#include "cp.h"
#include "cp-air.h"


#if defined(WITH_ATERM)

typedef ATbool (* aterm_output_fun) (ATerm, FILE *);

ATerm make_aterm (RNA::Rna, RNA::Type, RNA::IntContext *);
ATerm make_composite_aterm (RNA::Rna, RNA::Type, RNA::IntContext *);

#endif  // WITH_ATERM


RNA::Context * compiler_air::air = 0;

void
compiler_air::Initialize (RNA::Context * context)
{
    air = context;
}

compiler_air::output_format
compiler_air::str2fmt(const char * format_string)
{
    compiler_air::output_format format;

    if ( strcmp(format_string, "rna") == 0 )
        format = compiler_air::FMT_RNA;
    else if ( strcmp(format_string, "aterm_text") == 0 )
        format = compiler_air::FMT_ATERM_TEXT;
    else if ( strcmp(format_string, "aterm_baf") == 0 )
        format = compiler_air::FMT_ATERM_BAF;
    else if ( strcmp(format_string, "aterm_taf") == 0 )
        format = compiler_air::FMT_ATERM_TAF;
    else
        format = compiler_air::FMT_UNKNOWN;

    return format;
}

compiler_air::if_mode_t
compiler_air::str2ifmode(const char * format_string)
{
    compiler_air::if_mode_t if_mode;

    if ( strcmp(format_string, "tree") == 0 )
        if_mode = compiler_air::IF_MODE_TREE;
    else if ( strcmp(format_string, "list") == 0 )
        if_mode = compiler_air::IF_MODE_LIST;
    else
	RNA_SHOULD_NOT_REACH_HERE;

    return if_mode;
}

compiler_air::compiler_air (const char * output_filename, 
			    bool debug_mode1, output_format fmt, if_mode_t if_mode1)
{
    filename = strdup (output_filename);

    debug_mode = debug_mode1;
    format = fmt;
    if_mode = if_mode1;
    switch_mode = SWITCH_MODE_ON;

    RNA::File * file = new RNA::File (output_filename, "w");
    m = new RNA::Memory;
    RNA::Context * context = new RNA::StrContext;

    output = new RNA::AsciiSerializer ();
    output->set_stream (file);
    output->set_context (new RNA::StrContext (context));
    output->set_memory (m);

    function_memory = new RNA::Memory;
    function_memory->startArray (0, RNA::RECORD);
    count_functions = 0;

    src_filename=strdup("*unknown*");
}

void
compiler_air::compile_function (octave_user_function * fcn)
{
    RNA::Rna function (emit_octave_user_function (*fcn));
    function_memory->grow (function);
    count_functions ++;
}

compiler_air::~compiler_air ()
{
    RNA::Array * functions = function_memory->finishArray ();
    functions->setLength (count_functions);
    
    RNA::Rna program (TreeProgram::Create (function_memory, functions));

    if ( format == FMT_RNA ) {

        output->write (program, RNA::RECORD);

    } else if ( format == FMT_ATERM_TEXT ||
                format == FMT_ATERM_BAF ||
                format == FMT_ATERM_TAF ) {

#if defined(WITH_ATERM)

        aterm_output_fun output_fun;

        // return value mismatch in different output functions!
        switch ( format ) {
            case FMT_ATERM_TEXT: output_fun = ATwriteToTextFile;  break;
            case FMT_ATERM_BAF: RNA_SHOULD_NOT_REACH_HERE; /* output_fun = ATwriteToBinaryFile; */ break;
            case FMT_ATERM_TAF: RNA_SHOULD_NOT_REACH_HERE; break;
            default:RNA_SHOULD_NOT_REACH_HERE; 
        }

        RNA::IntContext * context = new RNA::IntContext ();
        ATerm term = make_aterm (program, RNA::RECORD, context);

        FILE * f = fopen (filename, "w");
        if (f != NULL) {
            int result = output_fun(term, f) > 0;
            if (!result) { RNA_SHOULD_NOT_REACH_HERE; }
            else fclose (f);
        }

#endif  // WITH_ATERM

    } else {
        RNA_SHOULD_NOT_REACH_HERE;
    }

    delete function_memory;
    count_functions = 0;
    
    delete output->get_stream ();
    delete m;
    delete output->get_context ();

    output->set_stream (0);
    output->set_context (0);
    output->set_memory (0);
    delete output;

    free (filename);
}


#if defined(WITH_ATERM)

using namespace RNA;

ATerm 
make_aterm (RNA::Rna rna, RNA::Type type, RNA::IntContext * context)
{
    ATerm result = (ATerm) 0;
    AFun fun;

    switch (type) {
        case BOOL: result = (ATerm) ATmakeInt (rna.readBool () ? 1 : 0);
        Case INT: result = (ATerm) ATmakeInt (rna.readInt ());
        Case FLOAT: result = (ATerm) ATmakeReal (rna.readFloat ());
        Case PTR:
            result = (ATerm) ATmakeInt ((int) rna.readPtr ());
        Case STR:
        case RECORD:
        case ARRAY:
            if (rna.isNull ()) {
		switch(type) {
		case STR: result = (ATerm) ATmakeAppl0(ATmakeAFun("", 0, ATtrue));
		Case RECORD: 
		    fun = ATmakeAFun ("", 0, ATfalse);
		    result = (ATerm) ATmakeAppl0(fun);
		Case ARRAY: result = (ATerm) ATmakeList0 ();
		Default: RNA_SHOULD_NOT_REACH_HERE;
		}
            } else {
                Context::Binding * entry = context->reverse_lookup (rna, type);

                if (entry->resolved) {
                    RNA_ASSERT (entry->type == type);
                    result = (ATerm) entry->handle.readInt ();
                } else {
                    context->resolve (entry, rna._word);
		
                    result = make_composite_aterm (rna, type, context);
                    entry->handle = Rna ((Int) result);
                    entry->type = type;
                }
            }
        Default:
            RNA_SHOULD_NOT_REACH_HERE;
    }

    return result;
}

ATerm
make_composite_aterm (RNA::Rna rna, RNA::Type type, RNA::IntContext * context)
{
    int i, length;

    Record * record;
    TypeDescriptor * td;
    RNA::Array * array;
    RNA::List * list;
    AFun fun;
    ATerm result = (ATerm) 0;
    ATermList at_list;

    switch (type) {

        case STR:
            result = (ATerm) ATmakeAppl0(ATmakeAFun(rna.readStr (), 0, ATtrue));

        Case RECORD:
            record = rna.readRecord ();
            td = record->getTypeInfo ();
            length = td->get_size ();
            list = td->getAttr()->find("at_name");
            char * at_fname;

            if ( list != NULL )
                at_fname = list->getCar().readStr();
            else
                at_fname = td->get_name().readStr();

            at_list = ATmakeList0 ();

	    for (i = length-1; i >= 0; i--) {
		Rna item(record->get(i));
		at_list = ATinsert(at_list,
                                    make_aterm(item, td->type(i), context));
	    }

            fun = ATmakeAFun (at_fname, length, ATfalse);
	    result = (ATerm) ATmakeApplList (fun, at_list);

            for ( list = record->getAttr(); list; list = list->getCdr() ) {
                char * at_label = list->getTag();
                ATerm at_anno = make_aterm (list->getCar(), list->getType(),
                                                                    context);
                ATsetAnnotation(result, ATparse(at_label), at_anno);
            }

        Case ARRAY:
            array = rna.readArray ();
            length = array->getLength ();

            at_list = ATmakeList0 ();
            for (i = length-1; i >= 0; i--) {
                RNA::Rna item (array->at (i));
                at_list = ATinsert (at_list, make_aterm (item, array->getType (), context));
            }

            result = (ATerm) at_list;

        Default:
            RNA_SHOULD_NOT_REACH_HERE;
    }

    return result;
}

#endif  // WITH_ATERM
