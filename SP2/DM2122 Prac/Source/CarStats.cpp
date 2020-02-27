#include "CarStats.h"
void CarStats::UpgradeOnce()
{
	for (int i = 0; i < 3; ++i)
	{
		StatLevel[i] += StatLevel[i + 3];
	}
}

void CarStats::UpgradeFromStart()
{
	if (current_upgrade != 0)
	{
		for (int i = 0; i < current_upgrade; ++i)
		{
			UpgradeOnce();
		}
	}
	else return;
}

CarStats::CarStats() //set default to 1
{
	for (int i = 0; i < 6; ++i)
	{
		StatLevel[i] = 1.f;
		StatTRS[i].Scale = Vector3(0.f, 1.f, 1.f);
	}
	cost = 500;
	cost_upgrade = 250;
	current_upgrade = 0;
	lock = true;
}

CarStats::CarStats(float speed, float turbo, float fuel, float speedupgrade, float turboupgrade, float fuelupgrade)
{
	StatLevel[0] = speed; StatLevel[1] = turbo; StatLevel[2] = fuel;
	StatLevel[3] = speedupgrade; StatLevel[4] = turboupgrade; StatLevel[5] = fuelupgrade;

	for (int i = 0; i < 6; ++i) {
		StatTRS[i].Scale = Vector3(0.f, 1.f, 1.f);
	}
	cost = 500;
	cost_upgrade = 250;
	current_upgrade = 0;
	lock = true;
}