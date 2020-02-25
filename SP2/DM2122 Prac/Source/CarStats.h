#pragma once
#include <TRS.h>

struct CarStats
{
	float StatLevel[6]; //save for logic later
	TRS StatTRS[6]; //save for logic later
	bool lock;
	int cost, cost_upgrade, current_upgrade;

	CarStats();
	CarStats(float speed, float turbo, float fuel, float speedupgrade, float turboupgrade, float fuelupgrade);
};
