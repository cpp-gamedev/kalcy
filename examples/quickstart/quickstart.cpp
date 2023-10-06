#include <kalcy/kalcy.hpp>
#include <cassert>
#include <format>
#include <iomanip>
#include <iostream>
#include <span>

namespace {
auto run(std::string_view const text, bool verbose) -> bool {
	try {
		// parse text into an expression
		auto expr = kalcy::Parser{text}.parse();
		assert(expr != nullptr);
		// evaluate parsed expression
		// a custom Env can be used and passed, if desired
		std::cout << kalcy::evaluate(*expr) << "\n";
		// print AST if verbose
		if (verbose) { std::cout << std::format("expression\t: {}\nAST\t\t: {}\n", text, kalcy::to_string(*expr)); }
		return true;
	} catch (kalcy::Error const& err) {
		// print error
		std::cerr << err.what() << "\n";
		// highlight error location under text
		std::cerr << " | " << text << "\n | " << err.build_highlight() << "\n";
		return false;
	}
}
} // namespace

auto main(int argc, char** argv) -> int {
	// obtain arguments, skip exe name
	auto args = std::span{argv, static_cast<std::size_t>(argc)}.subspan(1);

	bool verbose{};
	// check if verbose
	if (args.front() == std::string_view{"-v"}) {
		verbose = true;
		// advance args by 1
		args = args.subspan(1);
	}

	if (args.empty()) {
		// print usage
		std::cout << std::format("usage: {} [-v] \"<expression>\"\n", *argv);
		return EXIT_SUCCESS;
	}

	// run kalcy on input expression
	if (!run(args.front(), verbose)) { return EXIT_FAILURE; }

	// print epilogue
	std::cout << std::format("\n^^ kalcy v{}\n", kalcy::version_v);
}
