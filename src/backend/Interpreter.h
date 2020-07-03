#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>
#include <iostream>

#include "BaseVisitor.h"
#include "tree/nodes.h"
#include "backend/Scope.h"
#include "Object/objects.h"

class Interpreter : public BaseVisitor {
public:
	Interpreter();
	virtual ~Interpreter() = default;

	void interpret(const StmtList & tree);

	void execute(Statement * stmt);
	obj_ptr eval(Expression * expr);
	void execute_block(Block * block, scope_ptr sub_scope);

	void call(Func * func, ObjList && args);

	void visit(ExprStmt * expr_stmt) override;
	void visit(Literal * literal) override;
	void visit(Identifier * id) override;
	void visit(VarDecl * var_decl) override;
	void visit(Block * block) override;
	void visit(FuncDecl * func_decl) override;
	void visit(FuncCall * func_call) override;
	void visit(InfixOp * infix_op) override;
	void visit(IfExpression * if_expr) override;

private:
	obj_ptr value;
	scope_ptr scope;
};

#endif