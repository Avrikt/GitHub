/************************************************
* Developer: Avri Kehat							*
* Reviewed by: Tomer Savorai					*
* Date: 05.09.2023								*
* Description: Header file for Dll Loader		*
* Version: 1.0                              	*
* Status: Approved                             	*
*************************************************/

#ifndef MY_PROJECT_DLL_LOADER_HPP
#define MY_PROJECT_DLL_LOADER_HPP

#include <unordered_map>

#include "dir_monitor.hpp"

namespace dev
{

class DLLLoader
{
public:
    DLLLoader(std::string directory_);
    ~DLLLoader();
    
    void OnEvent(std::pair<DirMonitor::DirEvent, std::string> event_);

private:
    bool IsSharedObject(const std::string &dllPath_);
    
    const std::string m_dllPath;
    std::unordered_map<std::string, void *> m_handles;
};

} //namespace dev

#endif //MY_PROJECT_DLL_LOADER_HPP

