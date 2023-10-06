#include <kalcy/kalcy.hpp>
#include <cassert>
#include <format>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
struct Repl {
	const std::string_view EXIT_TOKEN{"exit"};
	bool verbose{}; // toggled on with '-v' and off with '-q'
	bool is_running{true};

	auto start() -> bool {
		while (is_running) {
			std::string text{};
			std::cout << std::format("[{}] > ", verbose ? "verbose" : "quiet");
			std::getline(std::cin, text);
			// run kalcy on input expression
			if (!run(text)) { return EXIT_FAILURE; }
		}
		// print epilogue
		std::cout << std::format("\n^^ kalcy v{}\n", kalcy::version_v);
	}

	auto run(std::string_view const text) -> bool {
		try {
			// return false when the user enters the exit token
			if (text == EXIT_TOKEN) {
				is_running = false;
			} else if (text == "-v") {
				verbose = !verbose;
			}  else {
				// parse text into an expression
				auto expr = kalcy::Parser{}.parse(text);
				assert(expr != nullptr);
				// evaluate parsed expression
				// a custom Env can be used and passed, if desired
				std::cout << kalcy::evaluate(*expr) << "\n";
				// print AST if verbose
				if (verbose) { std::cout << std::format("expression\t: {}\nAST\t\t: {}\n", text, kalcy::to_string(*expr)); }
			}
			return true;
		} catch (kalcy::Error const& err) {
			// print error
			std::cerr << err.what() << "\n";
			// highlight error location under text
			std::cerr << " | " << text << "\n | " << err.build_highlight() << "\n";
			return false;
		}
	}
};
} // namespace

auto main(int argc, char** argv) -> int {

	Repl repl{};
	repl.start();
}
