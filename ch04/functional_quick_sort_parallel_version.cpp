//!
//! @author Yue Wang
//! @date   26.11.2014
//!
//! @brief  Implementation for Listing 4.13
//!

#include <../include/utilities.hpp>
#include <iostream>
#include <list>
#include <algorithm>
#include <future>

namespace para {

template<typename T>
std::list<T> parallel_functional_quick_sort(std::list<T> l)
{
    using std::list;
    using std::partition;
    using std::move;
    using std::future;
    using std::async;

    if(l.empty())   return l;   //trivial case

    list<T> ret;
    ret.splice(ret.end(),l,l.begin());
    T const& pivot = ret.front();
    auto divide_point = partition(l.begin(),l.end(),[&](T const& t) {
            return t < pivot;
    });

    list<T> lower;
    lower.splice(lower.end(),l,l.begin(),divide_point);

    future<list<T>> sorted_lower   =   async(&parallel_functional_quick_sort<T>,move(lower));
           list<T>  sorted_higher  =          parallel_functional_quick_sort(move(l));

    ret.splice(ret.end(),   sorted_higher);
    ret.splice(ret.begin(), sorted_lower.get());
    return ret;
}
}//namespace

int main()
{
    std::list<int> l {6,3,2,5,1,4};
    para::println_range(l);

    l = para::parallel_functional_quick_sort(l);
    para::println_range(l);

    return 0;
}
