#pragma once

#include <type_traits>
#include "rebelfleet.h"

template<typename U>
class ImperialStarship {
	static_assert(std::is_arithmetic<U>(), "U should be arithmetic");

	U shield, speed, attackPower;

public:
	typedef U valueType;

	//konstruktor dla wszystkich statków
	ImperialStarship(U _shield, U _attackPower)
		: shield(_shield), attackPower(_attackPower) {};

	U getShield() {
		return shield;
	}

	U getAttackPower() {
		return attackPower;
	}

	void takeDamage(U damage) {
		if(damage <= shield)
			shield -= damage;
		else
			shield = 0;
	}
};

template<typename>
struct is_imperialship : std::false_type {};

template<typename U>
struct is_imperialship<ImperialStarship<U>> : std::true_type {};

template<typename I, typename R>
void attack(I &imperialShip, R &rebelShip) {
	static_assert(is_imperialship<I>::value && is_rebelship<R>::value);
	rebelShip.takeDamage(imperialShip.getAttackPower());
	if constexpr (is_rebelship_with_attack<R>::value)
		imperialShip.takeDamage(rebelShip.getAttackPower());
}