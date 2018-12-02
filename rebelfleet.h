#pragma once

#include <type_traits>
#include <cassert>

template<bool canAttack, typename U, int minSpeed, int maxSpeed>
class RebelStarship {
	U shield, speed, attackPower;
public:
	typedef U value_type;
	
	// konstruktor dla statkow, ktore moga atakowac
	template<typename Dummy = void, typename = std::enable_if_t<canAttack == true, Dummy>>
	RebelStarship(U _shield, U _speed, U _attackPower)
		: shield(_shield), speed(_speed), attackPower(_attackPower) {
		assert(minSpeed <= speed && speed <= maxSpeed);
	}
	
	// konstruktor dla statkow, ktore nie moga atakowac
	template<typename Dummy = void, typename = std::enable_if_t<canAttack == false, Dummy>>
	RebelStarship(U _shield, U _speed)
		: shield(_shield), speed(_speed) {
		assert(minSpeed <= speed && speed <= maxSpeed);
	}
	
	U getShield() const {
		return shield;
	}
	
	U getSpeed() const {
		return speed;
	}
	
	template<typename Dummy = void, typename = std::enable_if_t<canAttack == true, Dummy>>
	U getAttackPower() const {
		return attackPower;
	}
	
	void takeDamage(U damage) {
		if (damage < shield)
			shield -= damage;
		else
			shield = 0;
	}
};

template<typename U> using Explorer = RebelStarship<false, U, 299796, 2997960>;
template<typename U> using XWing = RebelStarship<true, U, 299796, 2997960>;
template<typename U> using StarCruiser = RebelStarship<true, U, 99999, 299795>;


template<typename>
struct is_rebelship_with_attack : std::false_type {};

template<typename U, int minSpeed, int maxSpeed>
struct is_rebelship_with_attack<RebelStarship<true, U, minSpeed, maxSpeed>> : std::true_type {};

template<typename>
struct is_rebelship : std::false_type {};

template<bool canAttack, typename U, int minSpeed, int maxSpeed>
struct is_rebelship<RebelStarship<canAttack, U, minSpeed, maxSpeed>> : std::true_type {};
