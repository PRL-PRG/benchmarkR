#include "analyze_bc.hpp"

#include <vector>

namespace benchmark
{

  const std::array<size_t, OPCOUNT> opcodeArgCount = {{
      0, // BCMISMATCH_OP
      0, // RETURN_OP
      1, // GOTO_OP
      2, // BRIFNOT_OP
      0, // POP_OP
      0, // DUP_OP
      0, // PRINTVALUE_OP
      2, // STARTLOOPCNTXT_OP
      1, // ENDLOOPCNTXT_OP
      0, // DOLOOPNEXT_OP
      0, // DOLOOPBREAK_OP
      3, // STARTFOR_OP
      1, // STEPFOR_OP
      0, // ENDFOR_OP
      0, // SETLOOPVAL_OP
      0, // INVISIBLE_OP
      1, // LDCONST_OP
      0, // LDNULL_OP
      0, // LDTRUE_OP
      0, // LDFALSE_OP
      1, // GETVAR_OP
      1, // DDVAL_OP
      1, // SETVAR_OP
      1, // GETFUN_OP
      1, // GETGLOBFUN_OP
      1, // GETSYMFUN_OP
      1, // GETBUILTIN_OP
      1, // GETINTLBUILTIN_OP
      0, // CHECKFUN_OP
      1, // MAKEPROM_OP
      0, // DOMISSING_OP
      1, // SETTAG_OP
      0, // DODOTS_OP
      0, // PUSHARG_OP
      1, // PUSHCONSTARG_OP
      0, // PUSHNULLARG_OP
      0, // PUSHTRUEARG_OP
      0, // PUSHFALSEARG_OP
      1, // CALL_OP
      1, // CALLBUILTIN_OP
      1, // CALLSPECIAL_OP
      1, // MAKECLOSURE_OP
      1, // UMINUS_OP
      1, // UPLUS_OP
      1, // ADD_OP
      1, // SUB_OP
      1, // MUL_OP
      1, // DIV_OP
      1, // EXPT_OP
      1, // SQRT_OP
      1, // EXP_OP
      1, // EQ_OP
      1, // NE_OP
      1, // LT_OP
      1, // LE_OP
      1, // GE_OP
      1, // GT_OP
      1, // AND_OP
      1, // OR_OP
      1, // NOT_OP
      0, // DOTSERR_OP
      1, // STARTASSIGN_OP
      1, // ENDASSIGN_OP
      2, // STARTSUBSET_OP
      0, // DFLTSUBSET_OP
      2, // STARTSUBASSIGN_OP
      0, // DFLTSUBASSIGN_OP
      2, // STARTC_OP
      0, // DFLTC_OP
      2, // STARTSUBSET2_OP
      0, // DFLTSUBSET2_OP
      2, // STARTSUBASSIGN2_OP
      0, // DFLTSUBASSIGN2_OP
      2, // DOLLAR_OP
      2, // DOLLARGETS_OP
      0, // ISNULL_OP
      0, // ISLOGICAL_OP
      0, // ISINTEGER_OP
      0, // ISDOUBLE_OP
      0, // ISCOMPLEX_OP
      0, // ISCHARACTER_OP
      0, // ISSYMBOL_OP
      0, // ISOBJECT_OP
      0, // ISNUMERIC_OP
      1, // VECSUBSET_OP
      1, // MATSUBSET_OP
      1, // VECSUBASSIGN_OP
      1, // MATSUBASSIGN_OP
      2, // AND1ST_OP
      1, // AND2ND_OP
      2, // OR1ST_OP
      1, // OR2ND_OP
      1, // GETVAR_MISSOK_OP
      1, // DDVAL_MISSOK_OP
      0, // VISIBLE_OP
      1, // SETVAR2_OP
      1, // STARTASSIGN2_OP
      1, // ENDASSIGN2_OP
      2, // SETTER_CALL_OP
      1, // GETTER_CALL_OP
      0, // SWAP_OP
      0, // DUP2ND_OP
      4, // SWITCH_OP
      0, // RETURNJMP_OP
      2, // STARTSUBSET_N_OP
      2, // STARTSUBASSIGN_N_OP
      1, // VECSUBSET2_OP
      1, // MATSUBSET2_OP
      1, // VECSUBASSIGN2_OP
      1, // MATSUBASSIGN2_OP
      2, // STARTSUBSET2_N_OP
      2, // STARTSUBASSIGN2_N_OP
      2, // SUBSET_N_OP
      2, // SUBSET2_N_OP
      2, // SUBASSIGN_N_OP
      2, // SUBASSIGN2_N_OP
      1, // LOG_OP
      1, // LOGBASE_OP
      2, // MATH1_OP
      2, // DOTCALL_OP
      1, // COLON_OP
      1, // SEQALONG_OP
      1, // SEQLEN_OP
      2, // BASEGUARD_OP
      0, // INCLNK_OP
      0, // DECLNK_OP
      1, // DECLNK_N_OP
      0, // INCLNKSTK_OP
      0  // DECLNKSTK_OP
  }};

