#include "CarStats.h"
void CarStats::UpgradeOnce()
{
	for (int i = 0; i < 5; ++i)
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

void CarStats::InitStatsinShop(HologramUI ObjectDisplay)
{
	StatTRS[0].translate = Vector3(-((3.f / 14.f) * ObjectDisplay.lengthX) + ((StatLevel[0] / 28.f) * ObjectDisplay.lengthX), -(1.f / 14.f) * ObjectDisplay.lengthY, 0.1f);
	StatTRS[1].translate = Vector3(-((3.f / 14.f) * ObjectDisplay.lengthX) + ((StatLevel[1] / 28.f) * ObjectDisplay.lengthX), -(2.f / 14.f) * ObjectDisplay.lengthY, 0.1f);
	StatTRS[2].translate = Vector3(-((3.f / 14.f) * ObjectDisplay.lengthX) + ((StatLevel[2] / 28.f) * ObjectDisplay.lengthX), -(3.f / 14.f) * ObjectDisplay.lengthY, 0.1f);
	StatTRS[3].translate = Vector3(-((3.f / 14.f) * ObjectDisplay.lengthX) + ((StatLevel[3] / 28.f) * ObjectDisplay.lengthX), -(4.f / 14.f) * ObjectDisplay.lengthY, 0.1f);
	StatTRS[4].translate = Vector3(-((3.f / 14.f) * ObjectDisplay.lengthX) + ((StatLevel[4] / 28.f) * ObjectDisplay.lengthX), -(5.f / 14.f) * ObjectDisplay.lengthY, 0.1f);
}

void CarStats::InitStatsinHolo(HologramUI ObjectDisplay)
{
	StatTRS[0].translate = Vector3(-((3.f / 14.f) * ObjectDisplay.lengthX) + (StatLevel[0] / 28.f) * ObjectDisplay.lengthX, (6.f / 14.f) * ObjectDisplay.lengthY, 0.1f);
	StatTRS[1].translate = Vector3(-((3.f / 14.f) * ObjectDisplay.lengthX) + (StatLevel[1] / 28.f) * ObjectDisplay.lengthX, (3.5f / 14.f) * ObjectDisplay.lengthY, 0.1f);
	StatTRS[2].translate = Vector3(-((3.f / 14.f) * ObjectDisplay.lengthX) + (StatLevel[2] / 28.f) * ObjectDisplay.lengthX, (1.f / 14.f) * ObjectDisplay.lengthY, 0.1f);
	StatTRS[3].translate = Vector3(-((3.f / 14.f) * ObjectDisplay.lengthX) + (StatLevel[3] / 28.f) * ObjectDisplay.lengthX, -(1.5f / 14.f) * ObjectDisplay.lengthY, 0.1f);
	StatTRS[4].translate = Vector3(-((3.f / 14.f) * ObjectDisplay.lengthX) + (StatLevel[4] / 28.f) * ObjectDisplay.lengthX, -(4.f / 14.f) * ObjectDisplay.lengthY, 0.1f);

}

void CarStats::BuyCar()
{
	lock = false;
}

void CarStats::ResettoBase()
{
	if (current_upgrade == 0) return;
	else
	{
		for (int i = current_upgrade;i > 0;--i)
		{
			for (int j = 0;j < 5;++j)
			{
				--StatLevel[j];
			}
		}

	}
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


CarStats::CarStats::CarStats(float acceleration, float speed, float turbo, float fuel)
{
	//StatLevel[0]: acceleration, StatLevel[1]: speed, StatLevel[2]: turbo, StatLevel[3]: fuel max
	//StatLevel[4]: fuel current
	StatLevel[0] = acceleration; StatLevel[1] = speed; StatLevel[2] = turbo;
	StatLevel[3] = fuel;
	StatLevel[4] = StatLevel[3];
	StatUpgrade.Scale = Vector3(0.f, 1.f, 1.f);

	for (int i = 0; i < 5; ++i) {
		StatTRS[i].Scale = Vector3(0.f, 1.f, 1.f);
	}
	cost = 500;
	cost_upgrade = 250;
	current_upgrade = 0;
	lock = true;
}