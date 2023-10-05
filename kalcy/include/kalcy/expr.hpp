#pragma once
#include <kalcy/token.hpp>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace kalcy {
namespace expr {
struct Literal;
struct Unary;
struct Binary;
struct Call;
struct Group;
} // namespace expr

///
/// \brief Expression variant.
///
using Expr = std::variant<expr::Literal, expr::Unary, expr::Binary, expr::Call, expr::Group>;
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
	Token callee{};
	Token paren_l{};
	Token paren_r{};
	std::vector<UExpr> arguments{};
};
} // namespace expr

[[nodiscard]] auto to_string(Expr const& expr) -> std::string;
} // namespace kalcy
