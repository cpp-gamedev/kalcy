#include <kalcy/expr.hpp>
#include <sstream>

namespace kalcy {
namespace {
struct ToString {
	std::stringstream str{};

	void operator()(expr::Literal const& expr) { str << expr.token.lexeme; }

	void operator()(expr::Unary const& expr) {
		str << Token::type_str_v[expr.op.type];
		std::visit(*this, *expr.right);
	}

	void operator()(expr::Binary const& expr) {
		str << "(";
		std::visit(*this, *expr.left);
		str << ' ' << Token::type_str_v[expr.op.type] << ' ';
		std::visit(*this, *expr.right);
		str << ")";
	}

	void operator()(expr::Call const& expr) {
		str << expr.callee.lexeme << "(";
		bool first{true};
		for (auto const& arg : expr.arguments) {
			if (!first) { str << ", "; }
			first = false;
			std::visit(*this, *arg);
		}
		str << ")";
	}

	void operator()(expr::Group const& expr) {
		str << "(";
		std::visit(*this, *expr.expr);
		str << ")";
	}
};
} // namespace
} // namespace kalcy

auto kalcy::to_string(Expr const& expr) -> std::string {
	auto ret = ToString{};
	std::visit(ret, expr);
	return ret.str.str();
}
