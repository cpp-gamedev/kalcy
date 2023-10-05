#include <kalcy/error.hpp>
#include <format>

namespace kalcy {
ParseError::ParseError(Token const token) : Error(token, "parse error") {}

ParseError::ParseError(Token const token, Token::Type const expected)
	: Error(token, std::format("parse error: expected '{}'", Token::type_str_v[expected])), expected(expected) {}

UndefinedSymbol::UndefinedSymbol(Token const token) : Error(token, std::format("undefined symbol: '{}'", token.lexeme)) {}

ArgsMismatch::ArgsMismatch(Token const token, std::size_t argument_count)
	: Error(token, std::format("{} does not take {} arguments", token.lexeme, argument_count)), argument_count(argument_count) {}

InvalidOperaor::InvalidOperaor(Token const token) : Error(token, std::format("invalid operator: '{}'", token.lexeme)) {}
} // namespace kalcy
