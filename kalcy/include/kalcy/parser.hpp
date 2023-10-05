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
	/// \brief Parse error.
	///
	struct Error {
		///
		/// \brief Token where error occurred.
		///
		Token token{};
		///
		/// \brief Expected token type.
		///
		Token::Type expected{};
	};

	///
	/// \brief Construct a parser for passed text.
	/// \param text text to parse.
	///
	/// Caller must ensure the source of text outlives all expressions parsed from it.
	///
	explicit Parser(std::string_view text);

	///
	/// \brief Parse tokens into an expression.
	/// \returns Parsed expression (or null if no tokens).
	/// \throws Error on parse error.
	///
	auto parse() noexcept(false) -> UExpr;

  private:
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

	Scanner m_scanner;

	Token m_previous{};
	Token m_current{};
	Token m_next{};
};
} // namespace kalcy
