#include "AllCarDetails.h"
AllCarDetails::AllCarDetails()
{

	if (!IsInit())
	{
		InitCarStats();

		car_Stats[2].lock = false;

		UpdateFile();

	}

	else GetData();

}

CarStats AllCarDetails::getCarStats(int number)
{
	return car_Stats[number];
}

void AllCarDetails::InitCarStats()
{
	
	car_Stats[0] = CarStats(3.f, 1.5f, 1.f, 1.f); //guangtheng car
	car_Stats[1] = CarStats(2.f, 2.f, 1.5f, 1.5f); //ryan car
	car_Stats[2] = CarStats(1.5f, 2.5f, 1.f, 1.f); //junchen car
	car_Stats[3] = CarStats(2.5f, 2.f, 1.5f, 1.5f); //jianfeng car

}

void AllCarDetails::InitCarStat(int carnumber)
{
	switch (carnumber)
	{
	case 0:
		car_Stats[0] = CarStats(3.f, 1.5f, 1.f, 1.f); //guangtheng car
		break;
	case 1:
		car_Stats[1] = CarStats(2.f, 2.f, 1.5f, 1.5f); //ryan car
		break;
	case 2:
		car_Stats[2] = CarStats(1.5f, 2.5f, 1.f, 1.f); //junchen car
		break;
	case 3:
		car_Stats[3] = CarStats(2.5f, 2.f, 1.5f, 1.5f); //jianfeng car
		break;
	default: 
		InitCarStats();
		break; 

	}
}

void AllCarDetails::SaveData(CarStats storage1, CarStats storage2, CarStats storage3, CarStats storage4)
{
	car_Stats[0] = storage1;
	car_Stats[1] = storage2;
	car_Stats[2] = storage3;
	car_Stats[3] = storage4;
	UpdateFile();
}

void AllCarDetails::UpdateFile()
{

	std::ofstream data("AllCarDetails.txt");
	if (data.is_open())
	{
		data.clear();
		std::string line;
		for (int i = 0; i < 4; ++i)
		{
			data << i << " - Car Choice" << std::endl;
			data << car_Stats[i].current_upgrade << " - Car Upgrade" << std::endl;
			data << car_Stats[i].lock << " - Car Lock" << std::endl;
			data << car_Stats[i].StatLevel[4] << " - car fuel level" << std::endl;
		}
		data.close();
	}
}

void AllCarDetails::InitScene(CarStats& carstat0, CarStats& carstat1, CarStats& carstat2, CarStats& carstat3)
{
	carstat0 = car_Stats[0];
	carstat1 = car_Stats[1];
	carstat2 = car_Stats[2];
	carstat3 = car_Stats[3];
}



bool AllCarDetails::IsInit()
{
	std::fstream data("AllCarDetails.txt");
	if (data.is_open())
	{
		data.close();
		return true;
	}
	else return false;
}

void AllCarDetails::GetData()
{
	std::ifstream data("AllCarDetails.txt");
	std::string line;
	if (data.is_open())
	{
		InitCarStats();

		std::getline(data, line);
		std::getline(data, line);
		car_Stats[0].current_upgrade = stof(line);
		car_Stats[0].UpgradeFromStart();
		std::getline(data, line);
		car_Stats[0].lock = stof(line);
		std::getline(data, line);
		car_Stats[0].StatLevel[4] = stof(line);
		
		std::getline(data, line);
		std::getline(data, line);
		car_Stats[1].current_upgrade = std::stof(line);
		car_Stats[1].UpgradeFromStart();
		std::getline(data, line);
		car_Stats[1].lock = stof(line);
		std::getline(data, line);
		car_Stats[1].StatLevel[4] = stof(line);

		std::getline(data, line);
		std::getline(data, line);
		car_Stats[2].current_upgrade = stof(line);
		car_Stats[2].UpgradeFromStart();
		std::getline(data, line);
		car_Stats[2].lock = stof(line);
		std::getline(data, line);
		car_Stats[2].StatLevel[4] = stof(line);

		std::getline(data, line);
		std::getline(data, line);
		car_Stats[3].current_upgrade = stof(line);
		car_Stats[3].UpgradeFromStart();
		std::getline(data, line);
		car_Stats[3].lock = stof(line);
		std::getline(data, line);
		car_Stats[3].StatLevel[4] = stof(line);

		data.close();
	}
	else return;
	
}

void AllCarDetails::UpdateCarStat(CarSelection update)
{
	car_Stats[update.cartype] = update.SelectedCar;
	UpdateFile();
}

