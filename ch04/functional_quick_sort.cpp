//!
//! @author Yue Wang
//! @date   26.11.2014
//!
//! @brief  Implementation for Listing 4.12

#include <../include/utilities.hpp>
#include <iostream>
#include <list>
#include <algorithm>

namespace para {

/**
 * @brief sequential_functional_quick_sort
 *
 * for Listing 4.12
 */
template<typename T>
std::list<T> sequential_functional_quick_sort(std::list<T> input)
{
    if(input.empty())   return input;   //  termination condtion

    std::list<T> ret;
    ret.splice(ret.begin(),input,input.begin());
    T pivot = ret.front();
    auto divide_point = std::partition(input.begin(),input.end(),
        [=](T const& t){
            return t < pivot;
        });

    std::list<T> lower;
    lower.splice(lower.end(), input, input.begin(),divide_point);

    auto new_lower (para::sequential_functional_quick_sort(lower));     //recur
    auto new_higher(para::sequential_functional_quick_sort(input));     //recur

    ret.splice(ret.end(), new_higher);
    ret.splice(ret.begin(),new_lower);
    return ret;
}
}//namespace

int main()
{
    std::list<int> l {6,3,2,5,1,4};
    para::println_range(l);

    l = para::sequential_functional_quick_sort(l);
    para::println_range(l);

    return 0;
}
//! @output
//!
//para> 6 3 2 5 1 4
//para> 1 2 3 4 5 6
