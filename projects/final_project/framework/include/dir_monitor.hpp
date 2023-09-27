/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Tomer Savorai					*
* Date: 03.09.2023								*
* Description: Header file for Dir Monitor  	*
* Version: 1.0                              	*
* Status: Approved                             	*
*************************************************/

#ifndef MY_PROJECT_DIR_MONITOR_HPP
#define MY_PROJECT_DIR_MONITOR_HPP

#include <boost/noncopyable.hpp>    // boost::noncopyable
#include <sys/inotify.h>            // inotify
#include <atomic>                   // std::atomic
#include <thread>                   // std::thread
#include <string>                   // std::string

#include "dispatcher.hpp"

namespace dev
{


class DirMonitor : private boost::noncopyable
{
public:
    
    DirMonitor(const std::string &directory_);
    ~DirMonitor();
    
    enum DirEvent
    {
        FILE_ADDED,
        FILE_REMOVED,
    };
  
    void AddCallback(CallbackBase<std::pair<DirEvent,std::string>> &callback_);
    
private:
    void WorkingThread();
    void ScanDirectory();


    Dispatcher<std::pair<DirEvent,std::string>> m_dispatcher;
    const std::string m_directory;
    std::thread m_worker;
    bool m_running;
    int m_fd;
    int m_wd;
};

} //namespace dev

#endif //MY_PROJECT_DIR_MONITOR_HPP