  SEXP analyze_bc(SEXP bc)
  {
    if (TYPEOF(bc) != VECSXP)
    {
      Rf_error("Expected a vector of bytecode instructions");
    }

    // Code is the second element in the vector
    SEXP scode = VECTOR_ELT(bc, 1);
    int *code = INTEGER(scode);
    int n = Rf_length(scode);

    std::vector<Loop> loopStack;
    std::vector<Loop> detectedLoops;
    ;

    // 1st element is the version so we skip it
    for (int i = 1; i < n; i++)
    {
      switch (code[i])
      {
      case STARTFOR_OP:
      {
        Loop loop(LoopType::FOR, i);
        loopStack.push_back(loop);
      }
      break;

      case ENDFOR_OP:
      {
        if (loopStack.empty() || loopStack.back().type != LoopType::FOR)
        {
          Rf_error("Mismatched ENDFOR_OP");
        }
        auto &loop = loopStack.back();
        loop.nb_opcodes = i - loop.pc + 1;
        detectedLoops.push_back(loop);
        loopStack.pop_back();
      }
      break;

      default:
        break;
      }
      i += opcodeArgCount[code[i]]; // Skip the number of arguments for the opcode
    }

    if (!loopStack.empty())
    {
      Rprintf("Loop stack size: %zu\n", loopStack.size());
      for (const auto &loop : loopStack)
      {
        Rprintf("Unmatched loop start at pc %d of type %d\n", loop.pc, static_cast<int>(loop.type));
      }
      Rf_error("Unmatched loop start detected");
    }

    // Create a list to hold the detected loops
    SEXP result = PROTECT(Rf_allocVector(VECSXP, detectedLoops.size()));
    // Rf_setAttrib(result, R_NamesSymbol, Rf_allocVector(STRSXP, detectedLoops.size()));

    for (size_t i = 0; i < detectedLoops.size(); ++i)
    {
      const Loop &loop = detectedLoops[i];

      // Create a list for each loop
      SEXP loopList = PROTECT(Rf_allocVector(VECSXP, 2));

      // Set the type of the loop
      SEXP type = PROTECT(Rf_ScalarString(Rf_mkChar(loop.type == LoopType::FOR ? "for" : "unknown")));
      SET_VECTOR_ELT(loopList, 0, type);

      // Set the number of opcodes
      SEXP nbOpcodes = PROTECT(Rf_ScalarInteger(loop.nb_opcodes));
      SET_VECTOR_ELT(loopList, 1, nbOpcodes);

      // Set the loop list in the result
      SET_VECTOR_ELT(result, i, loopList);

      UNPROTECT(3); // Unprotect loopList, type, nbOpcodes
    }

    UNPROTECT(1); // Unprotect the result

    return result;
  }

}
