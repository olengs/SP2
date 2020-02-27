#pragma once
#include "CarSelection.h"
#include "CarStats.h"
#include <fstream>
#include <iostream>
#include <string>

class PlayerDetails
{

public:
	CarStats all_car_Details[4];
	int currency;
	CarSelection car_number;
	void Update(CarSelection updatedcar, int updatedcurrency);
	void Update();
	void GetData();
	bool IsInit();

	PlayerDetails(CarSelection car, int currency);
	PlayerDetails();

};

