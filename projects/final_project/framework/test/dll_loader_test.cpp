/************************************************
* Developer: Avri Kehat							*
* Reviewed by: Tomer Savorai					*
* Date: 05.09.2023								*
* Description: Test file for DLL Loader			*
* Version: 1.0                              	*
* Status: Approved                             	*
*************************************************/

/************************************LIBRARIES**************************************************/
#include <filesystem>

#include "factory.hpp"
#include "command.hpp"
#include "handleton.hpp"
#include "dll_loader.hpp"
#include "utilities.hpp"

using namespace dev;

namespace fs = std::filesystem;

int main(void)
{
	HEADER(DLLLoader Test);
	
	std::string dir_path("./lib");
	if(!fs::exists(dir_path))
	{
		if(!fs::create_directory(dir_path))
		{
			std::cerr << "Error creating directory\n";
		}
	}

	DirMonitor dm(dir_path);
	
	DLLLoader *dl = Handleton<DLLLoader, std::string>::GetInstance(dir_path);
	Factory<ICommand, std::string> *factory = Handleton<Factory<ICommand, std::string>>::GetInstance();

	Callback<std::pair<DirMonitor::DirEvent, std::string>, DLLLoader> cb(dl, &DLLLoader::OnEvent);

	dm.AddCallback(cb);

	std::cout << "Enter 'x' to exit\n";
	while('x' != getc(stdin));
	
	ICommand *c_r = factory->Create("Write");
	ICommand *c_w = factory->Create("Read");

	c_w->Execute();
	c_r->Execute();

	delete c_w;
	delete c_r;

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/
