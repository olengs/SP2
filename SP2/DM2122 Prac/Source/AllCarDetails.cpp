#include "AllCarDetails.h"
AllCarDetails::AllCarDetails()
{

	if (!IsInit())
	{
		car_Stats[0] = CarStats(2.f, 1.f, 2.f, 1.f, 1.f, 1.f); //guangtheng car
		car_Stats[1] = CarStats(2.f, 2.f, 1.f, 1.f, 1.f, 1.f); //ryan car
		car_Stats[2] = CarStats(1.f, 1.f, 1.f, 1.f, 1.f, 1.f); //junchen car
		car_Stats[3] = CarStats(1.f, 2.f, 2.f, 1.f, 1.f, 1.f); //jianfeng car

		car_Stats[2].lock = false;

		UpdateFile();

	}

	else GetData();

}

CarStats AllCarDetails::getCarStats(int number)
{
	return car_Stats[number];
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
		}
		data.close();
	}
}

void AllCarDetails::InitScene(CarStats& carstat0, CarStats& carstat1, CarStats& carstat2, CarStats carstat3)
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
	int a;
	if (data.is_open())
	{
		std::getline(data, line);
		a = std::stoi(line);
		std::getline(data, line);
		car_Stats[a].current_upgrade = stoi(line);
		car_Stats[a].UpgradeFromStart();
		std::getline(data, line);
		car_Stats[a].lock = stoi(line);

		std::getline(data, line);
		a = std::stoi(line);
		std::getline(data, line);
		car_Stats[a].current_upgrade = stoi(line);
		car_Stats[a].UpgradeFromStart();
		std::getline(data, line);
		car_Stats[a].lock = stoi(line);

		std::getline(data, line);
		a = std::stoi(line);
		std::getline(data, line);
		car_Stats[a].current_upgrade = stoi(line);
		car_Stats[a].UpgradeFromStart();
		std::getline(data, line);
		car_Stats[a].lock = stoi(line);
	
		std::getline(data, line);
		a = std::stoi(line);
		std::getline(data, line);
		car_Stats[a].current_upgrade = stoi(line);
		car_Stats[a].UpgradeFromStart();
		std::getline(data, line);
		car_Stats[a].lock = stoi(line);

		data.close();
	}
	else return;
	
}
