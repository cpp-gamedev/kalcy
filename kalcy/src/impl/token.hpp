#pragma once
#include <impl/enum_array.hpp>
#include <string_view>

namespace kalcy {
///
/// \brief Scanned token.
///
struct Token {
	enum class Type : int {
		eNone,

		// single chars
		e_SingleCharFirst,
		ePlus = e_SingleCharFirst,
		eMinus,
		eStar,
		eSlash,
		eCaret,
		eComma,
		eParenLeft,
		eParenRight,
		e_SingleCharLast = eParenRight,

		// strings
		eIdentifier,
		eNumber,

		eCOUNT_
	};

	static constexpr auto type_str_v = EnumArray<Type, std::string_view>{
		"none", "+", "-", "*", "/", "^", ",", "(", ")", "number", "identifier",
	};

	Type type{};
	std::string_view text{};
	double value{};
	std::size_t location{};
};
} // namespace kalcy
