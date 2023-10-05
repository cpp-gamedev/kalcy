#pragma once
#include <kalcy/token.hpp>
#include <memory>
#include <variant>
#include <vector>

namespace kalcy {
namespace expr {
struct Unary;
struct Binary;
struct Literal;
struct Group;
struct Call;
} // namespace expr

///
/// \brief Expression variant.
///
using Expr = std::variant<expr::Unary, expr::Binary, expr::Literal, expr::Group, expr::Call>;
using UExpr = std::unique_ptr<Expr>;

namespace expr {
///
/// \brief Unary expression.
///
struct Unary {
	Token op{};
	UExpr right{};
};

///
/// \brief Binary expression.
///
struct Binary {
	UExpr left{};
	Token op{};
	UExpr right{};
};

///
/// \brief Literal expression.
///
struct Literal {
	Token token{};
};

///
/// \brief Parenthesized expression.
///
struct Group {
	UExpr expr{};
};

///
/// \brief Function call expression.
///
struct Call {
	UExpr callee{};
	Token paren_l{};
	Token paren_r{};
	std::vector<UExpr> arguments{};
};
} // namespace expr
} // namespace kalcy
