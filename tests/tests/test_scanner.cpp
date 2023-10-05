#include <impl/scanner.hpp>
#include <test/test.hpp>

namespace {
using namespace kalcy;

auto next_token(std::string_view const text) -> Token {
	auto s = Scanner{text};
	auto ret = Token{};
	if (!s.next(ret)) { return Token{}; }
	return ret;
}

auto match_token(std::string_view const text, Token::Type const type) -> bool {
	auto const token = next_token(text);
	return token.type == type && token.text == text;
}

auto match_number(std::string_view const text, double const value) -> bool {
	auto const token = next_token(text);
	return token.type == Token::Type::eNumber && token.text == text && token.value == value;
}

ADD_TEST(ScanSingles) {
	EXPECT(match_token("+", Token::Type::ePlus));
	EXPECT(match_token("-", Token::Type::eMinus));
	EXPECT(match_token("*", Token::Type::eStar));
	EXPECT(match_token("/", Token::Type::eSlash));
	EXPECT(match_token("^", Token::Type::eCaret));
	EXPECT(match_token(",", Token::Type::eComma));
	EXPECT(match_token("(", Token::Type::eParenLeft));
	EXPECT(match_token(")", Token::Type::eParenRight));
}

ADD_TEST(ScanIdentifiers) {
	EXPECT(match_token("foo", Token::Type::eIdentifier));
	EXPECT(match_token("sqrt", Token::Type::eIdentifier));
}

ADD_TEST(ScanNumbers) {
	EXPECT(match_number("3", 3));
	EXPECT(match_number("-42", -42));
	EXPECT(match_number("-4.2", -4.2)); // NOLINT
}
} // namespace
