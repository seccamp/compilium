#include "compilium.h"

void ConstantPropagation(struct Node *expr){
  if (strncmp(expr->op->begin, "+", expr->op->length) == 0) {
    int left_var = strtol(expr->left->op->begin, NULL, 10);
    int right_var = strtol(expr->right->op->begin, NULL, 10);
    PrintASTNode(expr);
    fprintf(stderr, "%d + %d = %d\n", left_var, right_var,
            left_var + right_var);
    char s[12];
    snprintf(s, sizeof(s), "%d", left_var + right_var);
    fprintf(stderr, "s: %s\n", s);
    char *ds = strdup(s);  // duplicate because s is allocated on the stack
    int line = 0;
    expr->op = CreateNextToken(ds, ds, &line);  // use ds here
    expr->left = NULL;
    expr->right = NULL;
    PrintASTNode(expr);
  } else if (strncmp(expr->op->begin, "-", expr->op->length) == 0) {
    int left_var = strtol(expr->left->op->begin, NULL, 10);
    int right_var = strtol(expr->right->op->begin, NULL, 10);
    PrintASTNode(expr);
    fprintf(stderr, "%d - %d = %d\n", left_var, right_var,
            left_var + right_var);
    char s[12];
    snprintf(s, sizeof(s), "%d", left_var - right_var);
    fprintf(stderr, "s: %s\n", s);
    char *ds = strdup(s);  // duplicate because s is allocated on the stack
    int line = 0;
    expr->op = CreateNextToken(ds, ds, &line);  // use ds here
    expr->left = NULL;
    expr->right = NULL;
    PrintASTNode(expr);
  }
}
void Optimize(struct Node *ast) {
  // Show the base AST
  fprintf(stderr, "AST before optimization:\n");
  PrintASTNode(ast);

  fputs("Optimization begin\n", stderr);
  // do something cool here...

  // Calculate constant expression on toplevel return
  assert(IsASTList(ast));
  for (int i = 0; i < GetSizeOfList(ast); i++) {
    struct Node *n = GetNodeAt(ast, i);
    if (n->type != kASTFuncDef) {
      continue;
    }
    // for each FuncDef
    assert(IsASTList(n->func_body));
    for (int k = 0; k < GetSizeOfList(n->func_body); k++) {
      struct Node *toplevel_expr = GetNodeAt(n->func_body, k);
      if (toplevel_expr->type != kASTExprStmt) {
        continue;
      }
      // for each ExprStmt
      struct Node *expr = toplevel_expr->left;
      if (!expr || expr->type != kASTExpr) {
        continue;
      }

      ConstantPropagation(expr);
    }
  }

  // Show the result
  fprintf(stderr, "AST after optimization:\n");
  PrintASTNode(ast);
  fputs("Optimization end\n", stderr);
}


