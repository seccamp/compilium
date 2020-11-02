#include "compilium.h"

// ConstantPropagation は式を受け取り，左右が定数値であれば，
// 定数畳み込みを行い，式を定数式に書き換える。
//
// @param expr: 式を表すノード。NULL なら何もせず 0 を返す。
// @return 式が定数式に書き換わったら true
int ConstantPropagation(struct Node *expr) {
  if (!expr || expr->type != kASTExpr) {
    return false;
  }
  if (!expr->left || !expr->right) {
    return false;
  }
  if (!expr->left->op || !expr->right->op) {
    return false;
  }

  if (expr->left->op->token_type != kTokenIntegerConstant ||
      expr->right->op->token_type != kTokenIntegerConstant) {
    return false;
  }

  int left_var = strtol(expr->left->op->begin, NULL, 10);
  int right_var = strtol(expr->right->op->begin, NULL, 10);
  // PrintASTNode(expr);

  char s[12];

  if (strncmp(expr->op->begin, "+", expr->op->length) == 0) {
    snprintf(s, sizeof(s), "%d", left_var + right_var);
  } else if (strncmp(expr->op->begin, "-", expr->op->length) == 0) {
    snprintf(s, sizeof(s), "%d", left_var - right_var);
  } else if (strncmp(expr->op->begin, "*", expr->op->length) == 0) {
    snprintf(s, sizeof(s), "%d", left_var * right_var);
  } else if (strncmp(expr->op->begin, "/", expr->op->length) == 0) {
    snprintf(s, sizeof(s), "%d", left_var / right_var);
  } else if (strncmp(expr->op->begin, "%", expr->op->length) == 0) {
    snprintf(s, sizeof(s), "%d", left_var % right_var);
  } else {
    return false;
  }

  fprintf(stderr, "%d %.*s %d = %s\n", left_var, expr->op->length,
          expr->op->begin, right_var, s);

  fprintf(stderr, "s: %s\n", s);
  char *ds = strdup(s);  // duplicate because s is allocated on the stack
  int line = 0;
  expr->op = CreateNextToken(ds, ds, &line);  // use ds here
  expr->left = NULL;
  expr->right = NULL;
  // PrintASTNode(expr);

  return true;
}

void Optimize(struct Node *n) {
  // Show the base AST
  if (!n) {
    return;
  }
  fprintf(stderr, "AST before optimization:\n");
  // PrintASTNode(ast);

  // fputs("Optimization begin\n", stderr);
  // do something cool here...

  // Calculate constant expression on toplevel return
  // assert(IsASTList(ast));
  // for (int i = 0; i < GetSizeOfList(ast); i++) {
  //   struct Node *n = GetNodeAt(ast, i);
  //   if (n->type != kASTFuncDef) {
  //     continue;
  //   }
  //   // for each FuncDef
  //   assert(IsASTList(n->func_body));
  //   for (int k = 0; k < GetSizeOfList(n->func_body); k++) {
  //     struct Node *n = GetNodeAt(n->func_body, k);
  if (n->type == kASTFuncDef) {
    Optimize(n->func_body);
    return;
  }
  if (n->type == kASTExprFuncCall) {
    Optimize(n->left);
    Optimize(n->arg_expr_list);
    return;
  }
  if (n->type == kASTExpr) {
    fprintf(stderr, "Optimize:\n");
    // PrintASTNode(n);
    Optimize(n->left);
    Optimize(n->right);
    // left と right が定数なら，ここで定数の計算
    ConstantPropagation(n);
    return;
  }
  if (n->type == kASTExprStmt || n->type == kASTJumpStmt) {
    if (n->left) {
      Optimize(n->left);
    }
    if (n->right) {
      Optimize(n->right);
    }
    return;
  }
  if (n->type == kASTList) {
    for (int l = 0; l < GetSizeOfList(n); l++) {
      struct Node *stmt = GetNodeAt(n, l);
      Optimize(stmt);
    }
    return;
  }
  if (n->type == kASTForStmt) {
    Optimize(n->body);
    return;
  }
  // Show the result
  fprintf(stderr, "AST after optimization:\n");
  // PrintASTNode(ast);
  // fputs("Optimization end\n", stderr);
}
