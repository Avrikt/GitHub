/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Yotam Sasson   				*
* Date: 15.08.2023							*
* Description: Header file for Tasks    	*
* Version: 2.1                              *
* Status: Approved                          *
*********************************************/

#ifndef MY_PROJECT_TASKS_HPP
#define MY_PROJECT_TASKS_HPP

#include <future>

namespace dev
{
/************************************ ITask ************************************************/
class ITask
{
public:
    ITask() = default;
    virtual ~ITask() = default;
    
    virtual void Execute() = 0;
};

/************************************ FutureTask *******************************************/
template<typename ReturnType>
class FutureTask: public ITask
{
public:
    FutureTask(std::function<ReturnType()> _func);
    void Execute();
    ReturnType GetValue();

private:
    std::promise<ReturnType> m_result;
    std::function<ReturnType()> m_func;
};

template <typename ReturnType>
FutureTask<ReturnType>::FutureTask(std::function<ReturnType()> _func): m_func(_func) {}

template <typename ReturnType>
void FutureTask<ReturnType>::Execute()
{
    m_result.set_value(m_func());
}

template <typename ReturnType>
ReturnType FutureTask<ReturnType>::GetValue()
{
    return m_result.get_future().get();
}

}       //namespace dev
#endif  // MY_PROJECT_TASKS_HPP