#pragma once

namespace kalcy {
///
/// \brief Overload wrapper for multiple lambdas.
///
template <typename... Types>
struct Visitor : Types... {
	using Types::operator()...;
};

template <typename... Types>
Visitor(Types...) -> Visitor<Types...>;
} // namespace kalcy
