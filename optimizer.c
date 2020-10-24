#include "compilium.h"

void Optimize(struct Node *ast, struct SymbolEntry *ctx) {
  (void)ctx;  // Just ignore unused warning
  fputs("Optimization begin\n", stderr);
  // Show the base AST
  fprintf(stderr, "AST before optimization:\n");
  PrintASTNode(ast);
  // do something cool here...

  // Show the result
  fprintf(stderr, "AST after optimization:\n");
  PrintASTNode(ast);
  fputs("Optimization end\n", stderr);
}
