#include <kalcy/error.hpp>
#include <kalcy/eval.hpp>
#include <kalcy/kalcy.hpp>
#include <kalcy/parser.hpp>
#include <cmath>

namespace kalcy {
Kalcy::Kalcy() {
	environment.define("pi", std::numbers::pi_v<double>);
	environment.define("sqrt", [](std::span<double const> args) {
		if (args.size() != 1) { throw Env::Mismatch{}; }
		return std::sqrt(args.front());
	});
}

auto Kalcy::evaluate(std::string_view const expression) const noexcept(false) -> double {
	if (expression.empty()) { return {}; }
	auto expr = Parser{expression}.parse();
	if (!expr) { return {}; }
	return Eval{environment}(*expr);
}
} // namespace kalcy
