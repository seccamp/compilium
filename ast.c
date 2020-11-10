#include "compilium.h"

bool IsASTList(struct Node *n) { return n && n->type == kASTList; }
bool IsASTDeclOfTypedef(struct Node *n) {
  return (n && n->type == kASTDecl &&
          IsTokenWithType(GetNodeAt(n->op, 0), kTokenKwTypedef));
}

bool IsASTDeclOfExtern(struct Node *n) {
  return (n && n->type == kASTDecl &&
          IsTokenWithType(GetNodeAt(n->op, 0), kTokenKwExtern));
}

struct Node *AllocNode(enum NodeType type) {
  struct Node *node = calloc(1, sizeof(struct Node));
  node->type = type;
  return node;
}

struct Node *CreateASTBinOp(struct Node *t, struct Node *left,
                            struct Node *right) {
  if (!right) ErrorWithToken(t, "Expected expression after binary operator");
  struct Node *op = AllocNode(kASTExpr);
  op->op = t;
  op->left = left;
  op->right = right;
  return op;
}

struct Node *CreateASTUnaryPrefixOp(struct Node *t, struct Node *right) {
  if (!right) ErrorWithToken(t, "Expected expression after prefix operator");
  struct Node *op = AllocNode(kASTExpr);
  op->op = t;
  op->right = right;
  return op;
}

struct Node *CreateASTUnaryPostfixOp(struct Node *left, struct Node *t) {
  assert(IsToken(t));
  if (!left) ErrorWithToken(t, "Expected expression before prefix operator");
  struct Node *op = AllocNode(kASTExpr);
  op->op = t;
  op->left = left;
  return op;
}

struct Node *CreateASTExprStmt(struct Node *t, struct Node *left) {
  struct Node *op = AllocNode(kASTExprStmt);
  op->op = t;
  op->left = left;
  return op;
}

struct Node *CreateASTFuncDef(struct Node *func_decl, struct Node *func_body) {
  assert(func_decl && func_decl->type == kASTDecl);
  assert(IsASTList(func_body));
  struct Node *n = AllocNode(kASTFuncDef);
  n->func_body = func_body;
  struct Node *type = CreateTypeFromDecl(func_decl);
  assert(type);
  n->func_name_token = type->left;
  assert(IsToken(n->func_name_token));
  n->func_type = GetTypeWithoutAttr(type);
  assert(n->func_type && n->func_type->type == kTypeFunction);
  return n;
}

struct Node *CreateASTKeyValue(const char *key, struct Node *value) {
  struct Node *n = AllocNode(kASTKeyValue);
  n->key = key;
  n->value = value;
  return n;
}

struct Node *CreateASTLocalVar(int byte_offset, struct Node *var_type) {
  struct Node *n = AllocNode(kASTLocalVar);
  n->byte_offset = byte_offset;
  n->expr_type = var_type;
  return n;
}

struct Node *CreateTypeBase(struct Node *t) {
  struct Node *n = AllocNode(kTypeBase);
  n->op = t;
  return n;
}

struct Node *CreateTypeLValue(struct Node *type) {
  struct Node *n = AllocNode(kTypeLValue);
  n->right = type;
  return n;
}

struct Node *CreateTypePointer(struct Node *type) {
  struct Node *n = AllocNode(kTypePointer);
  n->right = type;
  return n;
}

struct Node *CreateTypeFunction(struct Node *return_type,
                                struct Node *arg_type_list) {
  assert(IsASTList(arg_type_list));
  struct Node *n = AllocNode(kTypeFunction);
  n->left = return_type;
  n->right = arg_type_list;
  return n;
}

struct Node *GetReturnTypeOfFunction(struct Node *func_type) {
  assert(func_type && func_type->type == kTypeFunction);
  return func_type->left;
}

struct Node *GetArgTypeList(struct Node *func_type) {
  assert(func_type && func_type->type == kTypeFunction);
  return func_type->right;
}

struct Node *CreateTypeStruct(struct Node *tag_token,
                              struct Node *struct_spec) {
  assert(IsToken(tag_token));
  struct Node *n = AllocNode(kTypeStruct);
  n->tag = tag_token;
  n->type_struct_spec = struct_spec;
  return n;
}

struct Node *CreateTypeAttrIdent(struct Node *ident_token, struct Node *type) {
  assert(ident_token && IsToken(ident_token));
  struct Node *n = AllocNode(kTypeAttrIdent);
  n->left = ident_token;
  n->right = type;
  return n;
}

struct Node *CreateASTIdent(struct Node *ident) {
  assert(IsToken(ident));
  struct Node *n = AllocNode(kASTIdent);
  n->op = ident;
  return n;
}

struct Node *CreateTypeArray(struct Node *type_of, struct Node *index_decl) {
  struct Node *n = AllocNode(kTypeArray);
  n->type_array_type_of = type_of;
  n->type_array_index_decl = index_decl;
  return n;
}

