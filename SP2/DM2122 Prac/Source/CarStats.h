#pragma once
#include <TRS.h>

struct CarStats
{
	float StatLevel[6]; //save for logic later
	TRS StatTRS[6]; //save for logic later
	bool lock;
	int cost, cost_upgrade, current_upgrade;
	void UpgradeOnce();
	void UpgradeFromStart();
	CarStats();
	CarStats(float acceleration, float turbo, float fuel, float accelerationupgrade, float turboupgrade, float fuelupgrade);
};
