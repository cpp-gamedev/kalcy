#pragma once
#include <array>

namespace kalcy {
///
/// \brief Concept for enums.
///
template <typename Type>
concept EnumT = std::is_enum_v<Type>;

///
/// \brief Array of Type, indexed by E.
///
template <EnumT E, typename Type, std::size_t Count = std::size_t(E::eCOUNT_)>
class EnumArray {
  public:
	std::array<Type, Count> values{};

	EnumArray() = default;

	///
	/// \brief Constructor must be passed all the values to initialize.
	/// \param values arguments in order of storage.
	///
	template <std::convertible_to<Type>... T>
		requires(sizeof...(T) == Count)
	constexpr EnumArray(T... values) : values{std::move(values)...} {}

	///
	/// \brief Obtain the value at index e.
	/// \returns value at index e.
	/// \param e enum index.
	///
	[[nodiscard]] constexpr auto at(E const e) const -> Type const& { return values.at(static_cast<std::size_t>(e)); }

	constexpr auto operator[](E const e) const -> Type const& { return at(e); }
};
} // namespace kalcy
