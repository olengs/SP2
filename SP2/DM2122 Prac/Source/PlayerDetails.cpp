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

void PlayerDetails::Update(CarSelection updatedcar, int updatedcurrency) //update in skybox scene
{
	std::ofstream data;
	data.open("PlayerDetails.txt");
	if (data.is_open())
	{
		data.clear();
		data << updatedcurrency << " - Currency " << std::endl;
		data << updatedcar.cartype << " - Equipped Car Choice" << std::endl;
		data << updatedcar.SelectedCar.current_upgrade << " - Equipped Car Upgrade Level" << std::endl;
		data << updatedcar.SelectedCar.StatLevel[4] << " - Equipped Car Fuel Level" << std::endl;
		data.close();
	}

	car_number = updatedcar;
	
	currency = updatedcurrency;

	allcardetails.UpdateFile();
}

void PlayerDetails::Update(float fuelLevel) //update in drive scene
{
	car_number.SelectedCar.StatLevel[4] = fuelLevel;
	UpdateFile();
}

void PlayerDetails::UpdateFile() //update file
{
	std::ofstream data;
	data.open("PlayerDetails.txt");
	if (data.is_open())
	{
		data << currency << " - Currency " << std::endl;
		data << car_number.cartype << " - Equipped Car Choice" << std::endl;
		data << car_number.SelectedCar.current_upgrade << " - Equipped Car Upgrade Level" << std::endl;
		data << car_number.SelectedCar.StatLevel[4] << " - equipped car fuel level" << std::endl;
		data.close();
	}

	allcardetails.UpdateCarStat(car_number);

}

void PlayerDetails::UpdateAllCarDetails()
{
	
}

void PlayerDetails::GetData()
{
	std::ifstream data("PlayerDetails.txt");
	if (data.is_open())
	{
		std::string line;
		std::getline(data, line);
		currency = std::stof(line);
		std::getline(data, line);
		car_number.cartype = std::stof(line);
		std::getline(data, line);
		InitCarStat(car_number.cartype);
		car_number.SelectedCar.current_upgrade = std::stof(line);
		car_number.SelectedCar.UpgradeFromStart();
		std::getline(data, line);
		car_number.SelectedCar.StatLevel[4] = std::stof(line);
		data.close();
		car_number.SelectedCar.lock = false;
	}
	else return;
}

void PlayerDetails::InitCarStat(int carnumber)
{
	allcardetails.InitCarStat(carnumber);

	car_number.SelectedCar = allcardetails.getCarStats(carnumber);
	
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
