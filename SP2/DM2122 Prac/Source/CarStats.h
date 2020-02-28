#pragma once
#include <TRS.h>
#include "HologramUI.h"
struct CarStats
{
	float StatLevel[6]; //save for logic later
	TRS StatTRS[6]; //save for logic later
	TRS StatUpgrade;
	bool lock;
	int cost, cost_upgrade, current_upgrade;
	void UpgradeOnce();
	void UpgradeFromStart();
	void InitStatsinShop(HologramUI ObjectDisplay);
	void InitStatsinHolo(HologramUI ObjectDisplay);
	void BuyCar();
	void ResettoBase();
	void RechargeCarFuel();
	bool isRecharged();
	CarStats();

	CarStats(float acceleration, float speed, float turbo, float fuel);
	CarStats(float acceleration, float speed, float turbo, float fuel, float fuelcurrent);

};
