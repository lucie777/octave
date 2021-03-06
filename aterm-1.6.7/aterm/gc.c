/*{{{  includes */

#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include <setjmp.h>

#ifndef WIN32
#include <unistd.h>
#include <sys/times.h>
#endif

#include "_aterm.h"
#include "afun.h"
#include "memory.h"
#include "util.h"
#include "gc.h"
#include "debug.h"

/*}}}  */
/*{{{  globals */

char gc_id[] = "$Id: gc.c,v 1.42 2001/08/02 10:50:56 olivierp Exp $";

static ATerm *stackBot = NULL;

#define PRINT_GC_TIME           1
#define PRINT_GC_STATS          2

static int     flags               = 0;
int at_gc_count			   = 0;
static clock_t sweep_time[3]       = { 0, MYMAXINT, 0 };
static clock_t mark_time[3]        = { 0, MYMAXINT, 0 };
static int     stack_depth[3]      = { 0, MYMAXINT, 0 };
static int     reclaim_perc[3]     = { 0, MYMAXINT, 0 };
extern int     mark_stats[3];
extern int     nr_marks;



AFun at_parked_symbol = -1;

/*}}}  */

/*{{{  void AT_initGC(int argc, char *argv[], ATerm *bottomOfStack) */

/**
 * Initialize the garbage collector.
 */

void AT_initGC(int argc, char *argv[], ATerm *bottomOfStack)
{
  int i;

  stackBot = bottomOfStack;

  for(i=1; i<argc; i++) {
    if(streq(argv[i], "-at-print-gc-time"))
      flags |= PRINT_GC_TIME;
    else if(streq(argv[i], "-at-print-gc-info"))
      flags |= (PRINT_GC_TIME | PRINT_GC_STATS);
    else if(strcmp(argv[i], "-at-help") == 0) {
      fprintf(stderr, "    %-20s: print non-intrusive gc information "
	      "after execution\n", "-at-print-gc-time");
      fprintf(stderr, "    %-20s: print elaborate gc information "
	      "after execution\n", "-at-print-gc-info");
    }
  }
}

/*}}}  */
/*{{{  void AT_setBottomOfStack(ATerm *bottomOfStack) */

/**
 * This function can be used to change the bottom of the stack.
 * Note that we only have one application that uses this fuction:
 * the Java ATerm wrapper interface, because here the garbage collector
 * can be called from different (but synchronized) threads, so at
 * the start of any operation that could start the garbage collector,
 * the bottomOfStack must be adjusted to point to the stack of
 * the calling thread.
 */

void AT_setBottomOfStack(ATerm *bottomOfStack)
{
  stackBot = bottomOfStack;
}

/*}}}  */

/*{{{  ATerm *stack_top() */

/**
 * Find the top of the stack.
 */

ATerm *stack_top()
{
  ATerm topOfStack;
  ATerm *top = &topOfStack;

  return top;
}

/*}}}  */
/*{{{  static void mark_memory(ATerm *start, ATerm *stop) */

static void mark_memory(ATerm *start, ATerm *stop)
{
  ATerm *cur, real_term;
#ifdef AT_64BIT
  ATerm odd_term;
  AFun odd_sym;
#endif

  /* Traverse the stack */
  for(cur=start; cur<stop; cur++) {
    real_term = AT_isInsideValidTerm(*cur);
    if (real_term != NULL) {
      AT_markTerm(real_term);
    }

    if (AT_isValidSymbol((Symbol)*cur)) {
      AT_markSymbol((Symbol)*cur);
    }

#ifdef AT_64BIT
    odd_term = *((ATerm *)((MachineWord)cur)+4);
    real_term = AT_isInsideValidTerm(odd_term);
    if (real_term != NULL) {
      AT_markTerm(odd_term);
    }

    odd_sym = *((AFun *)((MachineWord)cur)+4);
    if (AT_isValidSymbol(odd_sym)) {
      AT_markSymbol(odd_sym);
    }
#endif
  }
}

/*}}}  */
/*{{{  void mark_phase() */

/**
 * Mark all terms reachable from the root set.
 */

