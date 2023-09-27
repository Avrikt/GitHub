/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Lihi Cohen   				    *
* Date: 13.08.2023						    	*
* Description: Header file for Priority Queue  	*
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/
#ifndef MY_PROJECT_PQ_HPP
#define MY_PROJECT_PQ_HPP

#include <queue> //priority_queue

namespace dev
{
template<
        typename T,
        typename Container = std::vector<T>,
        typename Compare = std::less<typename Container::value_type>>
class PQ: private std::priority_queue<T, Container, Compare>
{
public:
    using std::priority_queue<T, Container, Compare>::value_type;
    using std::priority_queue<T, Container, Compare>::reference;

    using std::priority_queue<T, Container, Compare>::push;
    using std::priority_queue<T, Container, Compare>::pop;
    using std::priority_queue<T, Container, Compare>::empty;
    inline const T& front() const;
};

template<typename T, typename Container, typename Compare>
const T& PQ<T,Container,Compare>::front() const
{
    return this->top();
}

} // namespace dev

#endif // MY_PROJECT_PQ_HPP