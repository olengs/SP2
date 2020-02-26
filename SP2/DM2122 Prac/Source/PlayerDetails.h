#pragma once
#include "CarSelection.h"
#include "CarStats.h"
#include <fstream>
#include <iostream>
#include <string>

class PlayerDetails
{
public:
	int currency;
	CarSelection car_number;
	void Update(CarSelection updatedcar, int updatedcurrency);
	void GetData();
	PlayerDetails(CarSelection car, int currency);
	PlayerDetails();

};

