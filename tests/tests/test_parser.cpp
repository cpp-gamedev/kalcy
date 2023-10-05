#include <kalcy/error.hpp>
#include <kalcy/parser.hpp>
#include <kalcy/scanner.hpp>
#include <test/test.hpp>
#include <optional>

namespace {
using namespace kalcy;
auto parse(std::string_view const text) -> UExpr { return Parser{text}.parse(); }

ADD_TEST(ParseEmptyString) { EXPECT(parse("") == nullptr); }

ADD_TEST(ParsePrimary) {
	auto const result = parse("42");
	ASSERT(result != nullptr);
	ASSERT(std::holds_alternative<expr::Literal>(*result));
	auto const& literal = std::get<expr::Literal>(*result);
	EXPECT(literal.token.type == Token::Type::eNumber);
	EXPECT(literal.token.value == 42.0); // NOLINT
}

ADD_TEST(ParseUnary) {
	auto const result = parse("- 3");
	ASSERT(result != nullptr);
	ASSERT(std::holds_alternative<expr::Unary>(*result));
	auto const& unary = std::get<expr::Unary>(*result);
	EXPECT(unary.op.type == Token::Type::eMinus);
	EXPECT(unary.op.lexeme == "-");
	ASSERT(unary.right != nullptr);
	ASSERT(std::holds_alternative<expr::Literal>(*unary.right));
	auto const& literal = std::get<expr::Literal>(*unary.right);
	EXPECT(literal.token.type == Token::Type::eNumber);
	EXPECT(literal.token.value == 3.0); // NOLINT
}

ADD_TEST(ParseBinary) {
	auto result = parse("3+2");
	ASSERT(result != nullptr);
	ASSERT(std::holds_alternative<expr::Binary>(*result));
	auto const& binary = std::get<expr::Binary>(*result);

	ASSERT(binary.left != nullptr);
	ASSERT(std::holds_alternative<expr::Literal>(*binary.left));
	auto literal = std::get<expr::Literal>(*binary.left);
	EXPECT(literal.token.type == Token::Type::eNumber);
	EXPECT(literal.token.value == 3.0); // NOLINT

	EXPECT(binary.op.type == Token::Type::ePlus);

	ASSERT(binary.right != nullptr);
	ASSERT(std::holds_alternative<expr::Literal>(*binary.right));
	literal = std::get<expr::Literal>(*binary.right);
	EXPECT(literal.token.type == Token::Type::eNumber);
	EXPECT(literal.token.value == 2.0); // NOLINT
}

ADD_TEST(ParseCall) {
	auto result = parse("sqrt(123.45)");
	ASSERT(result != nullptr);
	ASSERT(std::holds_alternative<expr::Call>(*result));
	auto const& sqrt_call = std::get<expr::Call>(*result);
	EXPECT(sqrt_call.callee.type == Token::Type::eIdentifier);
	EXPECT(sqrt_call.callee.lexeme == "sqrt");
	ASSERT(sqrt_call.arguments.size() == 1);
	ASSERT(sqrt_call.arguments.front() != nullptr);
	ASSERT(std::holds_alternative<expr::Literal>(*sqrt_call.arguments.front()));
	EXPECT(std::get<expr::Literal>(*sqrt_call.arguments.front()).token.value == 123.45); // NOLINT

	result = parse("pow(42, -5)");
	ASSERT(result != nullptr);
	ASSERT(std::holds_alternative<expr::Call>(*result));
	auto const& pow_call = std::get<expr::Call>(*result);
	EXPECT(pow_call.callee.type == Token::Type::eIdentifier);
	EXPECT(pow_call.callee.lexeme == "pow");
	ASSERT(pow_call.arguments.size() == 2);
	ASSERT(pow_call.arguments[0] != nullptr);
	ASSERT(std::holds_alternative<expr::Literal>(*pow_call.arguments[0]));
	EXPECT(std::get<expr::Literal>(*pow_call.arguments[0]).token.value == 42); // NOLINT
	ASSERT(pow_call.arguments[1] != nullptr);
	ASSERT(std::holds_alternative<expr::Literal>(*pow_call.arguments[1]));
	EXPECT(std::get<expr::Literal>(*pow_call.arguments[1]).token.value == -5); // NOLINT
}

ADD_TEST(ParseErrorOnExtraneous) {
	bool did_throw{};
	try {
		parse("123 45");
	} catch (ParseError const& e) {
		EXPECT(e.expected == Token::Type::eNone);
		EXPECT(e.token.location == std::string_view{"123 "}.size());
		did_throw = true;
	}
	EXPECT(did_throw);
}

ADD_TEST(ParsePrecedence) {
	auto result = parse("1 + 2 * 3");
	ASSERT(result != nullptr);
	ASSERT(std::holds_alternative<expr::Binary>(*result));
	auto const& outer = std::get<expr::Binary>(*result);

	ASSERT(outer.left != nullptr);
	ASSERT(std::holds_alternative<expr::Literal>(*outer.left));
	auto literal = std::get<expr::Literal>(*outer.left);
	EXPECT(literal.token.type == Token::Type::eNumber);
	EXPECT(literal.token.value == 1.0); // NOLINT

	EXPECT(outer.op.type == Token::Type::ePlus);

	ASSERT(outer.right != nullptr);
	ASSERT(std::holds_alternative<expr::Binary>(*outer.right));
	auto const& inner = std::get<expr::Binary>(*outer.right);

	ASSERT(inner.left != nullptr);
	ASSERT(std::holds_alternative<expr::Literal>(*inner.left));
	auto const& lhs = std::get<expr::Literal>(*inner.left);
	EXPECT(lhs.token.type == Token::Type::eNumber);
	EXPECT(lhs.token.value == 2.0); // NOLINT

	EXPECT(inner.op.type == Token::Type::eStar);

	ASSERT(inner.right != nullptr);
	ASSERT(std::holds_alternative<expr::Literal>(*inner.right));
	auto const& rhs = std::get<expr::Literal>(*inner.right);
	EXPECT(rhs.token.type == Token::Type::eNumber);
	EXPECT(rhs.token.value == 3.0); // NOLINT
}
} // namespace