struct Node *CreateMacroReplacement(struct Node *args_tokens,
                                    struct Node *to_tokens) {
  struct Node *n = AllocNode(kNodeMacroReplacement);
  n->arg_expr_list = args_tokens;
  n->value = to_tokens;
  return n;
}

static void PrintPadding(int depth) {
  for (int i = 0; i < depth; i++) {
    fputc(' ', stderr);
  }
}

static void PrintASTNodeSub(struct Node *n, int depth);
static void PrintOptionalAttr(const char *name, struct Node *attr, int depth) {
  if (!attr) {
    return;
  }
  fprintf(stderr, ", %s=", name);
  PrintASTNodeSub(attr, depth);
}

static void PrintASTNodeSub(struct Node *n, int depth) {
  if (!n) {
    fprintf(stderr, "(null)");
    return;
  }
  if (IsToken(n)) {
    PrintTokenBrief(n);
    return;
  }
  if (n->type == kASTList) {
    fprintf(stderr, "[");
    if (GetSizeOfList(n) == 0) {
      fprintf(stderr, "]");
      return;
    }
    for (int i = 0; i < GetSizeOfList(n); i++) {
      fprintf(stderr, "%s\n", i ? "," : "");
      PrintPadding(depth + 1);
      PrintASTNodeSub(GetNodeAt(n, i), depth + 1);
    }
    fprintf(stderr, "\n");
    PrintPadding(depth);
    fprintf(stderr, "]");
    return;
  }
  if (n->type == kASTStructSpec) {
    fprintf(stderr, "StructSpec: ");
    PrintASTNodeSub(n->struct_member_dict, depth);
    return;
  }
  if (n->type == kASTKeyValue) {
    fprintf(stderr, "%s: ", n->key);
    PrintASTNodeSub(n->value, depth);
    return;
  }
  if (n->type == kNodeStructMember) {
    fprintf(stderr, "Member +%d: ", n->struct_member_ent_ofs);
    PrintASTNodeSub(n->struct_member_ent_type, depth);
    return;
  }
  if (n->type == kNodeMacroReplacement) {
    fprintf(stderr, "MacroReplacement<args: ");
    PrintASTNodeSub(n->arg_expr_list, depth);
    fprintf(stderr, ", rep: ");
    PrintTokenSequence(n->value);
    fprintf(stderr, ">");
    return;
  }
  if (n->type == kTypeBase) {
    PrintTokenStrToFile(n->op, stderr);
    return;
  }
  if (n->type == kTypeLValue) {
    fprintf(stderr, "lvalue<");
    PrintASTNodeSub(n->right, depth);
    fprintf(stderr, ">");
    return;
  }
  if (n->type == kTypePointer) {
    fprintf(stderr, "pointer_of<");
    PrintASTNodeSub(n->right, depth);
    fprintf(stderr, ">");
    return;
  }
  if (n->type == kTypeFunction) {
    fprintf(stderr, "function<returns: ");
    PrintASTNodeSub(n->left, depth);
    fprintf(stderr, ", args: ");
    PrintASTNodeSub(n->right, depth);
    fprintf(stderr, ">");
    return;
  }
  if (n->type == kTypeStruct) {
    fprintf(stderr, "struct<tag: ");
    PrintASTNodeSub(n->tag, depth);
    if (!n->type_struct_spec) {
      fprintf(stderr, ", incomplete");
    }
    fprintf(stderr, ">");
    return;
  }
  if (n->type == kTypeArray) {
    fprintf(stderr, "array_of<");
    PrintASTNodeSub(n->type_array_type_of, depth);
    fprintf(stderr, ">[");
    PrintASTNodeSub(n->type_array_index_decl, depth);
    fprintf(stderr, "]");
    return;
  }
  if (n->type == kTypeAttrIdent) {
    fputc('`', stderr);
    PrintTokenStrToFile(n->left, stderr);
    fputc('`', stderr);
    fprintf(stderr, " has a type: ");
    PrintASTNodeSub(n->right, depth);
    return;
  }
  if (n->type == kASTFuncDef) {
    fprintf(stderr, "FuncDef ");
    PrintASTNodeSub(n->func_name_token, depth);
    fprintf(stderr, " : ");
    PrintASTNodeSub(n->func_type, depth);
    fprintf(stderr, "{\n");
    PrintPadding(depth + 1);
    PrintASTNodeSub(n->func_body, depth + 1);
    fprintf(stderr, "\n");
    PrintPadding(depth);
    fprintf(stderr, "}");
    return;
  }
  if (n->type == kASTExprStmt) {
    PrintASTNodeSub(n->left, depth);
    fprintf(stderr, ";");
    return;
  }
  if (n->type == kASTExprFuncCall) {
    fprintf(stderr, "FuncCall<");
    PrintASTNodeSub(n->func_expr, depth);
    fprintf(stderr, ">(");
    PrintASTNodeSub(n->arg_expr_list, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == kASTJumpStmt) {
    fprintf(stderr, "JumpStmt(op=");
    PrintTokenBrief(n->op);
    if (n->right) {
      fprintf(stderr, ", R=");
      PrintASTNodeSub(n->right, depth + 1);
    }
    fprintf(stderr, ")");
    return;
  }
  if (n->type == kASTForStmt) {
    fprintf(stderr, "ForStmt(op=");
    PrintTokenBrief(n->op);
    PrintOptionalAttr("init", n->init, depth);
    PrintOptionalAttr("cond", n->cond, depth);
    PrintOptionalAttr("updt", n->updt, depth);
    PrintOptionalAttr("body", n->body, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == kASTWhileStmt) {
    fprintf(stderr, "WhileStmt(op=");
    PrintTokenBrief(n->op);
    PrintOptionalAttr("cond", n->init, depth);
    PrintOptionalAttr("body", n->body, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == kASTDecl) {
    fprintf(stderr, "ASTDecl(op=");
    PrintASTNodeSub(n->op, depth);
    PrintOptionalAttr("right", n->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == kASTDecltor) {
    fprintf(stderr, "ASTDecltor(left=");
    PrintASTNodeSub(n->left, depth);
    PrintOptionalAttr("right", n->right, depth);
    PrintOptionalAttr("decltor_init_expr", n->decltor_init_expr, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == kASTDirectDecltor) {
    fprintf(stderr, "ASTDirectDecltor(op=");
    PrintASTNodeSub(n->op, depth);
    PrintOptionalAttr("left", n->left, depth);
    PrintOptionalAttr("right", n->right, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == kASTSelectionStmt) {
    fprintf(stderr, "ASTSelectionStmt(op=");
    PrintASTNodeSub(n->op, depth);
    PrintOptionalAttr("cond", n->cond, depth);
    PrintOptionalAttr("true_stmt", n->if_true_stmt, depth);
    PrintOptionalAttr("else_stmt", n->if_else_stmt, depth);
    fprintf(stderr, ")");
    return;
  }
  if (n->type == kASTExpr) {
    fprintf(stderr, "(op=");
    if (n->op) PrintTokenBrief(n->op);
    if (n->expr_type) {
      fprintf(stderr, ":");
      PrintASTNodeSub(n->expr_type, depth + 1);
    }
    if (n->reg) fprintf(stderr, ", reg: %d", n->reg);
    PrintOptionalAttr("cond", n->cond, depth);
    PrintOptionalAttr("left", n->left, depth);
    PrintOptionalAttr("right", n->right, depth);
    fprintf(stderr, ")");
    return;
  }
  fprintf(stderr, "ASTPrintNotImplemented(type=%d: %s)", n->type,
          GetASTNodeTypeName(n));
  assert(false);
}

void PrintASTNode(struct Node *n) {
  PrintASTNodeSub(n, 0);
  fputc('\n', stderr);
}

const char *node_type_names[kNodeTypeSize];

void InitNodeTypeNames() {
  node_type_names[kNodeNone] = "kNodeNone";
  node_type_names[kNodeToken] = "kNodeToken";
  node_type_names[kNodeStructMember] = "kNodeStructMember";
  node_type_names[kNodeMacroReplacement] = "kNodeMacroReplacement";
  node_type_names[kASTExpr] = "kASTExpr";
  node_type_names[kASTExprFuncCall] = "kASTExprFuncCall";
  node_type_names[kASTList] = "kASTList";
  node_type_names[kASTExprStmt] = "kASTExprStmt";
  node_type_names[kASTJumpStmt] = "kASTJumpStmt";
  node_type_names[kASTSelectionStmt] = "kASTSelectionStmt";
  node_type_names[kASTIdent] = "kASTIdent";
  node_type_names[kASTDirectDecltor] = "kASTDirectDecltor";
  node_type_names[kASTDecltor] = "kASTDecltor";
  node_type_names[kASTDecl] = "kASTDecl";
  node_type_names[kASTForStmt] = "kASTForStmt";
  node_type_names[kASTWhileStmt] = "kASTWhileStmt";
  node_type_names[kASTFuncDef] = "kASTFuncDef";
  node_type_names[kASTKeyValue] = "kASTKeyValue";
  node_type_names[kASTLocalVar] = "kASTLocalVar";
  node_type_names[kASTStructSpec] = "kASTStructSpec";
  node_type_names[kTypeBase] = "kTypeBase";
  node_type_names[kTypeLValue] = "kTypeLValue";
  node_type_names[kTypePointer] = "kTypePointer";
  node_type_names[kTypeFunction] = "kTypeFunction";
  node_type_names[kTypeAttrIdent] = "kTypeAttrIdent";
  node_type_names[kTypeStruct] = "kTypeStruct";
  node_type_names[kTypeArray] = "kTypeArray";
}

const char *GetASTNodeTypeName(struct Node *n) {
  if (!n) {
    return "null";
  }
  if (n->type < 0 || kNodeTypeSize <= n->type || !node_type_names[n->type]) {
    return "???";
  }
  return node_type_names[n->type];
}
