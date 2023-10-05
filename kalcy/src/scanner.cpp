#include <kalcy/scanner.hpp>
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

[[nodiscard]] auto is_alpha(char const ch) -> bool { return ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); }

[[nodiscard]] auto is_alphanumeric(char const ch) -> bool { return std::isdigit(static_cast<unsigned char>(ch)) != 0 || is_alpha(ch); }
} // namespace

auto Scanner::next() -> Token {
	while (!at_end() && is_space(current())) { advance(); }
	if (at_end()) { return {}; }

	auto out = Token{};
	if (match_number(out)) { return out; }
	if (match_single(out)) { return out; }
	if (match_identifier(out)) { return out; }

	return {};
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
	if (ec == std::errc{}) {
		out = make_token(Token::Type::eNumber, {m_text.data(), ptr});
		out.value = value;
		return true;
	}
	return false;
}

auto Scanner::match_identifier(Token& out) -> bool {
	assert(!at_end());
	if (!is_alpha(m_text.front())) { return false; }
	auto end = std::size_t{};
	while (end < m_text.size()) {
		if (!is_alphanumeric(m_text.at(end))) { break; }
		++end;
	}
	out = make_token(Token::Type::eIdentifier, m_text.substr(0, end));
	return true;
}

auto Scanner::make_token(Token::Type const type, std::string_view const text) -> Token {
	auto ret = Token{.type = type, .lexeme = text, .location = m_location};
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
