#pragma once
#include "CarStats.h"
#include <string>
#include <fstream>
class AllCarDetails
{
private:
	CarStats car_Stats[4];
public:
	AllCarDetails();
	CarStats getCarStats(int number);
	void SaveData(CarStats storage1, CarStats storage2, CarStats storage3, CarStats storage4);
	void UpdateFile();
	void InitScene(CarStats& carstat0, CarStats& carstat1, CarStats& carstat2, CarStats& carstat3);
	bool IsInit();
	void GetData();
};

