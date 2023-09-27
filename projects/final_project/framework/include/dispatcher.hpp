/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 30.08.2023								*
* Description: Header file for dispatcher		*
* Version: 1.1                              	*
* Status: Sent                              	*
*************************************************/

#ifndef MY_PROJECT_DISPATCHER_HPP
#define MY_PROJECT_DISPATCHER_HPP

#include <iostream>
#include <vector>
#include <algorithm>

namespace dev
{

template <typename Event>
class Dispatcher;

/************************************ CallbackBase *******************************************/
template <typename Event>
class CallbackBase
{
public:
    explicit CallbackBase() noexcept;
    virtual ~CallbackBase() noexcept;

    CallbackBase(const CallbackBase& other) = delete;
    CallbackBase &operator=(const CallbackBase &other) = delete;
    CallbackBase(CallbackBase&& other) = delete;
    CallbackBase& operator=(CallbackBase&& other) = delete;

protected:
    void UnsubscribeBase();
    Dispatcher<Event> *m_dispatcher;

private:
    friend class Dispatcher<Event>;
    virtual void Notify(Event e_) = 0;
    virtual void NotifyDeath() = 0;
    
};

template <typename Event>
CallbackBase<Event>::CallbackBase() noexcept : m_dispatcher(nullptr) {}

template <typename Event>
CallbackBase<Event>::~CallbackBase() noexcept
{
    try
    {
        UnsubscribeBase();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

template <typename Event>
void CallbackBase<Event>::UnsubscribeBase()
{
    if (m_dispatcher)
    {
        m_dispatcher->Unsubscribe(*this);
        m_dispatcher = nullptr;
    }
}

/************************************ Callback *******************************************/
template <typename Event, class Observer>
class Callback : public CallbackBase<Event>
{
public:
    typedef void (Observer::*NotifyFunc)(Event);
    typedef void (Observer::*NotifyDeathFunc)();
    explicit Callback(Observer *obs_, NotifyFunc onEvent_, NotifyDeathFunc onDeath_ = nullptr);
    void Unsubscribe();

private:
    virtual void Notify(Event e_);
    virtual void NotifyDeath();
    Observer *m_observer;
    NotifyFunc m_onEvent;
    NotifyDeathFunc m_onDeath;
};

template <typename Event, class Observer>
Callback<Event, Observer>::Callback(Observer *obs_, NotifyFunc onEvent_, NotifyDeathFunc onDeath_):
                                    m_observer(obs_), m_onEvent(onEvent_), m_onDeath(onDeath_) {}

template <typename Event, class Observer>
void Callback<Event, Observer>::Unsubscribe()
{
    this->UnsubscribeBase();
    this->m_dispatcher = nullptr;
}

template <typename Event, class Observer>
void Callback<Event, Observer>::Notify(Event e_)
{
    (m_observer->*m_onEvent)(e_);
}

template <typename Event, class Observer>
void Callback<Event, Observer>::NotifyDeath()
{
    (m_observer->*m_onDeath)();
}

/************************************ Dispatcher *******************************************/
template <typename Event>
class Dispatcher
{
public:
    Dispatcher() = default;
    ~Dispatcher();

    Dispatcher(const Dispatcher& other) = delete;
    Dispatcher &operator=(const Dispatcher &other) = delete;
    Dispatcher(Dispatcher&& other) = delete;
    Dispatcher& operator=(Dispatcher&& other) = delete;

    void Subscribe(CallbackBase<Event> &callback);
    void Notify(Event e_);
    friend class CallbackBase<Event>;

private:
    void Unsubscribe(CallbackBase<Event> &callback);
    std::vector<CallbackBase<Event>*> m_callbacks;
};

template <typename Event>
Dispatcher<Event>::~Dispatcher()
{
    for (auto callback : m_callbacks)
    {
        try
        {
            callback->NotifyDeath();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        callback->m_dispatcher = nullptr;
    }
}

template <typename Event>
void Dispatcher<Event>::Subscribe(CallbackBase<Event> &callback)
{
    callback.m_dispatcher = this;
    m_callbacks.push_back(&callback);
}

template <typename Event>
void Dispatcher<Event>::Notify(Event e_)
{
    for (auto callback : m_callbacks)
    {
        callback->Notify(e_);
    }
}

template <typename Event>
void Dispatcher<Event>::Unsubscribe(CallbackBase<Event> &callback)
{
    auto it = std::find(m_callbacks.begin(), m_callbacks.end(), &callback);
    if(it != m_callbacks.end())
    {
        m_callbacks.erase(it);
    }
}

} //namespace dev

#endif //MY_PROJECT_DISPATCHER_HPP

