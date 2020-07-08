#ifndef EXPR_H
#define EXPR_H

#include "tree/Node.h"

struct Expr;
using expr_ptr = std::shared_ptr<Expr>;
using ExprList = std::vector<expr_ptr>;

enum class ExprType {
	Id,
	Call,
	If,
	Infix,
	Literal,
	// Get/Set are member access (`.`)
	Get,
	Set
};

struct Expr : Node {
	Expr(const Position & pos, ExprType type) : Node(pos), type(type) {}
	virtual ~Expr() = default;

	virtual void accept(BaseVisitor & visitor) = 0;

	ExprType type;
};

#endif