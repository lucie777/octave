// -*-c++-*-

// Gordon's source file

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>

#include <string>
#include <strstream>
using namespace std;

#include "pt-walk.h"

#include "ov-usr-fcn.h"
#include "pt-misc.h"
#include "pt-decl.h"
#include "pt-stmt.h"

#include "pt-cmd.h"
#include "pt-jump.h"
#include "pt-loop.h"
#include "pt-select.h"

#include "pt-binop.h"
#include "pt-colon.h"
#include "pt-id.h"
#include "pt-const.h"
#include "pt-assign.h"
#include "pt-idx.h"
#include "pt-mat.h"
#include "pt-cell.h"
#include "pt-indir.h"
#include "pt-unop.h"

#include "pt-arg-list.h"
#include "comment-list.h"
#include "symtab.h"

#include "cp-dump.h"

void 
tree_dump::visit_octave_user_function (octave_user_function& item)
{
  fprintf (f, "%*s*(octave_user_function *)%8.8p%s\n", indent, "", 
	   &item,
	   item.is_system_fcn_file () ? " (system)" : "");
  indent += 2;
  fprintf (f, "%*sname=\"%s\"\n", indent, "", 
	   item.fcn_name.c_str ());
  fprintf (f, "%*sfilename=\"%s\"\n", indent, "", 
	   item.file_name.c_str ());
  indent -= 2;

  // incoming parameter list
  if (item.parameter_list ()) {
    indent += 2;
    fprintf (f, "%*sincoming parameter list:\n", indent, "");
    indent += 2;
    item.parameter_list ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // outgoing parameter list
  if (item.return_list ()) {
    indent += 2;
    fprintf (f, "%*soutgoing parameter list:\n", indent, "");
    indent += 2;
    item.return_list ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // function body
  if (item.body ()) {
    indent += 2;
    fprintf (f, "%*sfunction body:\n", indent, "");
    indent += 2;
    item.body ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // symbol table
  if (item.sym_tab) {
    indent += 2;
    fprintf (f, "%*ssymbol table:\n", indent, "");
    indent += 2;
    print_symbol_table (*item.sym_tab);
    indent -= 2;
    indent -= 2;
  }

  // comments
  if (item.lead_comm) {
    indent += 2;
    fprintf (f, "%*sleading comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.lead_comm);
    indent -= 2;
    indent -= 2;
  }
  if (item.trail_comm) {
    indent += 2;
    fprintf (f, "%*strailing comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.trail_comm);
    indent -= 2;
    indent -= 2;
  }
}

void 
tree_dump::visit_parameter_list (tree_parameter_list& item)
{
  fprintf (f, "%*s*(tree_parameter_list *)%8.8p%s\n", 
	   indent, "", &item,
	   item.takes_varargs() ? " (varargs)" : "");
  
  indent += 2;
  int i = 0;
  for (Pix p = item.first (); p; item.next (p)) {
    fprintf (f, "%*s%d. tree_parameter\n", 
	     indent, "",
	     i++);
    indent += 2;
    tree_identifier * elt = item (p);
    if (elt)
      elt->accept (*this);
    indent -= 2;
  }
  indent -= 2;
}

void 
tree_dump::visit_statement_list (tree_statement_list& item)
{
  fprintf (f, "%*s*(tree_statement_list *)%8.8p%s\n", 
	   indent, "", &item,
	   item.function_body ? " (body)" : "");
  
  indent += 2;
  int i = 0;
  for (Pix p = item.first (); p; item.next (p)) {
    fprintf (f, "%*s%d. tree_statement\n", 
	     indent, "",
	     i++);
    indent += 2;
    tree_statement *elt = item (p);
    if (elt)
      elt->accept (*this);
    indent -= 2;
  }
  indent -= 2;
}
void 
tree_dump::visit_statement (tree_statement& item) 
{
  fprintf (f, "%*s*(tree_statment *)%8.8p%s\n", 
	   indent, "", &item,
	   item.print_result () ? " (printing)" : "");

  // command
  if (item.command ()) {
    indent += 2;
    fprintf (f, "%*scommand:\n", indent, "");
    indent += 2;
    item.command ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }
  // expression
  if (item.expression ()) {
    indent += 2;
    fprintf (f, "%*sexpression:\n", indent, "");
    indent += 2;
    item.expression ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }
  // comments
  if (item.comment_text ()) {
    indent += 2;
    fprintf (f, "%*scomments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.comment_text ());
    indent -= 2;
    indent -= 2;
  }

}

void
tree_dump::print_comment_list (octave_comment_list& item)
{
  fprintf (f, "%*s*(octave_comment_list *)%8.8p\n", 
	   indent, "", &item);

  int i = 0;
  for (Pix p = item.first (); p; item.next (p)) {
    indent += 2;
    octave_comment_elt elt = item (p);
    print_comment (elt);
    indent -= 2;
  }
}

void 
tree_dump::print_comment (octave_comment_elt& item)
{
  fprintf (f, "%*s# %s\n", 
	   indent, "",
	   item.text ().c_str ());  
}

void
tree_dump::print_symbol_table (symbol_table& item)
{
  fprintf (f, "%*s*(symbol_table *)%8.8p\n", 
	   indent, "", &item);
  indent += 2;
  int j=0;
  for (int i = 0; i < item.table_size; i++) {
    for (symbol_record *ptr = item.table[i].next();
	 ptr; 
	 ptr = ptr->next ()) {
      fprintf (f, "%*s%d. symbol\n", 
	       indent, "", j++);
      print_symbol (*ptr);
    }
  }
  indent -= 2;
}

void
tree_dump::print_symbol (symbol_record& item)
{
  fprintf (f, "%*s*(symbol_record *)%8.8p name=\"%s\"\n", 
	   indent, "", &item, item.name ().c_str());
  if (item.definition) {
    indent += 2;
    if (item.definition->count != 1)
      fprintf (f, "%*ssymbol_def::count: %d\n", indent, "", 
	       item.definition->count);
    fprintf (f, "%*sdef.type_name():   %s\n", indent, "", 
	     item.definition->definition.type_name ().c_str());
    if (item.definition->definition.get_count () != 1)
      fprintf (f, "%*sdef.count():       %d\n", indent, "", 
	       item.definition->definition.get_count ());
    indent -= 2;
  }
}

void 
tree_dump::visit_break_command (tree_break_command& item) 
{
  fprintf (f, "%*s*(tree_break_command *)%8.8p\n", 
	   indent, "", &item);
}

void 
tree_dump::visit_continue_command (tree_continue_command& item) 
{
  fprintf (f, "%*s*(tree_continue_command *)%8.8p\n", 
	   indent, "", &item);
}

void 
tree_dump::visit_return_command (tree_return_command& item) 
{
  fprintf (f, "%*s*(tree_return_command *)%8.8p\n", 
	   indent, "", &item);
}

void 
tree_dump::visit_no_op_command (tree_no_op_command& item) 
{
  fprintf (f, "%*s*(tree_no_op_command *)%8.8p\n", 
	   indent, "", &item);
  indent += 2;
  fprintf (f, "%*s# %s\n", 
	   indent, "", item.original_command ().c_str ());
  indent -= 2;
}

void 
tree_dump::visit_simple_for_command (tree_simple_for_command& item) 
{
  fprintf (f, "%*s*(tree_simple_for_command *)%8.8p\n", indent, "", 
	   &item);

  // loop variable
  if (item.left_hand_side ()) {
    indent += 2;
    fprintf (f, "%*sloop variable:\n", indent, "");
    indent += 2;
    item.left_hand_side ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // loop range
  if (item.control_expr ()) {
    indent += 2;
    fprintf (f, "%*sloop range:\n", indent, "");
    indent += 2;
    item.control_expr ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // loop body
  if (item.body ()) {
    indent += 2;
    fprintf (f, "%*sloop body:\n", indent, "");
    indent += 2;
    item.body ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // comments
  if (item.leading_comment ()) {
    indent += 2;
    fprintf (f, "%*sleading comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.leading_comment ());
    indent -= 2;
    indent -= 2;
  }
  if (item.trailing_comment ()) {
    indent += 2;
    fprintf (f, "%*strailing comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.trailing_comment ());
    indent -= 2;
    indent -= 2;
  }  
}

void 
tree_dump::visit_complex_for_command (tree_complex_for_command& item) 
{
  fprintf (f, "%*s*(tree_complex_for_command *)%8.8p\n", indent, "", 
	   &item);

  // loop variable
  if (item.left_hand_side ()) {
    indent += 2;
    fprintf (f, "%*sloop variable:\n", indent, "");
    indent += 2;
    item.left_hand_side ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // loop range
  if (item.control_expr ()) {
    indent += 2;
    fprintf (f, "%*sloop range:\n", indent, "");
    indent += 2;
    item.control_expr ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // loop body
  if (item.body ()) {
    indent += 2;
    fprintf (f, "%*sloop body:\n", indent, "");
    indent += 2;
    item.body ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // comments
  if (item.leading_comment ()) {
    indent += 2;
    fprintf (f, "%*sleading comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.leading_comment ());
    indent -= 2;
    indent -= 2;
  }
  if (item.trailing_comment ()) {
    indent += 2;
    fprintf (f, "%*strailing comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.trailing_comment ());
    indent -= 2;
    indent -= 2;
  }  
}

void 
tree_dump::visit_while_command (tree_while_command& item) 
{
  fprintf (f, "%*s*(tree_while_command *)%8.8p\n", indent, "", 
	   &item);

  // condition
  if (item.condition ()) {
    indent += 2;
    fprintf (f, "%*sloop condition:\n", indent, "");
    indent += 2;
    item.condition ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // loop body
  if (item.body ()) {
    indent += 2;
    fprintf (f, "%*sloop body:\n", indent, "");
    indent += 2;
    item.body ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // comments
  if (item.leading_comment ()) {
    indent += 2;
    fprintf (f, "%*sleading comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.leading_comment ());
    indent -= 2;
    indent -= 2;
  }
  if (item.trailing_comment ()) {
    indent += 2;
    fprintf (f, "%*strailing comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.trailing_comment ());
    indent -= 2;
    indent -= 2;
  }  
}

void 
tree_dump::visit_do_until_command (tree_do_until_command& item) 
{
  fprintf (f, "%*s*(tree_do_until_command *)%8.8p\n", indent, "", 
	   &item);

  // condition
  if (item.condition ()) {
    indent += 2;
    fprintf (f, "%*sloop condition:\n", indent, "");
    indent += 2;
    item.condition ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // loop body
  if (item.body ()) {
    indent += 2;
    fprintf (f, "%*sloop body:\n", indent, "");
    indent += 2;
    item.body ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // comments
  if (item.leading_comment ()) {
    indent += 2;
    fprintf (f, "%*sleading comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.leading_comment ());
    indent -= 2;
    indent -= 2;
  }
  if (item.trailing_comment ()) {
    indent += 2;
    fprintf (f, "%*strailing comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.trailing_comment ());
    indent -= 2;
    indent -= 2;
  }  
}

void 
tree_dump::visit_if_command (tree_if_command& item) 
{
  fprintf (f, "%*s*(octave_if_command *)%8.8p\n", indent, "", 
	   &item);

  indent += 2;

  // clause list
  tree_if_command_list *list = item.cmd_list ();
  if (list)
    list->accept (*this);

  // comments
  if (item.leading_comment ()) {
    fprintf (f, "%*sleading comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.leading_comment ());
    indent -= 2;
  }
  if (item.trailing_comment ()) {
    fprintf (f, "%*strailing comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.trailing_comment ());
    indent -= 2;
  }  
  indent -= 2;
}

void 
tree_dump::visit_if_command_list (tree_if_command_list& item)
{
  int i = 0;
  for (Pix p = item.first (); p; item.next (p)) {
    tree_if_clause *elt = item (p);
    fprintf (f, "%*s%d. if_clause:\n", 
	     indent, "", i++);
    indent += 2;
    elt->accept (*this);
    indent -= 2;
  }
}

void 
tree_dump::visit_if_clause (tree_if_clause& item)
{
  fprintf (f, "%*s*(tree_if_clause *)%8.8p\n", indent, "", 
	   &item);

  // condition
  if (item.condition ()) {
    indent += 2;
    fprintf (f, "%*sif condition:\n", indent, "");
    indent += 2;
    item.condition ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // if body
  if (item.commands ()) {
    indent += 2;
    fprintf (f, "%*sif body:\n", indent, "");
    indent += 2;
    item.commands ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // comments
  if (item.leading_comment ()) {
    indent += 2;
    fprintf (f, "%*sleading comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.leading_comment ());
    indent -= 2;
    indent -= 2;
  }
}

void 
tree_dump::visit_switch_command (tree_switch_command& item)
{
  fprintf (f, "%*s*(tree_switch_command *)%8.8p\n", indent, "", 
	   &item);

  indent += 2;

  // condition
  if (item.switch_value ()) {
    indent += 2;
    fprintf (f, "%*sswitch condition:\n", indent, "");
    indent += 2;
    item.switch_value ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // clause list
  tree_switch_case_list *list = item.case_list ();
  if (list)
    list->accept (*this);

  // comments
  if (item.leading_comment ()) {
    fprintf (f, "%*sleading comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.leading_comment ());
    indent -= 2;
  }
  if (item.trailing_comment ()) {
    fprintf (f, "%*strailing comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.trailing_comment ());
    indent -= 2;
  }  
  indent -= 2;
}

void 
tree_dump::visit_switch_case_list (tree_switch_case_list& item)
{
  int i = 0;
  for (Pix p = item.first (); p; item.next (p)) {
    tree_switch_case *elt = item (p);
    fprintf (f, "%*s%d. switch_case:\n", 
	     indent, "", i++);
    indent += 2;
    elt->accept (*this);
    indent -= 2;
  }
}

void 
tree_dump::visit_switch_case (tree_switch_case& item)
{
  fprintf (f, "%*s*(tree_switch_case *)%8.8p\n", indent, "", 
	   &item);

  // condition
  if (item.case_label ()) {
    indent += 2;
    fprintf (f, "%*sswitch case label:\n", indent, "");
    indent += 2;
    item.case_label ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // if body
  if (item.commands ()) {
    indent += 2;
    fprintf (f, "%*sif body:\n", indent, "");
    indent += 2;
    item.commands ()->accept (*this);
    indent -= 2;
    indent -= 2;
  }

  // comments
  if (item.leading_comment ()) {
    indent += 2;
    fprintf (f, "%*sleading comments:\n", indent, "");
    indent += 2;
    print_comment_list (*item.leading_comment ());
    indent -= 2;
    indent -= 2;
  }
}

void 
tree_dump::visit_decl_command (tree_decl_command& item)
{
  fprintf (f, "%*s*(tree_decl_command *)%8.8p \n", indent, "", 
	   &item);
  
  indent += 2;
  fprintf (f, "%*sname=\"%s\"\n", indent, "", 
	   item.name ().c_str ());
  
  tree_decl_init_list *init_list = item.initializer_list ();
  if (init_list)
    init_list->accept (*this);
  indent -= 2;
}

void 
tree_dump::visit_decl_init_list (tree_decl_init_list& item)
{
  int i = 0;
  for (Pix p = item.first (); p; item.next (p)) {
    tree_decl_elt *elt = item (p);
    fprintf (f, "%*s%d. declaration:\n", 
	     indent, "", i++);
    indent += 2;
    elt->accept (*this);
    indent -= 2;
  }
}

void 
tree_dump::visit_decl_elt (tree_decl_elt& item)
{
  fprintf (f, "%*s*(tree_decl_elt *)%8.8p \n", indent, "", 
	   &item);

  // name
  indent += 2;
  fprintf (f, "%*sname:\n", indent, "");
  indent += 2;
  tree_identifier *id = item.ident ();
  if (id)
    id->accept (*this);
  indent -= 2;

  // value
  fprintf (f, "%*svalue:\n", indent, "");
  tree_expression *expr = item.expression ();
  indent += 2;
  if (expr)
    expr->accept (*this);
  indent -= 2;
  indent -= 2;
}

void 
tree_dump::visit_plot_command (tree_plot_command& item)
{
  fprintf (f, "%*s*(tree_plot_command *)%8.8p \n", indent, "", 
	   &item);

  fprintf (f, "%*s@@@\n", indent+2, "");
}

void 
tree_dump::visit_try_catch_command (tree_try_catch_command& item)
{
  fprintf (f, "%*s*(tree_try_catch_command *)%8.8p \n", indent, "", 
	   &item);

  fprintf (f, "%*s@@@\n", indent+2, "");
}

void 
tree_dump::visit_unwind_protect_command (tree_unwind_protect_command& item)
{
  fprintf (f, "%*s*(tree_unwind_protect_command *)%8.8p \n", indent, "", 
	   &item);

  fprintf (f, "%*s@@@\n", indent+2, "");
}

void 
tree_dump::visit_binary_expression (tree_binary_expression& item)
{
  fprintf (f, "%*s*(tree_binary_expression *)%8.8p \n", indent, "", 
	   &item);

  indent += 2;
  octave_value::binary_op op = item.etype;
  fprintf (f, "%*soperator: %d(%s)\n", indent, "",
	   op, octave_value::binary_op_as_string (op).c_str());
  
  fprintf (f, "%*sleft hand side:\n", indent, "");
  indent += 2;
  tree_expression *op1 = item.lhs ();
  if (op1)
    op1->accept (*this);
  indent -= 2;

  fprintf (f, "%*sright hand side:\n", indent, "");
  indent += 2;
  tree_expression *op2 = item.rhs ();
  if (op2)
    op2->accept (*this);
  indent -= 2;
  indent -= 2;
}

void 
tree_dump::visit_colon_expression (tree_colon_expression& item)
{
  fprintf (f, "%*s*(tree_colon_expression *)%8.8p \n", indent, "", 
	   &item);

  indent += 2;
  fprintf (f, "%*sbase value:\n", indent, "");
  indent += 2;
  tree_expression *op1 = item.base ();
  if (op1)
    op1->accept (*this);
  indent -= 2;

  fprintf (f, "%*sincrement:\n", indent, "");
  indent += 2;
  tree_expression *op2 = item.increment ();
  if (op2)
    op2->accept (*this);
  indent -= 2;

  fprintf (f, "%*slimit:\n", indent, "");
  indent += 2;
  tree_expression *op3 = item.limit ();
  if (op3)
    op3->accept (*this);
  indent -= 2;
  indent -= 2;
}

void 
tree_dump::visit_constant (tree_constant& item)
{
  fprintf (f, "%*s*(tree_constant *)%8.8p\n", 
	   indent, "", &item);
  
  indent += 2;
  ostrstream ss;
  item.print_raw (ss, true, true);
  ss << '\0';
  fprintf (f, "%*svalue: %s\n", indent, "", ss.str());
  indent -= 2;
}

void 
tree_dump::visit_identifier (tree_identifier& item)
{
  fprintf (f, "%*s*(tree_identifier *)%8.8p name=\"%s\" record=%8.8p\n", 
	   indent, "", &item,
	   item.name ().c_str (),
	   item.sym);
}

void 
tree_dump::visit_index_expression (tree_index_expression& item)
{
  fprintf (f, "%*s*(tree_index_expression *)%8.8p \n", indent, "", 
	   &item);

  indent += 2;
  fprintf (f, "%*sexpression:\n", indent, "");
  indent += 2;
  tree_expression *op1 = item.expression ();
  if (op1)
    op1->accept (*this);
  indent -= 2;

  fprintf (f, "%*sargument list:\n", indent, "");
  indent += 2;
  tree_argument_list *list = item.arg_list ();
  if (list)
    list->accept (*this);
  indent -= 2;
  indent -= 2;
}

void 
tree_dump::visit_argument_list (tree_argument_list& item) 
{
  int i = 0;
  for (Pix p = item.first (); p;  item.next (p)) {
    tree_expression *elt = item (p);
    fprintf (f, "%*s%d. argument:\n", 
	     indent, "", i++);
    indent += 2;
    elt->accept (*this);
    indent -= 2;
  }
}

void 
tree_dump::visit_simple_assignment (tree_simple_assignment& item) 
{
  fprintf (f, "%*s*(tree_simple_assignment *)%8.8p \n", indent, "", 
	   &item);
  
  indent += 2;
  octave_value::assign_op op = item.etype;
  fprintf (f, "%*soperator: %d(%s)\n", indent, "",
	   op, octave_value::assign_op_as_string (op).c_str());
  
  fprintf (f, "%*sleft hand side:\n", indent, "");
  indent += 2;
  tree_expression *op1 = item.left_hand_side ();
  if (op1)
    op1->accept (*this);
  indent -= 2;

  fprintf (f, "%*sright hand side:\n", indent, "");
  indent += 2;
  tree_expression *op2 = item.right_hand_side ();
  if (op2)
    op2->accept (*this);
  indent -= 2;
  indent -= 2;  
}

void 
tree_dump::visit_multi_assignment (tree_multi_assignment& item) 
{
  fprintf (f, "%*s*(tree_multi_assignment *)%8.8p \n", indent, "", 
	   &item);
  
  indent += 2;
  octave_value::assign_op op = item.etype;
  fprintf (f, "%*soperator: %d(%s)\n", indent, "",
	   op, octave_value::assign_op_as_string (op).c_str());
  

  fprintf (f, "%*sleft hand side:\n", indent, "");
  indent += 2;
  tree_argument_list *list = item.left_hand_side ();
  if (list)
    list->accept (*this);
  indent -= 2;
  indent -= 2;

  fprintf (f, "%*sright hand side:\n", indent, "");
  indent += 2;
  tree_expression *op2 = item.right_hand_side ();
  if (op2)
    op2->accept (*this);
  indent -= 2;
  indent -= 2;  
}

void 
tree_dump::visit_indirect_ref (tree_indirect_ref& item) 
{
  fprintf (f, "%*s*(tree_indirect_ref *)%8.8p \n", indent, "", 
	   &item);
  
  fprintf (f, "%*sexpression:\n", indent, "");
  indent += 2;
  tree_expression *op1 = item.expression ();
  if (op1)
    op1->accept (*this);
  indent -= 2;

  fprintf (f, "%*selement: %s\n", indent, "", 
	   item.elt_name().c_str());
  indent -= 2;  
}

void 
tree_dump::visit_matrix (tree_matrix& item) 
{
  fprintf (f, "%*s*(tree_matrix *)%8.8p\n", indent, "", 
	   &item);

  int i = 0;
  for (Pix p = item.first (); p; item.next (p)) {
    tree_argument_list *elt = item (p);
    if (elt) {
      fprintf (f, "%*s%d. row:\n", 
	       indent, "", i++);
      indent += 2;
      elt->accept (*this);
      indent -= 2;
    }
  }
}

void 
tree_dump::visit_cell (tree_cell& item) 
{
  fprintf (f, "%*s*(tree_cell *)%8.8p\n", indent, "", 
	   &item);

  int i = 0;
  for (Pix p = item.first (); p; item.next (p)) {
    tree_argument_list *elt = item (p);
    if (elt) {
      fprintf (f, "%*s%d. row:\n", 
	       indent, "", i++);
      indent += 2;
      elt->accept (*this);
      indent -= 2;
    }
  }
}

void  
tree_dump::visit_postfix_expression (tree_postfix_expression& item) 
{
  fprintf (f, "%*s*(tree_postfix_expression *)%8.8p \n", indent, "", 
	   &item);

  indent += 2;
  octave_value::unary_op op = item.etype;
  fprintf (f, "%*soperator: %d(%s)\n", indent, "",
	   op, octave_value::unary_op_as_string (op).c_str());
  
  fprintf (f, "%*sexpression:\n", indent, "");
  indent += 2;
  tree_expression *op1 = item.operand ();
  if (op1)
    op1->accept (*this);
  indent -= 2;
}

void  
tree_dump::visit_prefix_expression (tree_prefix_expression& item) 
{
  fprintf (f, "%*s*(tree_postfix_expression *)%8.8p \n", indent, "", 
	   &item);

  indent += 2;
  octave_value::unary_op op = item.etype;
  fprintf (f, "%*soperator: %d(%s)\n", indent, "",
	   op, octave_value::unary_op_as_string (op).c_str());
  
  fprintf (f, "%*sexpression:\n", indent, "");
  indent += 2;
  tree_expression *op1 = item.operand ();
  if (op1)
    op1->accept (*this);
  indent -= 2;
}

