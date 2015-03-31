// Compiler header file

#if !defined (octave_compiler_air_h)
#define octave_compiler_air_h 1

#include <rna/rna.h>

#include "mouse-tree.h"
#include "mouse-type.h"
#include "mouse-value.h"
using namespace Mouse;

#include "pt-walk.h"

class tree_command;
class tree_expression;

class compiler_air
{
public:

    typedef unsigned long output_format;

    typedef enum {
    	IF_MODE_TREE,
	IF_MODE_LIST
    } if_mode_t;

    typedef enum {
    	SWITCH_MODE_ON,
	SWITCH_MODE_OFF
    } switch_mode_t;

    static const output_format FMT_UNKNOWN = 0;
    static const output_format FMT_RNA = 1;
    static const output_format FMT_ATERM_TEXT = 2;
    static const output_format FMT_ATERM_BAF = 3;
    static const output_format FMT_ATERM_TAF = 4;

    static void Initialize (RNA::Context * air);
    static output_format str2fmt(const char * format_string);
    static if_mode_t str2ifmode(const char * format_string);

    compiler_air (const char * filename, 
		  bool debug_mode1 = false, 
		  output_format fmt = FMT_RNA,
		  if_mode_t if_mode1 = IF_MODE_LIST);
    ~compiler_air ();

    void compile_function (octave_user_function * fcn);

private:

    // used by constructor
    char * filename;
    bool debug_mode;
    output_format format;
    if_mode_t if_mode;
    switch_mode_t switch_mode;
    static RNA::Context * air;
    RNA::Memory * m;
    RNA::Serializer * output;
    char * src_filename;

    RNA::Memory * function_memory;
    int count_functions;

    // octave parse tree walking

    // functions and statements
    RNA::Rna emit_octave_user_function (octave_user_function&);
    RNA::Rna emit_parameter_list (tree_parameter_list&);
    RNA::Rna emit_identifier (tree_identifier&);
    RNA::Rna emit_statement_list (tree_statement_list&);
    RNA::Rna emit_statement (tree_statement&);

    // symbols and comments
    RNA::Rna emit_symbol_table (symbol_table&);
    RNA::Rna emit_symbol (symbol_record&);
    RNA::Array * emit_comment_list (octave_comment_list*);
    RNA::Rna emit_comment (octave_comment_elt&);

    // commands
    RNA::Rna emit_command (tree_command&);
    RNA::Rna emit_break_command (tree_break_command&);
    RNA::Rna emit_continue_command (tree_continue_command&);
    RNA::Rna emit_return_command (tree_return_command&);
    RNA::Rna emit_no_op_command (tree_no_op_command&);

    RNA::Rna emit_simple_for_command (tree_simple_for_command&);
    RNA::Rna emit_complex_for_command (tree_complex_for_command&);
    RNA::Rna emit_while_command (tree_while_command&);
    RNA::Rna emit_do_until_command (tree_do_until_command&);

    RNA::Rna emit_if_command (tree_if_command&);
    RNA::Rna emit_if_command_list (tree_if_command_list&);
    RNA::Rna emit_if_clause (tree_if_clause&);
    RNA::Rna emit_switch_command (tree_switch_command&);
    RNA::Rna emit_switch_case_list (tree_switch_case_list&, tree_expression&);
    RNA::Rna emit_switch_case (tree_switch_case&, tree_expression&);

    RNA::Rna emit_decl_command (tree_decl_command&);
    RNA::Rna emit_decl_init_list (tree_decl_init_list&);
    RNA::Rna emit_decl_elt (tree_decl_elt&);

    RNA::Rna emit_plot_command (tree_plot_command&);
    RNA::Rna emit_try_catch_command (tree_try_catch_command&);
    RNA::Rna emit_unwind_protect_command (tree_unwind_protect_command&);

    // expressions
    RNA::Rna emit_expression (tree_expression&);
    RNA::Rna emit_binary_expression (tree_binary_expression&);
    RNA::Rna emit_colon_expression (tree_colon_expression&);
    RNA::Rna emit_constant (tree_constant&);
    RNA::Rna emit_index_expression (tree_index_expression&);
    RNA::Rna emit_argument_list (tree_argument_list&);
    RNA::Rna emit_simple_assignment (tree_simple_assignment&);
    RNA::Rna emit_multi_assignment (tree_multi_assignment&);
    RNA::Rna emit_indirect_ref (tree_indirect_ref&);
    RNA::Rna emit_matrix (tree_matrix&);
    RNA::Rna emit_cell (tree_cell&);
    RNA::Rna emit_postfix_expression (tree_postfix_expression&);
    RNA::Rna emit_prefix_expression (tree_prefix_expression&);

    friend class tree_switch_emit_function;

    // helper functions
    RNA::Rna double2value (double);
    void cleanup_statement_list_rec (RNA::Array *, RNA::Memory *, int &);
};

#endif

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/

