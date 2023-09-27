/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Aviv Haklai   				    *
* Date: 28.08.2023						    	*
* Description: Plugin for Handleton            	*
* Version: 1.1                                  *
* Status: Approved                              *
*************************************************/
#include <iostream>

#include "transport.hpp"
#include "handleton.hpp"
#include "factory.hpp"

using namespace dev;

class Truck : public ITransport
{
public:
    Truck() : m_vehicle("Truck") {}
    void Drive();

private:
    const std::string m_vehicle;
};

void Truck::Drive()
{
    std::cout << m_vehicle << std::endl;
}

class Ship : public ITransport
{
public:
    Ship() : m_vehicle("Ship") {}
    void Drive();

private:
    const std::string m_vehicle;
};

void Ship::Drive()
{
    std::cout << m_vehicle << std::endl;
}

class Car : public ITransport
{
public:
    Car() : m_vehicle("Car") {}
    void Drive();

private:
    const std::string m_vehicle;
};

void Car::Drive()
{
    std::cout << m_vehicle << std::endl;
}

void MyCtor(void) __attribute__((constructor));
void MyCtor(void)
{
    Factory<ITransport,std::string> *factory = Handleton<Factory<ITransport, std::string>>::GetInstance();

    factory->Add("Truck", []() { return new Truck; });
    factory->Add("Ship", []() { return new Ship; });
    factory->Add("Car", []() { return new Car; });
}