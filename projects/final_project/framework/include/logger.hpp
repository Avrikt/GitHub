/************************************************
* Developer: Avri Kehat							*
* Reviewed by: Aaron Sarfaty					*
* Date: 29.08.2023							    *
* Description: Header file for Logger		    *
* Version: 1.0                              	*
* Status: Approved                             	*
*************************************************/

#ifndef MY_PROJECT_LOGGER_HPP
#define MY_PROJECT_LOGGER_HPP

#include <string>                   //std::string
#include <fstream>                  //std::ofstream
#include <thread>                   //std::thread
#include <queue>                    //std::queue
#include <atomic>

#include "waitable_q.hpp"

namespace dev
{

class Logger
{
public:
    Logger(const std::string& log_file_name);
    ~Logger();

    Logger(const Logger &other) = delete;
    Logger &operator=(const Logger &other) = delete;
    Logger(Logger&& other) = delete;
    Logger& operator=(Logger&& other) = delete;

    void LogMessage(const std::string& message);

private:
    void WorkerThread();
    const std::string CurrentDateTime();
    void CleanUp(const std::string &msg);
    
    std::ofstream m_log_file;
    std::thread m_worker;
    WaitableQueue<std::queue<std::string>> m_log_entries;
};

} //namespace dev

#endif //MY_PROJECT_LOGGER_HPP

