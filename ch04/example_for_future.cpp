//!
//! @author Yue Wang
//! @date   23.11.2014
//!
//! @brief  Based on Listing 4.6, this code shows how to use std::future to get the
//!         return value from asynchronous task.
//!

#include <future>
#include "../include/utilities.hpp"

long long fibo(int n)
{
    return n<2  ?  n  :  fibo(n-1) + fibo(n-2);
}

int main()
{
    std::future<long long> ans = std::async(std::launch::async,fibo, 43);

    para::println(">sleeping");
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    para::println(">wake up");

    para::println("the answer = ") << ans.get() << std::endl;
    return 0;
}

//! @output
//!
//>sleeping
//>wake up
//the answer =
//433494437

