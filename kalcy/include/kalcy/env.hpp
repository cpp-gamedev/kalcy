#pragma once
#include <kalcy/token.hpp>
#include <functional>
#include <span>
#include <unordered_map>
#include <variant>

namespace kalcy {
///
/// \brief Execution environment.
///
class Env {
  public:
	///
	/// \brief Callable function.
	///
	using Func = std::function<double(std::span<double const>)>;

	///
	/// \brief Funcs can throw an instance of this.
	///
	struct Mismatch {};

	///
	/// \brief Get a default instance (singleton).
	/// \returns const reference to a static default instance.
	///
	static auto get_default() -> Env const&;

	///
	/// \brief Populate environment with builtins.
	///
	explicit Env();

	///
	/// \brief Define a function.
	///
	void define(std::string_view name, Func func);
	///
	/// \brief Define a constant.
	///
	void define(std::string_view name, double constant);

	///
	/// \brief Invoke a defined function by name.
	/// \returns Return value of invoked function.
	/// \param name Name of function.
	/// \param args Arguments to function.
	/// \throws UndefinedSymbol, ArgsMismatch.
	///
	[[nodiscard]] auto invoke(Token name, std::span<double const> args) const noexcept(false) -> double;

	///
	/// \brief Dereference a defined constant by name.
	/// \returns Return value of constant.
	/// \param name Name of constant.
	/// \throws UndefinedSymbol if name is not defined as a constant.
	///
	[[nodiscard]] auto deref(Token name) const noexcept(false) -> double;

  private:
	using Symbol = std::variant<double, Func>;

	std::unordered_map<std::string_view, Symbol> m_table{};
};
} // namespace kalcy