#ifdef WIN32
void __cdecl mark_phase()
#else
void         mark_phase()
#endif
{
  int i, j;
  int stack_size;
  ATerm *stackTop;
  ATerm *start, *stop;
  ProtEntry *prot;

#ifdef AT_64BIT
  ATerm oddTerm;
  AFun oddSym;
#endif

#ifdef WIN32
  /*{{{  Process registers on WIN32 platform */

  unsigned int r_eax, r_ebx, r_ecx, r_edx, \
    r_esi, r_edi, r_esp, r_ebp;
  ATerm reg[8];

  __asm {
    /* Get the registers into local variables to check them
       for aterms later. */
    mov r_eax, eax
      mov r_ebx, ebx
      mov r_ecx, ecx
      mov r_edx, edx
      mov r_esi, esi
      mov r_edi, edi
      mov r_esp, esp
      mov r_ebp, ebp
  }
  /* Put the register-values into an array */
  reg[0] = (ATerm) r_eax;
  reg[1] = (ATerm) r_ebx;
  reg[2] = (ATerm) r_ecx;
  reg[3] = (ATerm) r_edx;
  reg[4] = (ATerm) r_esi;
  reg[5] = (ATerm) r_edi;
  reg[6] = (ATerm) r_esp;
  reg[7] = (ATerm) r_ebp;

  nr_stack_terms = 0;
  nr_stack_syms  = 0;
  nr_reg_terms   = 0;
  nr_reg_syms    = 0;

  for(i=0; i<8; i++) {
    real_term = AT_isInsideValidTerm(reg[i]);
    if (real_term != NULL) {
      AT_markTerm(real_term);
    }
    if (AT_isValidSymbol((Symbol)reg[i])) {
      AT_markSymbol((Symbol)reg[i]);
    }
  }

  /* The register variables are on the stack aswell
     I set them to zero so they won't be processed again when
     the stack is traversed. The reg-array is also in the stack
     but that will be adjusted later */
  r_eax = 0;
  r_ebx = 0;
  r_ecx = 0;
  r_edx = 0;
  r_esi = 0;
  r_edi = 0;
  r_esp = 0;
  r_ebp = 0;

  /*}}}  */
#else
  /*{{{  Process registers on UNIX platform */

  sigjmp_buf env;

  /* Traverse possible register variables */
  sigsetjmp(env,0);

  start = (ATerm *)env;
  stop  = ((ATerm *)(((char *)env) + sizeof(sigjmp_buf)));
  mark_memory(start, stop);

  /*}}}  */
#endif

  stackTop = stack_top();

  /*{{{  Mark the stack */

  start = MIN(stackTop, stackBot);
  stop  = MAX(stackTop, stackBot);

  stack_size = stop-start;
  STATS(stack_depth, stack_size);

  mark_memory(start, stop);

  /*}}}  */

  /*{{{  Traverse protected terms */

  /* Traverse protected terms */
  for(i=0; i<at_prot_table_size; i++) {
    ProtEntry *cur = at_prot_table[i];
    while(cur) {
      for(j=0; j<cur->size; j++) {
	if(cur->start[j])
	  AT_markTerm(cur->start[j]);
      }
      cur = cur->next;
    }
  }

  /*}}}  */
  /*{{{  Traverse protected memory */

  for (prot=at_prot_memory; prot != NULL; prot=prot->next) {
    mark_memory((ATerm *)prot->start, (ATerm *)(((char *)prot->start) + prot->size));
  }

  /*}}}  */

  /* Mark protected symbols */
  AT_markProtectedSymbols();

  /* Mark 'parked' symbol */
  if (AT_isValidSymbol(at_parked_symbol)) {
    AT_markSymbol(at_parked_symbol);
  }
}

/*}}}  */


/*{{{  void sweep_phase() */

/**
 * Sweep all unmarked terms into the appropriate free lists.
 */

void sweep_phase()
{
  int size, idx;
  int total = 0;
  int reclaiming = 0, perc;

  for(size=MIN_TERM_SIZE; size<MAX_TERM_SIZE; size++) {
    Block *block = at_blocks[size];
    int end = BLOCK_SIZE - (BLOCK_SIZE % size);

    while(block) {
      assert(block->size == size);
      for(idx=0; idx<end; idx+=size) {
	ATerm t = (ATerm)&(block->data[idx]);
	if(IS_MARKED(t->header))
	  CLR_MARK(t->header);
	else {
	  switch(ATgetType(t)) {
	    case AT_FREE:
	      break;
	    case AT_INT:
	    case AT_REAL:
	    case AT_APPL:
	    case AT_LIST:
	    case AT_PLACEHOLDER:
	    case AT_BLOB:
	      AT_freeTerm(size, t);
	      reclaiming++;
	      break;
	    case AT_SYMBOL:
	      AT_freeSymbol((SymEntry)t);
	      break;

	    default:
	      ATabort("panic in sweep phase\n");
	  }
	}
      }
      block = block->next_by_size;
    }
    total += at_nrblocks[size]*(BLOCK_SIZE/size);
  }
  perc = (100*reclaiming)/total;
  STATS(reclaim_perc, perc);
}

/*}}}  */

/*{{{  void AT_collect() */

/**
 * Collect all garbage
 */

