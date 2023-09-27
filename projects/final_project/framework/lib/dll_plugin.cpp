/************************************************
 Developer: Avri Kehat						    *
* Reviewed by: Tomer Savorai					*
* Date: 05.09.2023								*
* Description: Plugin for Dll Loader           	*
* Version: 1.0                                  *
* Status: Approved                             	*
*************************************************/
#include <iostream> //std::cout

#include "command.hpp"
#include "handleton.hpp"
#include "factory.hpp"

using namespace dev;

class Write : public ICommand
{
public:
    Write() : m_command("Write") {}
    void Execute();

private:
    const std::string m_command;
};

void Write::Execute()
{
    std::cout << m_command << std::endl;
}

class Read : public ICommand
{
public:
    Read() : m_command("Read") {}
    void Execute();

private:
    const std::string m_command;
};

void Read::Execute()
{
    std::cout << m_command << std::endl;
}

void MyCtor(void) __attribute__((constructor));
void MyCtor(void)
{
    Factory<ICommand,std::string> *factory = Handleton<Factory<ICommand, std::string>>::GetInstance();

    factory->Add("Write", []() { return new Write; });
    factory->Add("Read", []() { return new Read; });
}