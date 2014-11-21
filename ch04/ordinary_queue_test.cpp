//!
//! @author Yue Wang
//! @date   21.11.2014
//!
//! @brief  this code tests para::Queue.
//!

#include "ordinary_queue.hpp"
#include "../include/utilities.hpp"
#include <queue>
#include <string>

int main()
{
    //! construct queue and push elements
    para::Queue<std::string> q;
    for(auto&& i : {"ab","cd","efg","hi","jk"}) q.push(i);
    q.emplace("wxyz");

    //! pop all elements and print
    for(;!q.empty();q.pop())    para::println(q.front());
    return 0;
}
//! @output
//!
//ab
//cd
//efg
//hi
//jk
//wxyz

