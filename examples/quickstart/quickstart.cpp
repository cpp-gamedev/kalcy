#include <kalcy/kalcy.hpp>
#include <cassert>
#include <format>
#include <iomanip>
#include <iostream>
#include <span>

namespace {
auto run(std::string_view const text, bool verbose) -> bool {
	try {
		auto expr = kalcy::Parser{text}.parse();
		assert(expr != nullptr);
		std::cout << kalcy::evaluate(*expr) << "\n";
		if (verbose) { std::cout << std::format("expression\t: {}\nAST\t\t: {}\n", text, kalcy::to_string(*expr)); }
		return true;
	} catch (kalcy::Error const& err) {
		std::cerr << err.what() << "\n";
		std::cerr << " | " << text << "\n | " << err.build_highlight() << "\n";
		return false;
	}
}
} // namespace

auto main(int argc, char** argv) -> int {
	auto args = std::span{argv, static_cast<std::size_t>(argc)}.subspan(1);

	bool verbose{};
	if (args.front() == std::string_view{"-v"}) {
		args = args.subspan(1);
		verbose = true;
	}

	if (args.empty()) {
		std::cout << std::format("usage: {} [-v] \"<expression>\"\n", *argv);
		return EXIT_SUCCESS;
	}

	if (!run(args.front(), verbose)) { return EXIT_FAILURE; }

	std::cout << std::format("\n^^ kalcy v{}\n", kalcy::version_v);
}
