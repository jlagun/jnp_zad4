#pragma once

#include <array>
#include <any>
#include <tuple>
#include <iostream>
#include "imperialfleet.h"
#include "rebelfleet.h"

using std::cerr;
using std::endl;

template<typename T, T i, T mx, T... vals>
constexpr auto calcSquares() {
	if constexpr (i * i > mx)
		return std::array<T, sizeof...(vals)> {{vals...}};
	else
		return calcSquares<T, i + 1, mx, vals..., i * i>();
}

template<typename T, T t0, T t1, typename... S>
class SpaceBattle {
	static constexpr auto attack_moments = calcSquares<T, 0, t1>();
	static constexpr size_t shipsNum = sizeof...(S);
	size_t imperialAlive, rebelAlive;
	T curTime;
	std::tuple<S...> ships;
	
	template<typename SH, typename... Args>
	constexpr std::pair<size_t, size_t> count(size_t rebelCnt, size_t imperialCnt, SH ship, Args... args) {
		if constexpr (is_rebelship<SH>()) {
			if (ship.getShield() > 0)
				imperialCnt++;
		}
		if constexpr (is_imperialship<SH>()) {
			if (ship.getShield() > 0)
				rebelCnt++;
		}
		
		if constexpr (sizeof...(args) > 0)
			return count(rebelCnt, imperialCnt, args...);
		else
			return {rebelCnt, imperialCnt};
	}
	
	template<typename S1, typename S2>
	void odpalaj(S1 &a, S2 &b) {
		if constexpr (is_imperialship<S1>() && is_rebelship<S2>()) {
			if (a.getShield() > 0 && b.getShield() > 0) {
				attack<S1, S2>(a, b);
				if (a.getShield() <= 0)
					imperialAlive--;
				if (b.getShield() <= 0)
					rebelAlive--;
			}
		}
	}
	
	template<size_t id1, size_t id2>
	void iter2() {
		odpalaj(std::get<id1>(ships), std::get<id2>(ships));
		if constexpr (id2 + 1 < shipsNum)
			iter2<id1, id2 + 1>();
	}
	
	template<size_t id1>
	void iter1() {
		iter2<id1, 0>();
		if constexpr (id1 + 1 < shipsNum)
			iter1<id1 + 1>();
	}
public:
	SpaceBattle(S... spaceships)
		: curTime(t0), ships(spaceships...) {
		std::tie(imperialAlive, rebelAlive) = count(0, 0, spaceships...);
	}
	
	size_t countImperialFleet() const {
		return imperialAlive;
	}
	
	size_t countRebelFleet() const {
		return rebelAlive;
	}
	
	void tick(T timeStep) {
		if (countImperialFleet() == 0) {
			if (countRebelFleet() == 0)
				puts("DRAW\n");
			else
				puts("REBELLION WON\n");
			return;
		}
		else if (countRebelFleet() == 0) {
			puts("IMPERIUM WON\n");
			return;
		}
			
		if (std::find(std::begin(attack_moments), std::end(attack_moments), curTime) != std::end(attack_moments))
			iter1<0>();
		
		curTime = (curTime + timeStep) % t1;
	}
};

	// std::tuple<> rebels;
	// std::tuple<> imperials;
	
	// template<typename SH, typename... Args>
	// void divide(SH ship, Args... args) {
	// 	if constexpr (is_rebelship<SH>()) {
	// 		if (ship.getShield() > 0)
	// 			// rebels.emplace_back(&ship);
	// 			puts("rebel");
	// 	}
	// 	if constexpr (is_imperialship<SH>()) {
	// 		if (ship.getShield() > 0)
	// 		// 	// imperials.emplace_back(&ship);
	// 			puts("imperial");
	// 	}
		
	// 	if constexpr (sizeof...(args) > 0)
	// 		divide(args...);
	// 	// else {
	// 	// 	std::cerr << imperials.size() << std::endl;
	// 	// 	std::cerr << rebels.size() << std::endl;
	// 	// 	std::cerr << rebels[0]->getShield() << std::endl;
	// 	// }
	// }
	
