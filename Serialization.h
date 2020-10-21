#pragma once
#include "PersonalData.h"
enum DataStorageType { TXT, CSV, PDATA };//Format Type for the Serializer and UnSerializer
enum SerializationErrorType { UNITEST, FILEERROR };// Exception error types, just add a new type in here to define a certain type of error
//used to be able to do custom cases on certain exceptions if needed
struct SerializationError : public std::exception {
public:
	SerializationError(const char* message, SerializationErrorType errorType) : m_ErrorMessage(message), m_ErrorType(errorType) {}
	SerializationError(const char* message, SerializationErrorType errorType, const char* errorDetails) : m_ErrorMessage(message), m_ErrorType(errorType), m_ErrorDetails(errorDetails) {}
	SerializationError(const char* message, SerializationErrorType errorType, const char* errorDetails, const int lineNumber) : m_ErrorMessage(message), m_ErrorType(errorType), m_ErrorDetails(errorDetails), m_lineNumber(lineNumber) {}

	//Get The base Error
	const char* what() const noexcept { return m_ErrorMessage; }
	//Get the Details about the Error
	const char* GetErrorDetails() { return m_ErrorDetails; }
	//Get the LineNumber of the Error
	const int GetLineNumber() { return m_lineNumber; }
	//Get the Error Type
	SerializationErrorType GetError() { return m_ErrorType; }
private:
	const char* m_ErrorMessage = "";//Base Error of the Exception
	const char* m_ErrorDetails = "";// More Details about the Error
	const int m_lineNumber = 0;// Used to store the line number the exception was hit
	SerializationErrorType m_ErrorType;// Type of Exception error
};

