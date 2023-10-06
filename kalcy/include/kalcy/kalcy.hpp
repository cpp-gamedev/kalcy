#pragma once
#include <kalcy/build_version.hpp>
#include <kalcy/env.hpp>
#include <kalcy/error.hpp>

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
	/// \param out_ast (optional) out parameter to build stringified AST to.
	/// \throws sub-type of Error.
	///
	[[nodiscard]] auto evaluate(std::string_view expression, std::string* out_ast = {}) const noexcept(false) -> double;

	///
	/// \brief Execution environment.
	///
	/// The environment is read-only during evaluation of expressions.
	///
	Env environment{};
};
} // namespace kalcy
