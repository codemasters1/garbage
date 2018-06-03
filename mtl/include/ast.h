#ifndef AST_H
#define AST_H
#include <stdbool.h>
#include "types.h"

typedef enum {
	S_ASSIGN,
	S_DO,
	S_EMPTY,
	S_EXPR,
	S_FOR,
	S_FUNCTION,
	S_IF,
	S_RETURN,
	S_VAR,
	S_WHILE
} StatKind;

typedef enum {
	O_NONE,
	O_ADD,
	O_SUB,
	O_PREINC,
	O_PREDEC,
	O_POSTINC,
	O_POSTDEC
} ExprOperator;

typedef enum {
	E_UNARY,
	E_BINARY,
	E_TERNARY
} ExprKind;

struct Expr {
	ExprKind kind;
	ExprOperator op;
	union {
		struct {
			struct Expr * lvalue;
		} unary;
		
		struct {
			struct Expr * lvalue;
			struct Expr * rvalue;
		} binary;
	};
};

struct Statement {
	StatementKind kind;
	union {
		struct {
			Var * var;
			struct Expr * expr;
		} assignStat;
		
		struct {
			struct Expr * cond;
			struct Statement * ifBody;
			struct Statement * elseBody;
		} ifStat;
		
		struct {
			struct Expr * cond;
			struct Statement * body;
		} doWhileStat;
		
		struct {
			struct Statement * statement;
			struct Expr * before;
			struct Expr * after;
			struct Statement * body;
		} forStat;
		
		struct {
			struct Expr * expr;
		} returnStat;
		
		struct {
			struct Expr * expr;
		} exprStat;
		
		struct {
			// ..?
		} varStat;
		
		struct {
			Var * name;
			Var * args;
			struct Statement * body;
		} funcStat;
	};
	struct Statement * next;
};

typedef struct Expr Expr;
typedef struct Statement Statement;

Statement * newStatement(StatementKind kind);
Statement * newEmptyStat(void);
Statement * newIfStat(Expr * cond, Statement * ifBody, Statement * elseBody);
Statement * newDoOrWhileStat(bool useWhile, Expr * cond, Statement * body);
Statement * newReturnStat(Expr * expr);
Statement * newVarStat(void);
Statement * newFuncStat(Var * name, Var * args, Statement * body);

void print(Statement * root, int indent);

#endif