class Serialization
{

public:
	//Default Constructor
	Serialization() : currentDataStorageType(DataStorageType::TXT), currentFilePath("") { }
	//Defauly DeStructorS
	~Serialization() { };
	//Used to retrieve the file extension of a passed in File.
	std::string GetFileExtension(const std::string& filePath)
	{
		if (filePath.find_last_of(".") != std::string::npos)
			return filePath.substr(filePath.find_last_of(".") + 1);
		return "";
	}
	//Convert string to DataStorageEnum - Used to convert file extension to Enum
	//When adding a new type add a case here for the new extension and set the appropritae DataStorageType
	DataStorageType ConvertStringToDataStorageType(const std::string & extension)
	{
		//Return a DataStorageType based on the extension passed in.
		if (extension == "txt") {
			return TXT;
		}
		else if (extension == "csv") {
			return CSV;
		}
		else if (extension == "pData") {
			return PDATA;
		}
		return TXT;
	}
	//Serialize Data
	void SerializeData(PersonalData & personalData, const std::string fileDirAndNameToSerialize, DataStorageType dataStorageType)
	{
		//set the data storage type for the serializer
		currentDataStorageType = dataStorageType;
		//set the current path for the serializer
		currentFilePath = fileDirAndNameToSerialize;
		//Write out the file to an ofstream
		std::ofstream out(fileDirAndNameToSerialize);
		//Check if we can write out to the file
		//else assert an error
		try
		{
			if (!out.fail())
			{
				switch (currentDataStorageType)
				{
				case DataStorageType::TXT:
					out << personalData.m_Name << "\n" << personalData.m_Address << "\n" << personalData.m_PhoneNumber << "\n";
					break;
				case DataStorageType::CSV:
					out << "Name,Address,Phone Number\n" << personalData.m_Name << "," << personalData.m_Address << "," << personalData.m_PhoneNumber << "\n";
					break;
				case DataStorageType::PDATA:
					//DeSerialize the actual object and pass out the new one
					out.write((char*)&personalData, sizeof(*&personalData));
					break;
					//Add Implementation here for a new Serialization by simply adding a new enum to DataStorageType and added the case statement here
				}
			}
			else {
				throw SerializationError("Couldn't Open file for writing - ", SerializationErrorType::FILEERROR, fileDirAndNameToSerialize.c_str());
			}
		}
		catch (SerializationError& e)
		{
			std::cout << "Exception Thrown...\n";
			std::cout << e.what() << std::endl;
			std::cout << e.GetErrorDetails() << std::endl;
		}

		out.close();
	};
	//UnSerializeData - Using a pointer to modify the value from the Main Func Call, so we dont need to copy
	//Not passing in a DataStorageType as we will retrieve that info from the passed in file path
	void UnSerializeData(PersonalData * personalData, const std::string fileDirAndNameToUnSerialize)
	{
		//set the current path for the serializer
		currentFilePath = fileDirAndNameToUnSerialize;
		//Read in the file to an ifstream
		std::ifstream in(fileDirAndNameToUnSerialize);
		//file opens correctly?
		try
		{
			if (!in.fail())
			{
				//Unserialize depending on the type
				switch (ConvertStringToDataStorageType(GetFileExtension(currentFilePath)))
				{
				case DataStorageType::TXT:
				{
					//Go through each line and assign the personal Data variables to the corresponding line in the doc
					std::getline(in, personalData->m_Name);
					std::getline(in, personalData->m_Address);
					std::getline(in, personalData->m_PhoneNumber);
					currentDataStorageType = DataStorageType::TXT;
					break;
				}
				case DataStorageType::CSV:
				{
					//Go through each line and assign the personal Data variables to the corresponding line in the doc
					//Skipping the first line in the csv and then going to a new line when a comma is found
					std::string tmp;
					std::getline(in, tmp);
					std::getline(in, personalData->m_Name, ',');
					std::getline(in, personalData->m_Address, ',');
					std::getline(in, personalData->m_PhoneNumber);
					currentDataStorageType = DataStorageType::CSV;
					break;
				}
				case DataStorageType::PDATA:
				{
					//Serialize the actual object compared to the member elements
					in.read((char*)&*personalData, sizeof(*personalData));
					currentDataStorageType = DataStorageType::PDATA;
					break;
				}
				//Add Implementation here for a new UnSerialization by simply adding a new enum to DataStorageType and added the case statement here
				}
			}
			else
			{
				throw SerializationError("Couldn't Open file for reading - ", SerializationErrorType::FILEERROR, fileDirAndNameToUnSerialize.c_str());
			}
		}
		catch (SerializationError& e)
		{
			std::cout << "Exception Thrown...\n";
			std::cout << e.what() << std::endl;
			std::cout << e.GetErrorDetails() << std::endl;
		}
		in.close();


	};
	//Support could be added for a Serialize and Deserialize of data if the file had for e.g - multiple sets of data in it and then it would output a list of those details or serialize that list to the one file/from one file
	//TODO ---
	//Serialize(std::vector<PersonalData&> listOfPersonalData,const std::string fileDirAndNameToSerialize, DataStorageType dataStorageType)
	//UnSerialize(std::vector<PersonalData*> listOfPersonalData,const std::string fileDirAndNameToSerialize\)
	//Print the personal data
	void PrintPersonalData(PersonalData & personalData)
	{
		//Access the personal data GetPersonalData array which spits out all the info into a vector, so we
		//loop through and print out each element
		for (size_t i = 0; i < personalData.GetPersonalData().size(); i++)
		{
			std::cout << personalData.GetPersonalData().at(i) << "\n";
		}
	}
	//Prinat all currently used dataTypes
	void PrintAvailableStorageTypes()
	{
		//Get all the datatypenames and loop through printing them
		std::cout << "Currently supported Serializable & UnSerializable Types: \n";
		for (size_t i = 0; i < m_DataStorageTypeNames.size(); i++)
		{
			std::cout << m_DataStorageTypeNames.at(i) << "\n";
		}
	}

	//Unit Tester
	void UnitTest(PersonalData* pData, PersonalData* pDataToTestAgainst, int lNumber)
	{
		//Testing the m_Elements against some test data passed in
		try
		{
			if (pData->m_Name != pDataToTestAgainst->m_Name) {
				throw SerializationError("Unit Test Exception caught",SerializationErrorType::UNITEST,"Name Does Not Match Name", lNumber);
			}
			if (pData->m_Address != pDataToTestAgainst->m_Address) {
				throw SerializationError("Unit Test Exception caught", SerializationErrorType::UNITEST, "Address Does Not Match Address", lNumber);
			}
			if (pData->m_PhoneNumber != pDataToTestAgainst->m_PhoneNumber) {
				throw SerializationError("Unit Test Exception caught", SerializationErrorType::UNITEST, "Phone Number Does Not Match Phone Number", lNumber);
			}
		}
		catch (SerializationError& e)
		{
			std::cout << e.what() << std::endl;
			std::cout << e.GetErrorDetails() << std::endl;
			std::cout << "Error Thrown at line " <<e.GetLineNumber() << std::endl;
		}



	}
private:
	const std::vector<std::string> m_DataStorageTypeNames = { "Text File", "CSV File", "Personal Data Object" };//List of the Data storage Types - Human Readable names
																						//add to this list to retrieve the list of file formats acccepted.
	DataStorageType currentDataStorageType;// Current Data Storage Type
	std::string currentFilePath;// Current File path
};

