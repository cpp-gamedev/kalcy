#pragma once
#include <impl/token.hpp>

namespace kalcy {
///
/// \brief Token scanner.
///
class Scanner {
  public:
	///
	/// \brief Construct scanner for text.
	/// \param text text to scan.
	///
	explicit Scanner(std::string_view text) : m_text(text) {}

	///
	/// \brief Check if no more tokens remain.
	/// \returns true if remaining text is empty.
	///
	[[nodiscard]] auto at_end() const -> bool { return m_text.empty(); }

	///
	/// \brief Obtain the next token.
	/// \param out Token that's set on success.
	/// \returns true on success.
	///
	auto next(Token& out) -> bool;

  private:
	[[nodiscard]] auto current() const -> char { return at_end() ? '\0' : m_text.front(); }
	[[nodiscard]] auto make_token(Token::Type type, std::string_view text) -> Token;

	auto match_single(Token& out) -> bool;
	auto match_number(Token& out) -> bool;

	auto make_identifier() -> Token;

	void advance(int count = 1);

	std::string_view m_text{};
	std::size_t m_location{};
};
} // namespace kalcy
