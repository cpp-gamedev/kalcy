#include <kalcy/scanner.hpp>
#include <test/test.hpp>

namespace {
using namespace kalcy;

auto next_token(std::string_view const text) -> Token { return Scanner{text}.next(); }

auto match_token(std::string_view const text, Token::Type const type) -> bool {
	auto const token = next_token(text);
	return token.type == type && token.lexeme == text;
}

auto match_number(Token const& token, double const value) -> bool { return token.type == Token::Type::eNumber && token.value == value; }

auto match_number(std::string_view const text, double const value) -> bool {
	auto const token = next_token(text);
	return match_number(token, value) && token.lexeme == text;
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

ADD_TEST(ScanMultiple) {
	auto scanner = Scanner{"3+2-(4+5)"};
	auto token = Token{};

	token = scanner.next();
	ASSERT(static_cast<bool>(token));
	EXPECT(match_number(token, 3.0)); // NOLINT

	token = scanner.next();
	ASSERT(static_cast<bool>(token));
	EXPECT(token.type == Token::Type::ePlus);

	token = scanner.next();
	ASSERT(static_cast<bool>(token));
	EXPECT(match_number(token, 2.0)); // NOLINT

	token = scanner.next();
	ASSERT(static_cast<bool>(token));
	EXPECT(token.type == Token::Type::eMinus);

	token = scanner.next();
	ASSERT(static_cast<bool>(token));
	EXPECT(token.type == Token::Type::eParenLeft);

	token = scanner.next();
	ASSERT(static_cast<bool>(token));
	EXPECT(match_number(token, 4.0)); // NOLINT

	token = scanner.next();
	ASSERT(static_cast<bool>(token));
	EXPECT(token.type == Token::Type::ePlus);

	token = scanner.next();
	ASSERT(static_cast<bool>(token));
	EXPECT(match_number(token, 5.0)); // NOLINT

	token = scanner.next();
	ASSERT(static_cast<bool>(token));
	EXPECT(token.type == Token::Type::eParenRight);

	EXPECT(scanner.at_end());
}
} // namespace
