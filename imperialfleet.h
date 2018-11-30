#pragma once

#include <type_traits>
#include "rebelfleet.h"

template<typename U>
class ImperialStarship {};

template<typename>
struct is_imperialship : std::false_type {};

template<typename U>
struct is_imperialship<ImperialStarship<U>> : std::true_type {};

// statek rebelii moze atakowac
template<typename I, typename R>
void attack(std::enable_if_t<is_imperialship<I>::value, I> &imperialShip,
            std::enable_if_t<is_rebelship_with_attack<R>::value == true, R> &rebelShip) {}

// statek rebelii nie moze atakowac
template<typename I, typename R>
void attack(std::enable_if_t<is_imperialship<I>::value, I> &imperialShip,
            std::enable_if_t<is_rebelship_with_attack<R>::value == false, R> &rebelShip) {}