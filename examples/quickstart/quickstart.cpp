#include <kalcy/kalcy.hpp>
#include <format>
#include <iomanip>
#include <iostream>
#include <span>

namespace {
auto run(std::string_view const text, bool verbose) -> bool {
	auto kalcy = kalcy::Kalcy{};
	auto ast = std::string{};
	try {
		std::cout << kalcy.evaluate(text, &ast) << "\n";
		if (verbose) { std::cout << "expression\t: " << text << "\nAST\t\t: " << ast << "\n"; }
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
