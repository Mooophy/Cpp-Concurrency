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

/**
 * @brief parallel_functional_quick_sort
 * @param list
 * @return sorted list
 *
 * @concept for T : operator <
 *
 * using platform dependent number of threads.
 */
template<typename T>
std::list<T> parallel_functional_quick_sort(std::list<T> l)
{
    using std::list;
    using std::partition;
    using std::move;
    using std::future;
    using std::async;

    if(l.empty())   return l;   //trivial case

    //! use the first element as pivot.
    list<T> ret;
    ret.splice(ret.end(),l,l.begin());
    T pivot = ret.front();
    auto divide_point = partition(l.begin(),l.end(),[&](T const& t) {
            return t < pivot;
    });

    //!split l into two parts, lower and higher
    list<T> lower;
    lower.splice(lower.end(),l,l.begin(),divide_point);

    //! @brief recursion
    //! @attention  since futrue and async are used here, std library will decide if new
    //!             thread is needed for sorted_lower. As a result, threads number will differ
    //!             on different platform.
    future<list<T>> sorted_lower   =   async(&parallel_functional_quick_sort<T>,move(lower));
           list<T>  sorted_higher  =          parallel_functional_quick_sort(move(l));

    //! merge by splicing
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
//! output
//para> 6 3 2 5 1 4
//para> 1 2 3 4 5 6
