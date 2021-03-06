#ifndef ASSIGN_H
#define ASSIGN_H

#include "tree/Expr/Expr.h"

namespace jc::tree {
    // Note: In Jacy assignment is an expression
    // Note: `Assign` is expression for assignment with identifier only !!!
    // It means, that it's only valid for `id = value`
    // For other assignments there's `SetExpr` and for array will be one separate too.

    struct Assign : Expr {
        id_ptr id;
        expr_ptr value;

        // Used by augmented assignment, like `+=` (here `augment` is operator `+`)
        parser::Token assign_op;

        Assign(id_ptr id, expr_ptr value, parser::Token assign_op)
                : Expr(id->pos, ExprType::Assign), id(id), value(value), assign_op(assign_op) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif