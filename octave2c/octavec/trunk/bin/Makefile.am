include $(top_srcdir)/Makefile.xt
include $(top_srcdir)/Makefile.octave

SCFLAGS         = --main io-`basename $*` -O 2 --C-include "\"parse-octave.h\"" --format-check 0 #--statistics 2 

bin_PROGRAMS = \
	octave/program/frontend/octave-frontend \
	octave/program/frontend/pp-octave \
	octave/program/octavec

libexec_PROGRAMS = \
	octave/program/frontend/remove-implicit-assignments \
	octave/program/frontend/make-print-statements-explicit \
	octave/program/frontend/eliminate-side-effects \
	octave/program/frontend/pack-octave \
	octave/program/frontend/octave-format-checker \
	octave/program/frontend/clean-ast \
	octave/program/optimize/constant-folding \
	octave/program/optimize/constant-propagation \
	octave/program/optimize/copy-propagation \
	octave/program/optimize/partial-evaluation \
	octave/program/optimize/common-subexpression-elimination \
	octave/program/optimize/forward-substitution \
	octave/program/optimize/superopt \
	octave/program/optimize/dead-code-elimination \
	octave/program/optimize/dead-function-elimination \
	octave/program/optimize/share-constants \
	octave/program/type-inferencer/infer-types \
	octave/program/type-inferencer/calculate-max-types \
	octave/program/optimize/get-builtin-funcs \
	octave/program/backend/octave2c

