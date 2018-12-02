#pragma once

#include <type_traits>
#include "rebelfleet.h"

template<typename U>
class ImperialStarship {};

template<typename>
struct is_imperialship : std::false_type {};

template<typename U>
struct is_imperialship<ImperialStarship<U>> : std::true_type {};

template<typename I, typename R>
void attack(I &imperialShip, R &rebelShip) {
	static_assert(is_imperialship<I>::value && is_rebelship<R>::value);
	if constexpr (is_rebelship_with_attack<R>::value);
}