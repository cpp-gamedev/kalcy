#pragma once
#include <kalcy/env.hpp>
#include <kalcy/expr.hpp>

namespace kalcy {
///
/// \brief Expression evaluator.
///
struct Eval {
	///
	/// \brief Execution environment.
	///
	std::reference_wrapper<Env const> env;

	///
	/// \brief Evaluate expr.
	/// \param expr Expression to evaluate.
	/// \returns Evaluated value.
	/// \throws Env exceptions, InvalidOperator, InternalError.
	///
	[[nodiscard]] auto operator()(Expr const& expr) const noexcept(false) -> double;
};

inline auto evaluate(Expr const& expr, Env const& env = Env::get_default()) { return Eval{env}(expr); }
} // namespace kalcy
