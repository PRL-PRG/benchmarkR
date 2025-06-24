dis_raw <- function(x) {
  if(inherits(x, "bytecode")) {
    .Internal(disassemble(x))
  }
  else if (typeof(x)  == "closure") {
    .Internal(disassemble(.Internal(bodyCode(x))))
  }
}


#' Analyze the bytecode statically
#'
#' @param bc bytecode to analyze
#' @export
analyze_bc <- function(bc) {
  dis <- dis_raw(bc)
  .Call(C_analyze_bc, dis)
}
