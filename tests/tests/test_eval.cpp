#include <kalcy/error.hpp>
#include <kalcy/eval.hpp>
#include <kalcy/parser.hpp>
#include <test/test.hpp>
#include <cmath>

namespace {
using namespace kalcy;

auto make_expr(std::string_view text) -> UExpr { return Parser{}.parse(text); }

ADD_TEST(Evaluate) {
	auto const env = Env{};
	auto expr = make_expr("2 + 3");
	ASSERT(expr != nullptr);
	EXPECT(Eval{env}(*expr) == 5.0); // NOLINT

	expr = make_expr("1 + 2 * 3 ^ 2");
	ASSERT(expr != nullptr);
	EXPECT(Eval{env}(*expr) == 19.0); // NOLINT
}

ADD_TEST(Call) {
	auto env = Env{};
	env.define("sqrt", [](std::span<double const> args) {
		if (args.size() != 1) { throw Env::Mismatch{}; }
		return std::sqrt(args.front());
	});
	auto expr = make_expr("sqrt(4)");
	ASSERT(expr != nullptr);
	EXPECT(Eval{env}(*expr) == 2.0); // NOLINT

	env.define("pi", 3.14); // NOLINT
	expr = make_expr("pi");
	EXPECT(Eval{env}(*expr) == 3.14); // NOLINT

	bool did_throw{};
	expr = make_expr("sqrt(4, 5)");
	try {
		[[maybe_unused]] auto discard = Eval{env}(*expr);
	} catch (ArgsMismatch const& e) {
		EXPECT(e.argument_count == 2);
		did_throw = true;
	}
	EXPECT(did_throw);

	did_throw = false;
	expr = make_expr("fubar()");
	try {
		[[maybe_unused]] auto discard = Eval{env}(*expr);
	} catch (UndefinedSymbol const& e) {
		EXPECT(e.token.lexeme == "fubar");
		did_throw = true;
	}
	EXPECT(did_throw);
}
} // namespace