#ifdef WIN32
void AT_collect(int size)
  /* The timing/STATS parts haven't been tested yet (on NT)
     but without the info things seem to work fine */
{
  clock_t start, mark, sweep;
  clock_t user;

  at_gc_count++;
  if (!silent)
  {
    fprintf(stderr, "collecting garbage..");
    fflush(stderr);
  }
  start = clock();
  mark_phase();
  mark = clock();
  user = mark - start;
  STATS(mark_time, user);

  sweep_phase();
  sweep = clock();
  user = sweep - mark;
  STATS(sweep_time, user);

  if (!silent)
    fprintf(stderr, "..\n");
}
#else
void AT_collect(int size)
{
  struct tms start, mark, sweep;
  clock_t user;

  at_gc_count++;
  if (!silent)
  {
    fprintf(stderr, "collecting garbage..");
    fflush(stderr);
  }
  times(&start);
  mark_phase();
  times(&mark);
  user = mark.tms_utime - start.tms_utime;
  STATS(mark_time, user);
  sweep_phase();
  times(&sweep);
  user = sweep.tms_utime - mark.tms_utime;
  STATS(sweep_time, user);

  if (!silent)
    fprintf(stderr, "..\n");
}
#endif

/*}}}  */

/*{{{  void AT_cleanupGC() */

/**
 * Print garbage collection information
 */

void AT_cleanupGC()
{
#ifdef WIN32
  if(flags & PRINT_GC_TIME) {
    fprintf(stderr, "%d garbage collects,\n", at_gc_count);
    fprintf(stderr, "(all statistics are printed min/avg/max)\n");
    if(at_gc_count > 0) {
      if(nr_marks > 0) {
	fprintf(stderr, "  mark stack needed: %d/%d/%d (%d marks)\n",
		mark_stats[IDX_MIN], 
		mark_stats[IDX_TOTAL]/nr_marks,
		mark_stats[IDX_MAX], nr_marks);
      }
      fprintf(stderr, "  marking  took %.2f/%.2f/%.2f seconds, total: %.2f\n",
	      ((double)mark_time[IDX_MIN])/(double)CLOCKS_PER_SEC,
	      (((double)mark_time[IDX_TOTAL])/(double)at_gc_count)/(double)CLOCKS_PER_SEC,
	      ((double)mark_time[IDX_MAX])/(double)CLOCKS_PER_SEC,
	      ((double)mark_time[IDX_TOTAL])/(double)CLOCKS_PER_SEC);
      fprintf(stderr, "  sweeping took %.2f/%.2f/%.2f total: %.2f\n",
	      ((double)sweep_time[IDX_MIN])/(double)CLOCKS_PER_SEC,
	      (((double)sweep_time[IDX_TOTAL])/(double)at_gc_count)/(double)CLOCKS_PER_SEC,
	      ((double)sweep_time[IDX_MAX])/(double)CLOCKS_PER_SEC,
	      ((double)sweep_time[IDX_TOTAL])/(double)CLOCKS_PER_SEC);
    }
    fprintf(stderr, "Note: WinNT times are absolute, and might be influenced by other processes.\n");
  }
#else
  if(flags & PRINT_GC_TIME) {
    fprintf(stderr, "%d garbage collects,\n", at_gc_count);
    fprintf(stderr, "(all statistics are printed min/avg/max)\n");
    if(at_gc_count > 0) {
      if(nr_marks > 0) {
	fprintf(stderr, "  mark stack needed: %d/%d/%d (%d marks)\n", 
		mark_stats[IDX_MIN], mark_stats[IDX_TOTAL]/nr_marks, 
		mark_stats[IDX_MAX], nr_marks);
      }
      fprintf(stderr, "  marking  took %.2f/%.2f/%.2f seconds, total: %.2f\n", 
	      ((double)mark_time[IDX_MIN])/(double)CLK_TCK,
	      (((double)mark_time[IDX_TOTAL])/(double)at_gc_count)/(double)CLK_TCK,
	      ((double)mark_time[IDX_MAX])/(double)CLK_TCK,
	      ((double)mark_time[IDX_TOTAL])/(double)CLK_TCK);
      fprintf(stderr, "  sweeping took %.2f/%.2f/%.2f seconds, total: %.2f\n", 
	      ((double)sweep_time[IDX_MIN])/(double)CLK_TCK,
	      (((double)sweep_time[IDX_TOTAL])/(double)at_gc_count)/(double)CLK_TCK,
	      ((double)sweep_time[IDX_MAX])/(double)CLK_TCK,
	      ((double)sweep_time[IDX_TOTAL])/(double)CLK_TCK);
    }
  }

#endif

  if(flags & PRINT_GC_STATS) {
    if(at_gc_count > 0) {
      fprintf(stderr, "\n  stack depth: %d/%d/%d words\n", 
	      stack_depth[IDX_MIN],  
	      stack_depth[IDX_TOTAL]/at_gc_count,
	      stack_depth[IDX_MAX]);
      fprintf(stderr, "\n  reclamation percentage: %d/%d/%d\n",
	      reclaim_perc[IDX_MIN],
	      reclaim_perc[IDX_TOTAL]/at_gc_count,
	      reclaim_perc[IDX_MAX]);
    }
  }
}

/*}}}  */
