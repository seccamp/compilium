#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LEN 64
#define MAX_INPUT_SIZE 8192

typedef enum {
  kIdentifier,
  kStringLiteral,
  kCharacterLiteral,
  kInteger,
  kPunctuator,
  kKeyword,
} TokenType;

typedef enum {
  kASTFuncDecl,
  kASTFuncDef,
  kASTCompStmt,
  kASTExprUnaryPreOp,
  kASTExprUnaryPostOp,
  kASTExprBinOp,
  kASTConstant,
  kASTExprStmt,
  kASTJumpStmt,
  kASTCondStmt,
  kASTIfStmt,
  kASTWhileStmt,
  kASTForStmt,
  kASTILOp,
  kASTList,
  kASTKeyword,
  kASTDecltor,
  kASTDirectDecltor,
  kASTIdent,
  kASTDecl,
  kASTParamDecl,
  kASTPointer,
  //
  kASTDict,
  kASTLocalVar,
  kASTLabel,
  //
  kNumOfASTType
} ASTType;

typedef enum {
  kILOpNop,
  kILOpAdd,
  kILOpSub,
  kILOpMul,
  kILOpDiv,
  kILOpMod,
  kILOpAnd,
  kILOpXor,
  kILOpOr,
  kILOpNot,
  kILOpNegate,
  kILOpLogicalAnd,
  kILOpLogicalOr,
  kILOpLogicalNot,
  kILOpShiftLeft,
  kILOpShiftRight,
  kILOpIncrement,
  kILOpDecrement,
  kILOpCmpG,
  kILOpCmpGE,
  kILOpCmpL,
  kILOpCmpLE,
  kILOpCmpE,
  kILOpCmpNE,
  kILOpLoadImm,
  kILOpLoadIdent,
  kILOpLoadArg,
  kILOpFuncBegin,
  kILOpFuncEnd,
  kILOpReturn,
  kILOpCall,
  kILOpWriteLocalVar,
  kILOpReadLocalVar,
  kILOpLabel,
  kILOpJmp,
  kILOpJmpIfZero,
  kILOpJmpIfNotZero,
  kILOpSetLogicalValue,
  //
  kNumOfILOpFunc
} ILOpType;

typedef enum {
  kKernelDarwin,
  kKernelLinux,
} KernelType;

typedef struct TOKEN_LIST TokenList;
typedef struct TOKEN_STREAM TokenStream;
typedef struct AST_LIST ASTList;
typedef struct AST_DICT ASTDict;
typedef struct CONTEXT Context;

typedef struct {
  int vreg_id;
  int save_label_num;
  int real_reg;
} Register;

typedef struct {
  char str[MAX_TOKEN_LEN + 1];
  TokenType type;
  const char *filename;
  int line;
} Token;

typedef struct {
  ASTType type;
} ASTNode;

typedef struct {
  ASTType type;
  const Token *token;
} ASTKeyword;

typedef struct {
  ASTType type;
  ASTList *root_list;
} ASTRoot;

typedef struct {
  ASTType type;
  ASTNode *type_and_name;
  ASTList *arg_list;
} ASTFuncDecl;

typedef struct {
  ASTType type;
  ASTList *stmt_list;
} ASTCompStmt;

typedef struct {
  ASTType type;
  const Token *op;
  ASTNode *expr;
} ASTExprUnaryPreOp;

typedef struct {
  ASTType type;
  const Token *op;
  ASTNode *expr;
} ASTExprUnaryPostOp;

typedef struct {
  ASTType type;
  const Token *op;
  ASTNode *left;
  ASTNode *right;
} ASTExprBinOp;

typedef struct {
  ASTType type;
  const Token *token;
} ASTConstant;

typedef struct {
  ASTType type;
  ASTNode *expr;
} ASTExprStmt;

typedef struct {
  ASTType type;
  ASTKeyword *kw;
  ASTNode *param;
} ASTJumpStmt;

typedef struct {
  ASTType type;
  ASTNode *cond_expr;
  ASTNode *true_expr;
  ASTNode *false_expr;
} ASTCondStmt;

typedef struct {
  ASTType type;
  ASTNode *cond_expr;
  ASTNode *true_stmt;
  ASTNode *false_stmt;
} ASTIfStmt;

typedef struct {
  ASTType type;
  ASTNode *cond_expr;
  ASTNode *body_stmt;
} ASTWhileStmt;

typedef struct {
  ASTType type;
  ASTNode *init_expr;
  ASTNode *cond_expr;
  ASTNode *updt_expr;
  ASTNode *body_stmt;
} ASTForStmt;

typedef struct {
  ASTType type;
  ILOpType op;
  Register *dst;
  Register *left;
  Register *right;
  ASTNode *ast_node;
} ASTILOp;

typedef struct {
  ASTType type;
  const Token *token;
} ASTIdent;

typedef struct AST_DIRECT_DECLTOR ASTDirectDecltor;
struct AST_DIRECT_DECLTOR {
  ASTType type;
  ASTDirectDecltor *direct_decltor;
  ASTNode *data;
};

typedef struct {
  ASTType type;
  ASTList *decl_specs;
  ASTList *init_decltors;
} ASTDecl;

typedef struct {
  ASTType type;
  ASTList *decl_specs;
  ASTNode *decltor;
} ASTParamDecl;

typedef struct AST_POINTER ASTPointer;
struct AST_POINTER {
  ASTType type;
  ASTPointer *pointer;
};

typedef struct {
  ASTType type;
  ASTPointer *pointer;
  ASTDirectDecltor *direct_decltor;
} ASTDecltor;

