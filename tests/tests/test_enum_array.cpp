#include <kalcy/enum_array.hpp>

namespace {
using namespace kalcy;

enum class Foo { eZero, eOne, eTwo, eCOUNT_ };

constexpr auto foo_empty_v = EnumArray<Foo, int>{};
constexpr auto foo_full_v = EnumArray<Foo, int>{0, 1, 2};

static_assert(foo_full_v[Foo::eZero] == 0);
static_assert(foo_full_v[Foo::eOne] == 1);
static_assert(foo_full_v[Foo::eTwo] == 2);
} // namespace