octave_programs = \
  $(wildcard octave/program/*.str) \
  $(wildcard octave/program/frontend/*.str) \
  $(wildcard octave/program/optimize/*.str) \
  $(wildcard octave/program/type-inferencer/*.str) \
  $(wildcard octave/program/backend/*.str)

extra_octave_programs = \
  $(wildcard octave/program/*.meta) \
  $(wildcard octave/program/frontend/*.meta) \
  $(wildcard octave/program/optimize/*.meta) \
  $(wildcard octave/program/type-inferencer/*.meta) \
  $(wildcard octave/program/backend/*.meta)

octave_program_frontend_octave_frontend_LDADD                = ../src/liboctavec.la $(LDADD)  
nodist_octave_program_frontend_octave_frontend_SOURCES = octave/program/frontend/octave-frontend.c

octave_program_frontend_pp_octave_LDADD                      = ../src/liboctavec.la $(LDADD) 
nodist_octave_program_frontend_pp_octave_SOURCES = octave/program/frontend/pp-octave.c

octave_program_frontend_remove_implicit_assignments_LDADD    = ../src/liboctavec.la $(LDADD) 
nodist_octave_program_frontend_remove_implicit_assignments_SOURCES = octave/program/frontend/remove-implicit-assignments.c

octave_program_frontend_make_print_statements_explicit_LDADD = ../src/liboctavec.la $(LDADD)  
nodist_octave_program_frontend_make_print_statements_explicit_SOURCES = octave/program/frontend/make-print-statements-explicit.c

octave_program_frontend_eliminate_side_effects_LDADD         = ../src/liboctavec.la $(LDADD)  
nodist_octave_program_frontend_eliminate_side_effects_SOURCES = octave/program/frontend/eliminate-side-effects.c

octave_program_frontend_pack_octave_LDADD                    = ../src/liboctavec.la $(LDADD)  
nodist_octave_program_frontend_pack_octave_SOURCES = octave/program/frontend/pack-octave.c

octave_program_frontend_octave_format_checker_LDADD          = ../src/liboctavec.la $(LDADD)  
nodist_octave_program_frontend_octave_format_checker_SOURCES = octave/program/frontend/octave-format-checker.c

octave_program_frontend_clean_ast_LDADD                      = ../src/liboctavec.la $(LDADD) 
nodist_octave_program_frontend_clean_ast_SOURCES = octave/program/frontend/clean-ast.c


octave_program_optimize_constant_folding_LDADD     = ../src/liboctavec.la ../src/liboctave-eval.la  $(LDADD)  -loctavec -lparseoctave -lstratego-xtc
nodist_octave_program_optimize_constant_folding_SOURCES = octave/program/optimize/constant-folding.c

octave_program_optimize_constant_propagation_LDADD = ../src/liboctavec.la ../src/liboctave-eval.la $(LDADD) 
nodist_octave_program_optimize_constant_propagation_SOURCES = octave/program/optimize/constant-propagation.c


octave_program_optimize_get_builtin_funcs_LDADD = ../src/liboctavec.la ../src/liboctave-eval.la $(LDADD) 
nodist_octave_program_optimize_get_builtin_funcs_SOURCES = octave/program/optimize/get-builtin-funcs.c

octave_program_optimize_copy_propagation_LDADD = ../src/liboctavec.la ../src/liboctave-eval.la $(LDADD) 
nodist_octave_program_optimize_copy_propagation_SOURCES = octave/program/optimize/copy-propagation.c

octave_program_optimize_common_subexpression_elimination_LDADD = ../src/liboctavec.la ../src/liboctave-eval.la $(LDADD) 
nodist_octave_program_optimize_common_subexpression_elimination_SOURCES = octave/program/optimize/common-subexpression-elimination.c

octave_program_optimize_forward_substitution_LDADD = ../src/liboctavec.la ../src/liboctave-eval.la $(LDADD) 
nodist_octave_program_optimize_forward_substitution_SOURCES = octave/program/optimize/forward-substitution.c

octave_program_optimize_superopt_LDADD = ../src/liboctavec.la ../src/liboctave-eval.la $(LDADD) 
nodist_octave_program_optimize_superopt_SOURCES = octave/program/optimize/superopt.c

octave_program_optimize_share_constants_LDADD      = ../src/liboctavec.la ../src/liboctave-eval.la $(LDADD)  
nodist_octave_program_optimize_share_constants_SOURCES = octave/program/optimize/share-constants.c

octave_program_optimize_partial_evaluation_LDADD      = ../src/liboctavec.la ../src/liboctave-eval.la $(LDADD) 
nodist_octave_program_optimize_partial_evaluation_SOURCES = octave/program/optimize/partial-evaluation.c

octave_program_optimize_dead_code_elimination_LDADD      = ../src/liboctavec.la ../src/liboctave-eval.la $(LDADD) 
nodist_octave_program_optimize_dead_code_elimination_SOURCES = octave/program/optimize/dead-code-elimination.c

octave_program_optimize_dead_function_elimination_LDADD      = ../src/liboctavec.la ../src/liboctave-eval.la $(LDADD) 
nodist_octave_program_optimize_dead_function_elimination_SOURCES = octave/program/optimize/dead-function-elimination.c

octave_program_type_inferencer_infer_types_LDADD         = ../src/liboctavec.la $(LDADD) 
nodist_octave_program_type_inferencer_infer_types_SOURCES = octave/program/type-inferencer/infer-types.c

octave_program_type_inferencer_calculate_max_types_LDADD = ../src/liboctavec.la $(LDADD) 
nodist_octave_program_type_inferencer_calculate_max_types_SOURCES = octave/program/type-inferencer/calculate-max-types.c


octave_program_backend_octave2c_LDADD = ../src/liboctavec.la ../src/libo2c.la $(LDADD) 
nodist_octave_program_backend_octave2c_SOURCES = octave/program/backend/octave2c.c


octave_program_octavec_LDADD = ../src/liboctavec.la $(LDADD) 
nodist_octave_program_octavec_SOURCES = octave/program/octavec.c


STRINCLUDES = -I $(top_srcdir)/syntax -I . -I ../src

#LDADD      += ../src/liboctavec.la ../src/libo2c.la ../src/liboctave-eval.la ../src/liboctave-dataflow.la $(SSL_LIBS)

EXTRA_DIST = $(nobase_pkgdata_DATA) $(extra_octave_programs) $(octave_programs)
CLEANFILES = $(octave_programs:.str=.c) $(octave_programs:.str=.c.dep) 

include $(wildcard octave/program/*.dep)
include $(wildcard octave/program/backend/*.dep)
include $(wildcard octave/program/frontend/*.dep)
include $(wildcard octave/program/optimize/*.dep)
include $(wildcard octave/program/type-inferencer/*.dep)

