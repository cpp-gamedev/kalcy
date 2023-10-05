#include <kalcy/env.hpp>
#include <kalcy/error.hpp>
#include <cassert>

namespace kalcy {
void Env::define(std::string_view name, Func func) {
	if (name.empty() || !func) { return; }
	m_table.insert_or_assign(name, std::move(func));
}

void Env::define(std::string_view name, double const constant) {
	if (name.empty()) { return; }
	m_table.insert_or_assign(name, constant);
}

auto Env::invoke(Token const name, std::span<double const> args) const noexcept(false) -> double {
	auto const it = m_table.find(name.lexeme);
	if (it == m_table.end() || !std::holds_alternative<Func>(it->second)) { throw UndefinedSymbol{name}; }

	return std::get<Func>(it->second)(args);
}

auto Env::deref(Token const name) const noexcept(false) -> double {
	auto const it = m_table.find(name.lexeme);
	if (it == m_table.end() || !std::holds_alternative<double>(it->second)) { throw UndefinedSymbol{name}; }

	return std::get<double>(it->second);
}
} // namespace kalcy
