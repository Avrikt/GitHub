/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 21.09.2023								*
* Description: Test file for Framework Prototype*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

#include "singleton.hpp"
#include "input_mediator.hpp"
#include "input_proxy.hpp"
#include "ndb_proxy.hpp"

using namespace dev;

void InitFramework(timeval reactor_timeout_,
                   std::size_t tp_num_threads_,
                   int thread_pool_sched_policy_,
                   int thread_pool_niceness_,
                   std::vector<IInputProxy*>& nbd_proxy);

const timeval REACTOR_TIMEOUT{7, 0};

int main(void)
{
    std::vector<IInputProxy*> proxyVector;
    NBDProxy *proxy = new NBDProxy;

    proxyVector.push_back(proxy);

    InitFramework(REACTOR_TIMEOUT, std::thread::hardware_concurrency(), SCHED_IDLE, 0, proxyVector);

    return 0;
}

void InitFramework(timeval reactor_timeout_,
                   std::size_t tp_num_threads_,
                   int thread_pool_sched_policy_,
                   int thread_pool_niceness_,
                   std::vector<IInputProxy*>& nbd_proxy)
{
    ThreadPool *thread_pool = Singleton<ThreadPool, std::size_t, int, int>
                              ::GetInstance(tp_num_threads_, thread_pool_niceness_, thread_pool_sched_policy_);

    Reactor *reactor = Singleton<Reactor, timeval>::GetInstance(reactor_timeout_);

    InputMediator mediator(*reactor, *thread_pool);

    for (auto nbd_item : nbd_proxy)
    {
        mediator.Add(Reactor::READ, STDIN_FILENO, *nbd_item);
    }

    reactor->Run();

    sleep(5);
}