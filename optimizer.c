#include "compilium.h"


int ConstantPropagation(struct Node *expr){
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
  } else {
    return false;
  }
  
  return true;
}

void OptimizeExpr(struct Node *expr){
  return 1;
}


{
  /* data */
};


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

      printf("Z %d\n", toplevel_expr->type);


      if (toplevel_expr->type == kASTExprStmt) {
        struct Node *expr = toplevel_expr->left;
        // for each ExprStmt
        printf("A %1s %d\n", expr->op->begin, expr->type);
        if (!expr || expr->type != kASTExpr) {
          continue;
        }
        printf("B %1s %d\n", expr->op->begin, expr->type);
        if (ConstantPropagation(expr) == true) {
          continue;
        }
        printf("C %1s %d\n", expr->op->begin, expr->type);
      } else if (toplevel_expr->type == kASTJumpStmt) {
        if(toplevel_expr->left) {
          ConstantPropagation(toplevel_expr->left);
      }
        if(toplevel_expr->right) {
          ConstantPropagation(toplevel_expr->right);
      }
      }
    }
  }

  // Show the result
  fprintf(stderr, "AST after optimization:\n");
  PrintASTNode(ast);
  fputs("Optimization end\n", stderr);
}


