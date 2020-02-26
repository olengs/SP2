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
		data.clear();
		data << currency << " - Currency" << std::endl;
		data << car_number.SelectedCar.StatLevel[0] << " - Equipped Car Acceleration " << std::endl
			<< car_number.SelectedCar.StatLevel[1] << " - Equipped Car Turbo "<< std::endl
			<< car_number.SelectedCar.StatLevel[2]  << " - Equipped Car Fuel " << std::endl
			<< car_number.cartype << " - Equipped Car Choice" << std::endl;
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
		car_number.SelectedCar.StatLevel[0] = std::stoi(line);
		std::getline(data, line);
		car_number.SelectedCar.StatLevel[1] = std::stoi(line);
		std::getline(data, line);
		car_number.SelectedCar.StatLevel[2] = std::stoi(line);
		std::getline(data, line);
		car_number.cartype = std::stoi(line);
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
	std::ofstream data;
	data.open("PlayerDetails.txt");
	if (data.is_open())
	{
		data << currency << " - Currency "<< std::endl;
		data << car.SelectedCar.StatLevel[0] << " - Equipped Car Acceleration" << std::endl;
		data << car.SelectedCar.StatLevel[1] << " - Equipped Car Turbo" << std::endl;
		data << car.SelectedCar.StatLevel[2] << " - Equipped Car Fuel" << std::endl;
		data << car.cartype << " - Equipped Car Choice"<< std::endl;
		data.close();
	}
	else std::cout << "unable to open player details file \n";
	car_number = car;
	this->currency = currency;
}

PlayerDetails::PlayerDetails()
{
}
