#include <impl/scanner.hpp>
#include <cassert>
#include <cctype>
#include <charconv>

namespace kalcy {
namespace {
[[nodiscard]] auto is_space(char const ch) { return std::isspace(static_cast<unsigned char>(ch)) != 0; }

[[nodiscard]] auto next_whitespace(std::string_view const text) {
	for (std::size_t i = 0; i < text.size(); ++i) {
		if (is_space(text.at(i))) { return i; }
	}
	return std::string_view::npos;
}
} // namespace

auto Scanner::next(Token& out) -> bool {
	while (!at_end() && is_space(current())) { advance(); }
	if (at_end()) { return false; }

	if (match_number(out)) { return true; }
	if (match_single(out)) { return true; }

	out = make_identifier();
	return true;
}

auto Scanner::match_single(Token& out) -> bool {
	auto const ch = current();
	for (Token::Type t = Token::Type::e_SingleCharFirst; t <= Token::Type::e_SingleCharLast; t = Token::Type(int(t) + 1)) {
		if (ch == Token::type_str_v[t].front()) {
			out = make_token(t, m_text.substr(0, 1));
			return true;
		}
	}
	return false;
}

auto Scanner::match_number(Token& out) -> bool {
	assert(!at_end());
	auto const index = next_whitespace(m_text);
	char const* end = index < m_text.size() ? m_text.data() + index : m_text.data() + m_text.size();
	auto value = double{};
	auto const [ptr, ec] = std::from_chars(m_text.data(), end, value);
	if (ec == std::errc{} && ptr == end) {
		out = make_token(Token::Type::eNumber, {m_text.data(), end});
		out.value = value;
		return true;
	}
	return false;
}

auto Scanner::make_identifier() -> Token {
	auto end = std::size_t{};
	for (; end < m_text.size() && !is_space(m_text.at(end)); ++end) {}
	return make_token(Token::Type::eIdentifier, m_text.substr(0, end));
}

auto Scanner::make_token(Token::Type const type, std::string_view const text) -> Token {
	auto ret = Token{.type = type, .text = text, .location = m_location};
	advance(static_cast<int>(text.size()));
	return ret;
}

void Scanner::advance(int count) {
	for (; count > 0; --count) {
		if (at_end()) { return; }
		m_text = m_text.substr(1);
		++m_location;
	}
}
} // namespace kalcy
