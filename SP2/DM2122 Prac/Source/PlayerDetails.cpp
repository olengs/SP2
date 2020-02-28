#include "PlayerDetails.h"

//int PlayerDetails::getCurrency()
//{
//	return currency;
//}
//
//CarStats PlayerDetails::getall_car_details(int number)
//{
//	return all_car_Details[number];
//}
//
//int PlayerDetails::getCarsBought()
//{
//	return CarsBought;
//}
//
//CarSelection PlayerDetails::getcar_number()
//{
//	return car_number;
//}

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
		car_number.SelectedCar.UpgradeFromStart();
		
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
	if (!IsInit())
	{
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

	else GetData();
	
}

PlayerDetails::PlayerDetails()
{
}
