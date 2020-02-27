#include "PlayerDetails.h"

void PlayerDetails::Update(CarSelection updatedcar, int updatedcurrency)
{
	std::ofstream data;
	data.open("PlayerDetails.txt");
	if (data.is_open())
	{
		data.clear();
		data << updatedcurrency << std::endl;
		data << updatedcar.SelectedCar.StatLevel[0] << std::endl
			<< updatedcar.SelectedCar.StatLevel[1] << std::endl
			<< updatedcar.SelectedCar.StatLevel[2] << std::endl
			<< updatedcar.cartype << std::endl;
		data.close();
	}
	car_number = updatedcar;
	
	currency = updatedcurrency;
}

void PlayerDetails::Update()
{
	std::ofstream data;
	data.open("PlayerDetails.txt");
	if (data.is_open())
	{
		data << currency << " - Currency " << std::endl;
		data << car_number.cartype << " - Equipped Car Choice" << std::endl;
		data << car_number.SelectedCar.current_upgrade << " - Equipped Car Upgrade Level" << std::endl;
		data.close();
	}
}

void PlayerDetails::GetData()
{
	std::ifstream data("PlayerDetails.txt");
	if (data.is_open())
	{
		std::string line;
		std::getline(data, line);
		currency = std::stoi(line);
		std::getline(data, line);
		car_number.cartype = std::stoi(line);
		std::getline(data, line);
		car_number.SelectedCar.current_upgrade = std::stoi(line);
		for (int i = 0; i < car_number.SelectedCar.current_upgrade; ++i)
		{
			car_number.SelectedCar.UpgradeOnce();
		}
		data.close();
		car_number.SelectedCar.lock = false;
	}
	else return;
}

bool PlayerDetails::IsInit()
{
	std::fstream data;
	data.open("PlayerDetails.txt");
	if (data.is_open())
	{
		data.close();
		return true;
	}
	else return false;
}


PlayerDetails::PlayerDetails(CarSelection car, int currency)
{

	all_car_Details[0] = CarStats(2.f, 1.f, 2.f, 1.f); //guangtheng car
	all_car_Details[1] = CarStats(2.f, 2.f, 1.f, 1.f); //ryan car
	all_car_Details[2] = car.SelectedCar; //junchen car
	all_car_Details[3] = CarStats(1.f, 2.f, 2.f, 1.f); //jianfeng car

	std::ofstream data;
	data.open("PlayerDetails.txt");
	if (data.is_open())
	{
		data << currency << " - Currency " << std::endl;
		data << car.cartype << " - Equipped Car Choice" << std::endl;
		data << car.SelectedCar.current_upgrade << " - Equipped Car Upgrade Level" << std::endl;
		data.close();
	}
	else std::cout << "unable to open player details file \n";
	car_number = car;

	for (int i = 0; i < car_number.SelectedCar.current_upgrade; ++i)
	{
		car_number.SelectedCar.UpgradeOnce();
	}
	
	this->currency = currency;
}

PlayerDetails::PlayerDetails()
{
}
