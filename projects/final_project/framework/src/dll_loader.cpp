/************************************************
 Developer: Avri Kehat						    *
* Reviewed by: Tomer Savorai					*
* Date: 05.09.2023								*
* Description: Source file for DLL Loader		*
* Version: 1.0                              	*
* Status: Approved                             	*
*************************************************/

/************************************LIBRARIES**************************************************/
#include <dlfcn.h> //dlopen, dlerror

#include "dll_loader.hpp"

/************************************Functions***************************************************/

namespace dev
{

DLLLoader::DLLLoader(std::string directory_) : m_dllPath(directory_) {}

DLLLoader::~DLLLoader()
{
    for(auto handle : m_handles)
    {
        dlclose(handle.second);
    }
    m_handles.clear();
}

void DLLLoader::OnEvent(std::pair<DirMonitor::DirEvent, std::string> event_)
{
    if((event_.first == DirMonitor::FILE_ADDED) && IsSharedObject(event_.second))
    {
        #ifdef DEBUG
        std::cout << "New Shared Object recognized: " << event_.second << std::endl;
        #endif

        std::string fullPath((m_dllPath + "/" + event_.second));
        
        sleep(1);
        void *handle = dlopen(fullPath.c_str(), RTLD_LAZY);
        if(!handle)
        {
            throw std::runtime_error(dlerror());
        }
        m_handles[event_.second] = handle;
    }
    
    if(event_.first == DirMonitor::FILE_REMOVED && IsSharedObject(event_.second))
    {
        auto it = m_handles.find(event_.second);
        if(it != m_handles.end())
        {
            #ifdef DEBUG
            std::cout << "File removed: " << event_.second << std::endl;
            #endif
        }
    }
}

bool DLLLoader::IsSharedObject(const std::string &dllPath_)
{
    size_t pos = dllPath_.rfind(".so");

    return (pos != std::string::npos);
}
	
} //namespace dev
