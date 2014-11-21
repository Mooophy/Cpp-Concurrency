//!
//! @author Yue Wang
//! @date   21.11.2014
//!
//! @brief  this code tests para::Queue.
//!

#include <string>
#include "ordinary_queue.hpp"
#include "../include/utilities.hpp"

int main()
{
    //! construct and push elements
    para::Queue<std::string> q;
    for(auto&& i : {"ab","cd","efg"}) q.push(i);
    q.emplace("wxyz1234");

    //! pop and print
    for( ; !q.empty(); q.pop())    para::println(q.front());
    return 0;
}
//! @output
//!
//ab
//cd
//efg
//wxyz1234


