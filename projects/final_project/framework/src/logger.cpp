/************************************************
* Developer: Avri Kehat							*
* Reviewed by: Aaron Sarfaty					*
* Date: 29.08.2023							    *
* Description: Source file for Logger			*
* Version: 1.0                              	*
* Status: Approved                             	*
*************************************************/

/************************************LIBRARIES**************************************************/
#include <iomanip>      //std::put_time

#include "logger.hpp"

/************************************Functions***************************************************/

namespace dev
{

Logger::Logger(const std::string& log_file_name): m_log_file(log_file_name, std::ios_base::app)
{
    struct sched_param param;
    param.sched_priority = sched_get_priority_min(SCHED_IDLE);
    
    if(!m_log_file.is_open())
    {
        throw std::runtime_error("Failed to open log file");
    }
    
    LogMessage("Log file created");

    try
    {
        m_worker = std::thread(&Logger::WorkerThread, this);

        if(0 != pthread_setschedparam(m_worker.native_handle(), SCHED_IDLE, &param))
        {
            throw std::runtime_error("Failed to set policy");
        }
    }
    catch(const std::exception& e)
    {
        CleanUp("Exception caught");
        throw e;
    } 
}

Logger::~Logger()
{
    CleanUp("End of Logger");
}

void Logger::LogMessage(const std::string& message)
{
    std::string logEntry('[' + CurrentDateTime() + "]: " + message);
    m_log_entries.Push(logEntry);
}

void Logger::WorkerThread()
{
    std::string msg;
    
    while((msg != "End of Logger") && (msg != "Exception caught"))
    {
        m_log_entries.Pop(msg);
        m_log_file << msg << std::endl;
    }
}

void Logger::CleanUp(const std::string &msg)
{
    m_log_entries.Push(msg);
    m_worker.join();
    m_log_file.close();
}

const std::string Logger::CurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %X");
    return ss.str();
}

} //namespace dev