#pragma once
#include "CarSelection.h"
#include "CarStats.h"
#include <fstream>
#include <iostream>
#include <string>
#include "AllCarDetails.h"

class PlayerDetails
{

public:
	int currency;
	int coinCounter;
	CarSelection car_number;
	AllCarDetails allcardetails;

	//getters
	/*int getCurrency();
	CarStats getall_car_details(int number);
	int getCarsBought();
	CarSelection getcar_number();*/

	void Update(CarSelection updatedcar, int updatedcurrency);
	void Update(float fuelLevel);
	void UpdateFile();
	void UpdateAllCarDetails();
	void GetData();
	bool IsInit();
	void InitCarStat(int carnumber);
	
	PlayerDetails(CarSelection car, int currency);
	PlayerDetails();

};

