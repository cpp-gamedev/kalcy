#include <kalcy/error.hpp>
#include <kalcy/eval.hpp>
#include <kalcy/visitor.hpp>
#include <cassert>
#include <cmath>
#include <format>

namespace kalcy {
namespace {
struct Evaluator {
	Env const& env; // NOLINT
	double value{};

	auto operator()(expr::Literal const& literal) const -> double {
		if (literal.token.type == Token::Type::eIdentifier) { return env.deref(literal.token); }
		if (literal.token.type != Token::Type::eNumber) {
			throw InternalError{literal.token, std::format("internal error: unexpected literal token: '{}'", literal.token.lexeme)};
		}
		return literal.token.value;
	}

	auto operator()(expr::Unary const& unary) const -> double {
		if (unary.op.type != Token::Type::eMinus) { throw InvalidOperaor{unary.op}; }
		return -Evaluator{env}.evaluate(*unary.right);
	}

	auto operator()(expr::Binary const& binary) const -> double {
		auto const lhs = Evaluator{env}.evaluate(*binary.left);
		auto const rhs = Evaluator{env}.evaluate(*binary.right);

		switch (binary.op.type) {
		case Token::Type::ePlus: return lhs + rhs;
		case Token::Type::eMinus: return lhs - rhs;
		case Token::Type::eStar: return lhs * rhs;
		case Token::Type::eSlash: return lhs / rhs;
		case Token::Type::eCaret: return std::pow(lhs, rhs);
		default: break;
		}

		throw InvalidOperaor{binary.op};
	}

	auto operator()(expr::Call const& call) const -> double {
		auto args = std::vector<double>{};
		args.reserve(call.arguments.size());
		for (auto const& in : call.arguments) { args.push_back(Evaluator{env}.evaluate(*in)); }
		try {
			return env.invoke(call.callee, args);
		} catch (Env::Mismatch const& mismatched_args) { throw ArgsMismatch{call.callee, mismatched_args.argument_count}; }
	}

	auto operator()(expr::Group const& group) const -> double { return Evaluator{env}.evaluate(*group.expr); }

	[[nodiscard]] auto evaluate(Expr const& expr) -> double { return std::visit(*this, expr); }
};
} // namespace

auto Eval::operator()(Expr const& expr) const noexcept(false) -> double { return Evaluator{env}.evaluate(expr); }
} // namespace kalcy
