#include "CarSelection.h"

void CarSelection::EquipCar(CarStats car, int cartype)
{
	SelectedCar = car;
	this->cartype = cartype;
}


CarSelection::CarSelection()
{
	cartype = 2;
}

CarSelection::CarSelection(CarStats chosenCar, int cartype)
{
	SelectedCar = chosenCar;
	this->cartype = cartype;
}
