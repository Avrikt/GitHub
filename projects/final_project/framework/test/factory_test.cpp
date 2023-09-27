/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Aviv Haklai   				    *
* Date: 22.08.2023						    	*
* Description: Test file for Factory          	*
* Version: 1.1                                  *
* Status: Approved                              *
*************************************************/
#include <iostream>

#include "factory.hpp"

using namespace dev;

class Animal
{
public:
    Animal(int leg_ = 4);
    virtual void  MakeSound() = 0;
    virtual ~Animal() = default;
    int GetLegs();

private:
    int m_legs;
};


Animal::Animal(int leg_): m_legs(leg_)
{}

int Animal::GetLegs()
{
    return m_legs;
}

class Dog: public Animal
{
public:
    Dog(int leg_ = 4);
    void MakeSound();
    static Animal *MakeDog(int leg_ = 4);
};

Dog::Dog(int leg_):Animal(leg_)
{}

void Dog::MakeSound()
{
    std::cout<<"Woof! Num of legs = "<<GetLegs()<<std::endl;
}

Animal *Dog::MakeDog(int leg_)
{
    Animal *tmp = new Dog(leg_);
    return  tmp;
}

class Cat: public Animal
{
public:
    Cat(int leg_ = 4);
    void MakeSound();
    static Animal *MakeCat(int leg_ = 4);

};

Cat::Cat(int leg_):Animal(leg_)
{}

void Cat::MakeSound()
{
    std::cout<<"Meow! Num of legs = "<<GetLegs()<<std::endl;
}

Animal *Cat::MakeCat(int leg_)
{
    Animal *tmp = new Cat(leg_);
    return  tmp;
}

int main(void)
{
    Factory<Animal, std::string, int> factory;

    factory.Add("Dog", Dog::MakeDog);
    factory.Add("Cat", Cat::MakeCat);

    Animal *an1 = factory.Create("Dog", 6);
    Animal *an2 = factory.Create("Cat", 7);

    an1->MakeSound();
    an2->MakeSound();

    delete an1;
    delete an2;

    return 0;
}