#include "compilium.h"

//
struct Node *CreateNodeFromValue(int value) {
  char s[12];
  snprintf(s, sizeof(s), "%d", value);
  char *ds = strdup(s);  // duplicate because s is allocated on the stack
  int line = 0;
  struct Node *node = AllocNode(kASTExpr);

  if (value < 0) {
    // マイナスだったら一度負じゃない部分のトークンを作る
    node->op = CreateNextToken(ds + 1, ds + 1, &line);
    node->op->begin--;
    node->op->length++;
  } else {
    node->op = CreateNextToken(ds, ds, &line);  // use ds here
  }

  return node;
}

// Strength Reduction
// は式を受け取り、可能であればよりコストの低い演算に書き換える
// 左辺または右辺が負の値である演算には対応していない
// @param expr: 式を表すノード。NULL なら何もせず 0 を返す。
void StrengthReduction(struct Node **exprp) {
  assert(exprp != NULL);
  struct Node *expr = *exprp;
  if (!expr || expr->type != kASTExpr) {
    return;
  }
  if (!expr->left || !expr->right) {
    return;
  }
  if (!expr->left->op || !expr->right->op) {
    return;
  }

  if (expr->right->op->token_type != kTokenIntegerConstant) {
    return;
  }
  int right_var = strtol(expr->right->op->begin, NULL, 10);

  if (strncmp(expr->op->begin, "/", expr->op->length) == 0) {
    if (right_var < 0) {
      return;
    }
    if (__builtin_popcount(right_var) != 1) {
      return;
    }
    int log2_right_var = __builtin_popcount(right_var - 1);
    expr->op->begin = strdup(">>");
    expr->op->length = strlen(">>");
    expr->right = CreateNodeFromValue(log2_right_var);
    return;
  }
  if (strncmp(expr->op->begin, "/=", expr->op->length) == 0) {
    if (right_var < 0) {
      return;
    }
    if (__builtin_popcount(right_var) != 1) {
      return;
    }
    int log2_right_var = __builtin_popcount(right_var - 1);
    expr->op->begin  = strdup(">>=");
    expr->op->length = strlen(">>=");
    expr->right = CreateNodeFromValue(log2_right_var);
    return;
  }
}

// ConstantPropagation は式を受け取り，左右が定数値であれば，
// 定数畳み込みを行い，式を定数式に書き換える。
//
// @param expr: 式を表すノード。NULL なら何もせず 0 を返す。
// @return 式が定数式に書き換わったら true
int ConstantPropagation(struct Node **exprp) {
  assert(exprp != NULL);
  struct Node *expr = *exprp;
  if (!expr || expr->type != kASTExpr) {
    return false;
  }

  if (!expr->right || !expr->right->op || expr->right->op->token_type != kTokenIntegerConstant) {
    return false;
  }
  int right_var = strtol(expr->right->op->begin, NULL, 10);
  if (!expr->left) {
    // 単項演算子
    int val;
    if (strncmp(expr->op->begin, "-", expr->op->length) == 0) {
      val =  - right_var;
    } else if (strncmp(expr->op->begin, "+", expr->op->length) == 0) {
      val = + right_var;
    } else {
      return false;
    }
    *exprp = CreateNodeFromValue(val);
    return true;
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
  // PrintASTNode(expr);

  int val;
  if (strncmp(expr->op->begin, "+", expr->op->length) == 0) {
    val = left_var + right_var;
  } else if (strncmp(expr->op->begin, "-", expr->op->length) == 0) {
    val = left_var - right_var;
  } else if (strncmp(expr->op->begin, "*", expr->op->length) == 0) {
    val = left_var * right_var;
  } else if (strncmp(expr->op->begin, "/", expr->op->length) == 0) {
    val = left_var / right_var;
  } else if (strncmp(expr->op->begin, "%", expr->op->length) == 0) {
    val = left_var % right_var;
  } else {
    return false;
  }
  *exprp = CreateNodeFromValue(val);
  return true;
}

int isRecursiveFunction(struct Node *fnp, struct Node *n) {
  if (n->type == kASTExprFuncCall) {
    Optimize(&n->left);
    Optimize(&n->arg_expr_list);
    return;
  }
  if (n->type == kASTExpr) {
    fprintf(stderr, "Optimize:\n");
    // PrintASTNode(n);
    Optimize(&n->left);
    Optimize(&n->right);
    // left と right が定数なら，ここで定数の計算
    ConstantPropagation(np);
    StrengthReduction(np);
    return;
  }
  if (n->type == kASTExprStmt || n->type == kASTJumpStmt) {
    if (n->left) {
      Optimize(&n->left);
    }
    if (n->right) {
      Optimize(&n->right);
    }
    return;
  }
  if (n->type == kASTList) {
    for (int l = 0; l < GetSizeOfList(n); l++) {
      struct Node *stmt = GetNodeAt(n, l);
      Optimize(&stmt);
    }
    return;
  }
  if (n->type == kASTForStmt) {
    Optimize(&n->body);
    return;
  }
  // Show the result
  fprintf(stderr, "AST after optimization:\n");
  // PrintASTNode(ast);
  // fputs("Optimization end\n", stderr);
}

void OptimizeRecursiveFunction(struct Node **fnp) {
  assert(fnp != NULL);
  struct Node *fn = *fnp;
  assert(fn != NULL);
  assert(fn->type == kASTFuncDef);

  isRecursiveFunction(Node, );
}

void Optimize(struct Node **np) {
  if (!np) {
    return;
  }
  struct Node *n = *np;
  if (!n) {
    return;
  }
  fprintf(stderr, "AST before optimization:\n");
  if (n->type == kASTFuncDef) {
    //関数の再起呼び出しの検知
    fprintf(stderr, "FuncDefName : %.*s\n", n->func_name_token->length, n->func_name_token->begin);
    Optimize(&n->func_body);
    return;
  }
  if (n->type == kASTExprFuncCall) {
    Optimize(&n->left);
    Optimize(&n->arg_expr_list);
    return;
  }
  if (n->type == kASTExpr) {
    fprintf(stderr, "Optimize:\n");
    // PrintASTNode(n);
    Optimize(&n->left);
    Optimize(&n->right);
    // left と right が定数なら，ここで定数の計算
    ConstantPropagation(np);
    StrengthReduction(np);
    return;
  }
  if (n->type == kASTExprStmt || n->type == kASTJumpStmt) {
    if (n->left) {
      Optimize(&n->left);
    }
    if (n->right) {
      Optimize(&n->right);
    }
    return;
  }
  if (n->type == kASTList) {
    for (int l = 0; l < GetSizeOfList(n); l++) {
      struct Node *stmt = GetNodeAt(n, l);
      Optimize(&stmt);
    }
    return;
  }
  if (n->type == kASTForStmt) {
    Optimize(&n->body);
    return;
  }
  // Show the result
  fprintf(stderr, "AST after optimization:\n");
  // PrintASTNode(ast);
  // fputs("Optimization end\n", stderr);
}
