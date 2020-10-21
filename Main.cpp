#include "Serialization.h"
#include <stdio.h>  /* defines FILENAME_MAX */
#define WINDOWS  /* comment this line to use it for linux.*/ 
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
//Get The Current Working Directory
std::string GetCurrentWorkingDir(void) {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}
int main()
{
	//Create the serializer
	Serialization* serializer = new Serialization();
	//Create 6 sets of test people
	//first three we hard code in here with some random data
	//last three are just created, ready to have data inserted
	//Created as a vector to be able to easily add new data
	std::vector<PersonalData*>* allPersonalData = new std::vector<PersonalData*>();
	allPersonalData->push_back(new PersonalData("Test User 1", "Test Address 1", "1"));
	allPersonalData->push_back(new PersonalData("Test User 2", "Test Address 2", "2"));
	allPersonalData->push_back(new PersonalData("Test User 3", "Test Address 3", "3"));
	allPersonalData->push_back(new PersonalData());
	allPersonalData->push_back(new PersonalData());
	allPersonalData->push_back(new PersonalData());

	//Print all available storage types
	serializer->PrintAvailableStorageTypes();
	//Serialize the test data into THREE formats - Easy to change save format by changing the ENUM
	//Could loop through all people and serialize if that would be what you needed
	std::cout << "---Serialize Data---\n";
	serializer->SerializeData(*allPersonalData->at(0), GetCurrentWorkingDir() + "/dataOut.txt", DataStorageType::TXT);
	serializer->SerializeData(*allPersonalData->at(1), GetCurrentWorkingDir() + "/dataOut.csv", DataStorageType::CSV);
	serializer->SerializeData(*allPersonalData->at(2), GetCurrentWorkingDir() + "/dataOut.pData", DataStorageType::PDATA);
	//UnSerialize the test data and the format is discovered via the extensions name
	std::cout << "---UnSerialize Data---\n";
	serializer->UnSerializeData(allPersonalData->at(3), GetCurrentWorkingDir() + "/data.txt");
	serializer->UnSerializeData(allPersonalData->at(4), GetCurrentWorkingDir() + "/data.csv");
	//---This will be used for a test case to test the exceptions, this file doesnt exist so it should throw and excpetion and keep running
	serializer->UnSerializeData(allPersonalData->at(5), GetCurrentWorkingDir() + "/data.pData");

	//Print the data of each Person
	std::cout << "---Print Data---\n";
	for (size_t i = 0; i < allPersonalData->size(); i++)
	{
		std::cout << "Data for Person " + std::to_string(i) <<std::endl;
		serializer->PrintPersonalData(*allPersonalData->at(i));
	}

	//Unit Test -- check if variables are what they should be//
	//This is referring to a set of test data in two files and a set of test data passed in just at the start of the program
	serializer->UnitTest(allPersonalData->at(0), new PersonalData("Test User 1", "Test Address 1", "1"), __LINE__);
	serializer->UnitTest(allPersonalData->at(1), new PersonalData("Test User 2", "Test Address 2", "2"), __LINE__);
	serializer->UnitTest(allPersonalData->at(2), new PersonalData("Test User 3", "Test Address 3", "3"), __LINE__);
	serializer->UnitTest(allPersonalData->at(3), new PersonalData("Test User 4", "Test Address 4", "4"), __LINE__);
	serializer->UnitTest(allPersonalData->at(4), new PersonalData("Test User 5", "Test Address 5", "5"), __LINE__);
	serializer->UnitTest(allPersonalData->at(5), new PersonalData("Test User 6", "Test Address 6", "6"), __LINE__);

	system("pause");
	return 0;
}