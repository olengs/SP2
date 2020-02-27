#include "CarStats.h"
void CarStats::UpgradeOnce()
{
	for (int i = 0; i < 4; ++i)
	{
		++StatLevel[i];
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
	for (int i = 0; i < 5; ++i)
	{
		StatLevel[i] = 1.f;
		StatTRS[i].Scale = Vector3(0.f, 0.25f, 1.f);
	}
	StatUpgrade.Scale = Vector3(0.f, 1.f, 1.f);
	cost = 500;
	cost_upgrade = 250;
	current_upgrade = 0;
	lock = true;
}

CarStats::CarStats(float acceleration, float speed, float turbo, float fuel)
{
	//StatLevel[0]: acceleration, StatLevel[1]: speed, StatLevel[2]: turbo, StatLevel[3]: fuel max
	//StatLevel[4]: fuel current
	StatLevel[0] = acceleration; StatLevel[1] = speed; StatLevel[2] = turbo;
	StatLevel[3] = fuel; 
	StatLevel[4] = StatLevel[3];
	StatUpgrade.Scale = Vector3(0.f, 1.f, 1.f);
	for (int i = 0; i < 6; ++i) {
		StatTRS[i].Scale = Vector3(0.f, 1.f, 1.f);
	}
	cost = 500;
	cost_upgrade = 250;
	current_upgrade = 0;
	lock = true;
}