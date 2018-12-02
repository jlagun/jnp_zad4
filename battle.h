#pragma once

#include <array>
#include <algorithm>
#include <tuple>
#include "imperialfleet.h"
#include "rebelfleet.h"

template<typename T, T t0, T t1, typename... S>
class SpaceBattle {
	static_assert(t0 <= t1 && t0 >= 0, "invalid start or end time");
	static_assert(((is_rebelship<S>::value || is_imperialship<S>::value) && ...),
	              "S should contain only spaceship types");

	template<T i = 0, T... vals>
	static constexpr auto calcSquares() {
		if constexpr (i * i > t1)
			return std::array<T, sizeof...(vals)> {{vals...}};
		else
			return calcSquares<i + 1, vals..., i * i>();
	}
	
	static constexpr auto attackMoments = calcSquares();
	static constexpr size_t shipsNum = sizeof...(S);
	mutable size_t rebelAlive, imperialAlive;
	T curTime;
	std::tuple<S&...> ships;
	
	template<typename S1, typename S2>
	void battleHelper(S1 &a, S2 &b) {
		if constexpr (is_imperialship<S1>::value && is_rebelship<S2>::value)
			if (a.getShield() > 0 && b.getShield() > 0)
				attack(a, b);
	}
	
	// iteruje sie po kazdej parze statkow i przeprowadza bitwe miedzy nimi
	template<size_t id1 = 0, size_t id2 = 0>
	void battle() {
		if constexpr (id1 < shipsNum)
			battleHelper(std::get<id1>(ships), std::get<id2>(ships));
		if constexpr (id1 + 1 < shipsNum)
			battle<id1 + 1, id2>();
		else if constexpr (id2 + 1 < shipsNum)
			battle<0, id2 + 1>();
	}
	
	template<typename SH>
	void countHelper(const SH& ship, size_t &rebelCnt, size_t &imperialCnt) const {
		if constexpr (is_rebelship<SH>::value)
			rebelCnt += ship.getShield() > 0;
		else if constexpr (is_imperialship<SH>::value)
			imperialCnt += ship.getShield() > 0;
	}
	
	// liczy liczbe statkow rebelii i imperium
	template<size_t id = 0>
	std::pair<size_t, size_t> count(size_t rebelCnt = 0, size_t imperialCnt = 0) const {
		if constexpr (id < shipsNum)
			countHelper(std::get<id>(ships), rebelCnt, imperialCnt);
		if constexpr (id + 1 < shipsNum)
			return count<id + 1>(rebelCnt, imperialCnt);
		return {rebelCnt, imperialCnt};
	}
	
	void updateCounts() const {
		std::tie(rebelAlive, imperialAlive) = count();
	}
	
public:
	SpaceBattle(S&... spaceships)
		: curTime(t0), ships(spaceships...) {
		updateCounts();
	}
	
	size_t countRebelFleet() const {
		updateCounts();
		return rebelAlive;
	}
	
	size_t countImperialFleet() const {
		updateCounts();
		return imperialAlive;
	}
	
	void tick(T timeStep) {
		updateCounts();
		if (imperialAlive == 0) {
			puts(rebelAlive == 0 ? "DRAW" : "REBELLION WON");
			return;
		}
		else if (rebelAlive == 0) {
			puts("IMPERIUM WON");
			return;
		}
		
		if (std::binary_search(std::begin(attackMoments), std::end(attackMoments), curTime))
			battle();
		
		curTime = (curTime + timeStep) % (t1 + 1);
	}
};
