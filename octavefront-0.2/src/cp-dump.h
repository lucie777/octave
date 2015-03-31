// Compiler header file

#if !defined (octave_compiler_dump_h)
#define octave_compiler_dump_h 1

#include "pt-walk.h"

class 
tree_dump : public tree_walker
{
public:

  tree_dump (FILE * output = stdout)
    : f (output), indent (0) { };

  ~tree_dump (void) { }

  // functions and statements
  void visit_octave_user_function (octave_user_function&);
  void visit_parameter_list (tree_parameter_list&);
  void visit_identifier (tree_identifier&);
  void visit_statement_list (tree_statement_list&);
  void visit_statement (tree_statement&);

  // symbols and comments
  void print_symbol_table (symbol_table&);
  void print_symbol (symbol_record&);
  void print_comment_list (octave_comment_list&);
  void print_comment (octave_comment_elt&);

  // commands
  void visit_break_command (tree_break_command&);
  void visit_continue_command (tree_continue_command&);
  void visit_return_command (tree_return_command&);
  void visit_no_op_command (tree_no_op_command&);

  void visit_simple_for_command (tree_simple_for_command&);
  void visit_complex_for_command (tree_complex_for_command&);
  void visit_while_command (tree_while_command&);
  void visit_do_until_command (tree_do_until_command&);

  void visit_if_command (tree_if_command&);
  void visit_if_command_list (tree_if_command_list&);
  void visit_if_clause (tree_if_clause&);
  void visit_switch_command (tree_switch_command&);
  void visit_switch_case_list (tree_switch_case_list&);
  void visit_switch_case (tree_switch_case&);

  void visit_decl_command (tree_decl_command&);
  void visit_decl_init_list (tree_decl_init_list&);
  void visit_decl_elt (tree_decl_elt&);

  void visit_plot_command (tree_plot_command&);
  void visit_try_catch_command (tree_try_catch_command&);
  void visit_unwind_protect_command (tree_unwind_protect_command&);

  // expressions
  void visit_binary_expression (tree_binary_expression&);
  void visit_colon_expression (tree_colon_expression&);
  void visit_constant (tree_constant&);
  void visit_index_expression (tree_index_expression&);
  void visit_argument_list (tree_argument_list&);
  void visit_simple_assignment (tree_simple_assignment&);
  void visit_multi_assignment (tree_multi_assignment&);
  void visit_indirect_ref (tree_indirect_ref&);
  void visit_matrix (tree_matrix&);
  void visit_cell (tree_cell&);
  void visit_postfix_expression (tree_postfix_expression&);
  void visit_prefix_expression (tree_prefix_expression&);
  

  // ***************

  void visit_octave_user_function_header (octave_user_function&) {}

  void visit_octave_user_function_trailer (octave_user_function&) {}

  void visit_plot_limits (plot_limits&) {}

  void visit_plot_range (plot_range&) {}

  void visit_return_list (tree_return_list&) {}

  void visit_subplot (subplot&) {}

  void visit_subplot_axes (subplot_axes&) {}

  void visit_subplot_list (subplot_list&) {}

  void visit_subplot_style (subplot_style&) {}

  void visit_subplot_using (subplot_using&) {}


private:

  FILE * f;

  // Current indentation.
  int indent;

  // Must create with an output stream!

  tree_dump (void);

  // No copying!

  tree_dump (const tree_dump&);

  tree_dump& operator = (const tree_dump&);
};

#endif

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/

