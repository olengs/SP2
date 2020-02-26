#pragma once
#include "CarStats.h"
class CarSelection 
{
public:
	CarStats SelectedCar;
	int cartype;
	void EquipCar(CarStats car, int cartype);
	CarSelection();
	CarSelection(CarStats chosenCar, int cartype);

};

