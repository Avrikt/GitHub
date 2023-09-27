/************************************************
* Developer: Avri Kehat							*
* Reviewed by: Tomer Savorai					*
* Date: 03.09.2023								*
* Description: Test file for Dir Monitor		*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

/************************************LIBRARIES**************************************************/

#include "dir_monitor.hpp"

using namespace dev;

class Observer
{
public:
	void OnEvent(std::pair<DirMonitor::DirEvent, std::string> event)
	{	
		if(event.first == DirMonitor::DirEvent::FILE_ADDED)
		{
			std::cout << "File Added: " << event.second << std::endl;
		}

		if (event.first == DirMonitor::DirEvent::FILE_REMOVED)
		{
			std::cout << "File Removed: " << event.second << std::endl;
		}
	}
};

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " 'Filepath' " << std::endl;
		return 1;
	}

	DirMonitor monitor(argv[1]);

	Observer obs;

	Callback<std::pair<DirMonitor::DirEvent, std::string>, Observer>
			cb (&obs, &Observer::OnEvent);

	monitor.AddCallback(cb);

	std::cout << "Enter 'x' to exit\n";
	while('x' != getc(stdin));

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/
