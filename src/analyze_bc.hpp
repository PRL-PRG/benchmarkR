#ifndef ANALYZE_BC_HPP
#define ANALYZE_BC_HPP

extern "C"
{
// FIXME: can we avoid this
#include <Rinternals.h>
}

#include <array>

namespace benchmark
{
  enum
  {
    BCMISMATCH_OP,
    RETURN_OP,
    GOTO_OP,
    BRIFNOT_OP,
    POP_OP,
    DUP_OP,
    PRINTVALUE_OP,
    STARTLOOPCNTXT_OP,
    ENDLOOPCNTXT_OP,
    DOLOOPNEXT_OP,
    DOLOOPBREAK_OP,
    STARTFOR_OP,
    STEPFOR_OP,
    ENDFOR_OP,
    SETLOOPVAL_OP,
    INVISIBLE_OP,
    LDCONST_OP,
    LDNULL_OP,
    LDTRUE_OP,
    LDFALSE_OP,
    GETVAR_OP,
    DDVAL_OP,
    SETVAR_OP,
    GETFUN_OP,
    GETGLOBFUN_OP,
    GETSYMFUN_OP,
    GETBUILTIN_OP,
    GETINTLBUILTIN_OP,
    CHECKFUN_OP,
    MAKEPROM_OP,
    DOMISSING_OP,
    SETTAG_OP,
    DODOTS_OP,
    PUSHARG_OP,
    PUSHCONSTARG_OP,
    PUSHNULLARG_OP,
    PUSHTRUEARG_OP,
    PUSHFALSEARG_OP,
    CALL_OP,
    CALLBUILTIN_OP,
    CALLSPECIAL_OP,
    MAKECLOSURE_OP,
    UMINUS_OP,
    UPLUS_OP,
    ADD_OP,
    SUB_OP,
    MUL_OP,
    DIV_OP,
    EXPT_OP,
    SQRT_OP,
    EXP_OP,
    EQ_OP,
    NE_OP,
    LT_OP,
    LE_OP,
    GE_OP,
    GT_OP,
    AND_OP,
    OR_OP,
    NOT_OP,
    DOTSERR_OP,
    STARTASSIGN_OP,
    ENDASSIGN_OP,
    STARTSUBSET_OP,
    DFLTSUBSET_OP,
    STARTSUBASSIGN_OP,
    DFLTSUBASSIGN_OP,
    STARTC_OP,
    DFLTC_OP,
    STARTSUBSET2_OP,
    DFLTSUBSET2_OP,
    STARTSUBASSIGN2_OP,
    DFLTSUBASSIGN2_OP,
    DOLLAR_OP,
    DOLLARGETS_OP,
    ISNULL_OP,
    ISLOGICAL_OP,
    ISINTEGER_OP,
    ISDOUBLE_OP,
    ISCOMPLEX_OP,
    ISCHARACTER_OP,
    ISSYMBOL_OP,
    ISOBJECT_OP,
    ISNUMERIC_OP,
    VECSUBSET_OP,
    MATSUBSET_OP,
    VECSUBASSIGN_OP,
    MATSUBASSIGN_OP,
    AND1ST_OP,
    AND2ND_OP,
    OR1ST_OP,
    OR2ND_OP,
    GETVAR_MISSOK_OP,
    DDVAL_MISSOK_OP,
    VISIBLE_OP,
    SETVAR2_OP,
    STARTASSIGN2_OP,
    ENDASSIGN2_OP,
    SETTER_CALL_OP,
    GETTER_CALL_OP,
    SWAP_OP,
    DUP2ND_OP,
    SWITCH_OP,
    RETURNJMP_OP,
    STARTSUBSET_N_OP,
    STARTSUBASSIGN_N_OP,
    VECSUBSET2_OP,
    MATSUBSET2_OP,
    VECSUBASSIGN2_OP,
    MATSUBASSIGN2_OP,
    STARTSUBSET2_N_OP,
    STARTSUBASSIGN2_N_OP,
    SUBSET_N_OP,
    SUBSET2_N_OP,
    SUBASSIGN_N_OP,
    SUBASSIGN2_N_OP,
    LOG_OP,
    LOGBASE_OP,
    MATH1_OP,
    DOTCALL_OP,
    COLON_OP,
    SEQALONG_OP,
    SEQLEN_OP,
    BASEGUARD_OP,
    INCLNK_OP,
    DECLNK_OP,
    DECLNK_N_OP,
    INCLNKSTK_OP,
    DECLNKSTK_OP,
    OPCOUNT
  };

  // Static std::array for the number of arguments for each opcode
  extern const std::array<size_t, OPCOUNT> opcodeArgCount;


  enum class LoopType
  {
    FOR,
    WHILE,
    REPEAT,
  };

  struct Loop
  {
    LoopType type;
    int pc; // program counter of the starting opcode
    long nb_opcodes;
    Loop(LoopType type, int pc)
        : type(type), pc(pc), nb_opcodes(0) {}
  };

  SEXP analyze_bc(SEXP bc);
}

#endif
