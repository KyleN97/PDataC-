#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
class PersonalData
{
public:
	//Default Empty Constructor
	PersonalData() : m_Name(""), m_Address(""), m_PhoneNumber("") {}
	//Construct a Personal Data with data passed in
	//To add another type of personal Data we can just expand this, or best would be to create another Constructor
	PersonalData(std::string _name, std::string _address, std::string _phoneNumber)
		: m_Name(_name), m_Address(_address), m_PhoneNumber(_phoneNumber) {}
	//Return a tmp vector which contains all the personal data
	std::vector<std::string> GetPersonalData() {
		return std::vector<std::string> { m_Name, m_Address, m_PhoneNumber };
	};
	//WIP - Doesnt work with std::getline, so will have members public, there shouldnt be much harm because of this
	////Implementation bellow for the Getters...
	//std::string GetName() { return m_Name; }
	//std::string GetAddress() { return m_Address; }
	//std::string GetPhoneNumber() { return m_PhoneNumber; }
	////Implementation below for the Setters...
	//void SetName(std::string _name) { m_Name = _name; }
	//void SetAddress(std::string _address) { m_Address = _address; }
	//void SetPhoneNumber(std::string _phoneNumber) { m_PhoneNumber = _phoneNumber; }

	//Member Variables for the Personal Data
	std::string m_Name;
	std::string m_Address;
	std::string m_PhoneNumber;

};