typedef struct {
  ASTType type;
  ASTList *decl_specs;
  ASTDecltor *decltor;
  ASTCompStmt *comp_stmt;
  Context *context;
} ASTFuncDef;

typedef struct {
  ASTType type;
  int ofs_in_stack;
  const char *name;
} ASTLocalVar;

typedef struct {
  ASTType type;
  int label_number;
} ASTLabel;

// @ast.c
void InitASTTypeName();
const char *GetASTTypeName(ASTNode *node);

ASTNode *ToASTNode(void *node);
#define DefToAST(type) AST##type *ToAST##type(ASTNode *node)
DefToAST(FuncDecl);
DefToAST(FuncDef);
DefToAST(CompStmt);
DefToAST(ExprUnaryPreOp);
DefToAST(ExprUnaryPostOp);
DefToAST(ExprBinOp);
DefToAST(Constant);
DefToAST(ExprStmt);
DefToAST(JumpStmt);
DefToAST(CondStmt);
DefToAST(IfStmt);
DefToAST(WhileStmt);
DefToAST(ForStmt);
DefToAST(ILOp);
DefToAST(List);
DefToAST(Keyword);
DefToAST(Decltor);
DefToAST(DirectDecltor);
DefToAST(Ident);
DefToAST(Decl);
DefToAST(ParamDecl);
DefToAST(Pointer);
DefToAST(Dict);
DefToAST(LocalVar);
DefToAST(Label);

#define DefAllocAST(type) AST##type *AllocAST##type()
DefAllocAST(FuncDecl);
DefAllocAST(FuncDef);
DefAllocAST(CompStmt);
DefAllocAST(ExprUnaryPreOp);
DefAllocAST(ExprUnaryPostOp);
DefAllocAST(ExprBinOp);
DefAllocAST(Constant);
DefAllocAST(ExprStmt);
DefAllocAST(JumpStmt);
DefAllocAST(CondStmt);
DefAllocAST(IfStmt);
DefAllocAST(WhileStmt);
DefAllocAST(ForStmt);
DefAllocAST(ILOp);
ASTList *AllocASTList(int capacity);
DefAllocAST(Keyword);
DefAllocAST(Decltor);
DefAllocAST(DirectDecltor);
DefAllocAST(Ident);
DefAllocAST(Decl);
DefAllocAST(ParamDecl);
DefAllocAST(Pointer);
ASTDict *AllocASTDict(int capacity);
ASTLocalVar *AllocASTLocalVar(int ofs_in_stack);
DefAllocAST(Label);

ASTNode *AllocAndInitASTConstant(const Token *token);
ASTIdent *AllocAndInitASTIdent(const Token *token);
ASTKeyword *AllocAndInitASTKeyword(const Token *token);
ASTNode *AllocAndInitASTExprBinOp(const Token *op, ASTNode *left,
                                  ASTNode *right);
const Token *GetIdentTokenFromDecltor(ASTDecltor *decltor);
const Token *GetIdentTokenFromDecltor(ASTDecltor *decltor);
const Token *GetFuncNameTokenFromFuncDef(ASTFuncDef *func_def);

void PrintASTNode(ASTNode *node, int depth);
void PushASTNodeToList(ASTList *list, ASTNode *node);
ASTNode *PopASTNodeFromList(ASTList *list);
ASTNode *GetASTNodeAt(const ASTList *list, int index);
int GetSizeOfASTList(const ASTList *list);
ASTNode *GetLastASTNode(const ASTList *list);

void AppendASTNodeToDict(ASTDict *dict, const char *key, ASTNode *node);
ASTNode *FindASTNodeInDict(ASTDict *dict, const char *key);
int GetSizeOfASTDict(const ASTDict *dict);

// @compilium.c
extern KernelType kernel_type;

// @error.c
void Error(const char *fmt, ...);

// @generate.c
void InitILOpTypeName();
const char *GetILOpTypeName(ILOpType type);
void GenerateCode(FILE *fp, ASTList *il, KernelType kernel_type);

// @il.c
int GetStackSizeForContext(const Context *context);
ASTList *GenerateIL(ASTNode *root);

// @parser.c
ASTNode *Parse(TokenList *tokens);

// @token.c
Token *AllocToken(const char *s, TokenType type);
Token *AllocTokenWithSubstring(const char *begin, const char *end,
                               TokenType type, const char *filename, int line);
int IsEqualToken(const Token *token, const char *s);
int IsTypeToken(const Token *token);
void SetNumOfTokens(int num_of_tokens);
TokenList *AllocTokenList(int capacity);
void AppendTokenToList(TokenList *list, const Token *token);
const Token *GetTokenAt(const TokenList *list, int index);
int GetSizeOfTokenList(const TokenList *list);
void SetSizeOfTokenList(TokenList *list, int size);
void DebugPrintToken(const Token *token);
void PrintToken(const Token *token);
void PrintTokenList(const TokenList *list);

TokenStream *AllocAndInitTokenStream(const TokenList *list);
const Token *PopToken(TokenStream *stream);
void UnpopToken(TokenStream *stream);
int GetStreamPos(TokenStream *stream);
int SeekStream(TokenStream *stream, int pos);
const Token *PeekToken(const TokenStream *stream);
int IsNextToken(TokenStream *stream, const char *str);
int IsNextTokenInList(TokenStream *stream, const char *list[]);
const Token *ConsumeToken(TokenStream *stream, const char *str);
const Token *ExpectToken(TokenStream *stream, const char *str);
void DebugPrintTokenStream(const char *s, const TokenStream *stream);

// @tokenizer.c
char *ReadFile(const char *file_name);
void Tokenize(TokenList *tokens, const char *p, const char *filename);
