/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Tomer Savorai					*
* Date: 03.09.2023								*
* Description: Source file for Dir Monitor  	*
* Version: 1.0                              	*
* Status: Approved                             	*
*************************************************/

/************************************LIBRARIES**************************************************/
#include <dirent.h>     // opendir, readrid, closedir

#include "dir_monitor.hpp"

/************************************Functions***************************************************/

namespace dev
{

DirMonitor::DirMonitor(const std::string &directory_) : m_directory(directory_), m_running(true)
{
    if(-1 == (m_fd = inotify_init()))
    {
        std::cerr << "inotify_init()\n";
        throw;
    }

    if(-1 == (m_wd = inotify_add_watch(m_fd, directory_.c_str(), IN_CREATE | IN_DELETE | IN_MOVE)))
    {
        close(m_fd);
        throw std::runtime_error("inotify_add_watch()\n");
    }

    try
    {
        m_worker = std::thread(&DirMonitor::WorkingThread, this);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Failed to create thread: " << e.what() << '\n';
        close(m_fd);
        throw e;
    }
}

DirMonitor::~DirMonitor()
{
    m_running = false;
    inotify_rm_watch(m_fd, m_wd);
    m_worker.join();
    close(m_fd);
}

void DirMonitor::WorkingThread()
{
    const int BUFFER_SIZE = 2048;
    
    ScanDirectory();

    while(m_running)
    {
        std::vector<char> buf(BUFFER_SIZE);
        
        ssize_t bytes_read = read(m_fd, buf.data(), buf.size());
        if(-1 == bytes_read)
        {
            if(errno == EBADF)
            {
                break;
            }
            else if (errno != EAGAIN)
            {
                throw std::runtime_error("Failed to read");
            }
        }

        for (ssize_t i = 0; i < bytes_read;)
        {
            struct inotify_event *event = reinterpret_cast<struct inotify_event *>(&buf[i]);
            
            std::pair<DirEvent, std::string> eventData(FILE_ADDED, event->name);

            if(event->mask & IN_CREATE || event->mask & IN_MOVED_TO)
            {
                eventData.first = FILE_ADDED;
            }
            
            if(event->mask & IN_DELETE || event->mask & IN_MOVED_FROM)
            {
                eventData.first = FILE_REMOVED;
            }

            m_dispatcher.Notify(eventData);

            i += sizeof(inotify_event) + event->len;
        }
    }
}

void DirMonitor::ScanDirectory()
{
    DIR *dir = opendir(m_directory.c_str());

    if(!dir)
    {
        throw std::runtime_error("opendir()");
    }

    struct dirent *dirent_s = nullptr;

    while(nullptr != (dirent_s = readdir(dir)))
    {
        if((DT_REG == dirent_s->d_type))
        {
            m_dispatcher.Notify(std::pair<DirEvent, std::string>(FILE_ADDED, dirent_s->d_name));
        }
    }

    closedir(dir);
}

void DirMonitor::AddCallback(CallbackBase<std::pair<DirEvent,std::string>> &callback_)
{
    m_dispatcher.Subscribe(callback_);
}
	
} //namespace dev
