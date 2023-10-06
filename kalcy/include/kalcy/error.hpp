#pragma once
#include <kalcy/token.hpp>
#include <stdexcept>

namespace kalcy {
struct Error : std::runtime_error {
	///
	/// \brief Token where error occurred.
	///
	Token token{};

	explicit Error(Token const token, std::string const& message) : std::runtime_error(message), token(token) {}

	[[nodiscard]] auto build_highlight(char higlight = '^') const -> std::string;
};

///
/// \brief Internal error.
///
struct InternalError : Error {
	using Error::Error;
};

///
/// \brief Parse error.
///
struct ParseError : Error {
	///
	/// \brief Expected token type.
	///
	Token::Type expected{};

	explicit ParseError(Token token);
	explicit ParseError(Token token, Token::Type expected);
};

struct UndefinedSymbol : Error {
	explicit UndefinedSymbol(Token token);
};

struct ArgsMismatch : Error {
	///
	/// \brief Count of arguments passed.
	///
	std::size_t argument_count{};

	explicit ArgsMismatch(Token token, std::size_t argument_count);
};

struct InvalidOperaor : Error {
	explicit InvalidOperaor(Token token);
};
} // namespace kalcy
