#pragma once
#include <kalcy/env.hpp>

namespace kalcy {
///
/// \brief Expression evaluator with an owned environment.
///
class Kalcy {
  public:
	///
	/// \brief Populate environment with builtins.
	///
	explicit Kalcy();

	///
	/// \brief Evaluate an expression.
	/// \param expression expression to evaluate.
	/// \throws sub-type of Error.
	///
	[[nodiscard]] auto evaluate(std::string_view expression) const noexcept(false) -> double;

	///
	/// \brief Execution environment.
	///
	/// The environment is read-only during evaluation of expressions.
	///
	Env environment{};
};
} // namespace kalcy
