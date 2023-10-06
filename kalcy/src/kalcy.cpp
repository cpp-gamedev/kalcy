#include <kalcy/error.hpp>
#include <kalcy/eval.hpp>
#include <kalcy/kalcy.hpp>
#include <kalcy/parser.hpp>
#include <cmath>
#include <numbers>

namespace kalcy {
Kalcy::Kalcy() {
	environment.define("pi", std::numbers::pi_v<double>);
	environment.define("sqrt", [](std::span<double const> args) {
		if (args.size() != 1) { throw Env::Mismatch{}; }
		return std::sqrt(args.front());
	});
}

auto Kalcy::evaluate(std::string_view const expression, std::string* out_ast) const noexcept(false) -> double {
	if (expression.empty()) { return {}; }
	auto expr = Parser{expression}.parse();
	if (!expr) { return {}; }
	if (out_ast != nullptr) { *out_ast = to_string(*expr); }
	return Eval{environment}(*expr);
}
} // namespace kalcy
