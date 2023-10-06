#pragma once
#include <kalcy/expr.hpp>
#include <kalcy/scanner.hpp>

namespace kalcy {
///
/// \brief Expression parser.
///
class Parser {
  public:
	///
	/// \brief Parse tokens into an expression.
	/// \param text text to parse.
	/// \returns Parsed expression (or null if no tokens).
	/// \throws ParseError.
	///
	/// Caller must ensure the source of text outlives all expressions parsed from it.
	///
	auto parse(std::string_view text) noexcept(false) -> UExpr;

  private:
	void reset(std::string_view text);

	[[nodiscard]] auto expression() -> UExpr;
	[[nodiscard]] auto sum() -> UExpr;
	[[nodiscard]] auto product() -> UExpr;
	[[nodiscard]] auto exponent() -> UExpr;
	[[nodiscard]] auto unary() -> UExpr;
	[[nodiscard]] auto call() -> UExpr;
	[[nodiscard]] auto primary() -> UExpr;

	template <typename... T>
	auto match(T... types) -> bool;
	auto consume(Token::Type type) noexcept(false) -> void;
	void advance();

	Scanner m_scanner{};

	Token m_previous{};
	Token m_current{};
	Token m_next{};
};
} // namespace kalcy
