#include "analyze_bc.hpp"

extern "C" {
  void R_init_benchmarkR(DllInfo *dll);
}

static const R_CallMethodDef callMethods[] = {
  {"analyze_bc", (DL_FUNC)&benchmark::analyze_bc, 1},
  {NULL, NULL, 0}
};


void R_init_benchmarkR(DllInfo *dll) {
  R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
