#include <kalcy/kalcy.hpp>
#include <cassert>
#include <format>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
struct Repl {
	bool verbose{}; // toggled on with '-v' and off with '-q'
	bool is_running{true};
	kalcy::Parser parser{};

	auto print_help() -> void {
		std::cout << "Usage: [OPTION] | [EXPRESSION]\n";
		std::cout << "\nDescription:\n\n";
		std::cout << std::format("  {:<15} {}\n", "-h, -help", "Display this help message, providing information about available options.");
		std::cout << std::format("  {:<15} {}\n", "-v", "Toggle verbose mode to control the level of detail in the output.");
		std::cout << std::format("  {:<15} {}\n\n", "exit", "Terminate the REPL input loop.");
	}

	auto start() -> bool {
		while (is_running) {
			auto text = std::string{};
			std::cout << std::format("{} > ", verbose ? "[verbose]" : "");
			std::getline(std::cin, text);
			// run kalcy on input expression
			if (!text.empty()) {
				if (!run(text)) { return EXIT_FAILURE; }
			} else {
				print_help();
			}
		}
		// print epilogue
		std::cout << std::format("\n^^ kalcy v{}\n", kalcy::version_v);
		return true;
	}

	auto run(std::string_view const text) -> bool {
		try {
			if (text == "exit") {
				is_running = false;
				return true;
			}

			if (text == "-h" || text == "-help") {
				print_help();
				return true;
			}

			if (text == "-v") {
				verbose = !verbose;
			} else {
				// parse text into an expression
				auto expr = parser.parse(text);
				assert(expr != nullptr);
				// evaluate parsed expression
				// a custom Env can be used and passed, if desired
				std::cout << kalcy::evaluate(*expr) << "\n";
				// print AST if verbose
				if (verbose) { std::cout << std::format("expression\t: {}\nAST\t\t: {}\n", text, kalcy::to_string(*expr)); }
				return true;
			}
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

auto main() -> int {

	Repl repl{};
	try {
		repl.start();
	} catch (std::exception const& e) { std::cerr << e.what() << "\n"; }
}
