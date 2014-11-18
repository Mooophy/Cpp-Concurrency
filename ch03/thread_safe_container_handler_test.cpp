//!
//! @author Yue Wang
//! @date   18.11.2014
//!
//! @brief  this is a test code for para::ContainerHandler.
//!

#include "thread_safe_container_handler.hpp"

int main()
{
    /**
     * @brief test for move semantics
     */
    std::list<int> list(3,42);
    para::ContainerHandler<std::list<int>> handler(list);
    handler.print()     <<  std::endl;

    auto handler2 (std::move(handler));
    handler2.print()    <<  std::endl;

    auto handler3 = std::move(handler2);
    handler3.print()    <<  std::endl;

    /**
     * @brief test for members
     */
    handler3.add(999);
    handler3.print()    <<  std::endl;
    std::cout << (handler3.if_contains(999)? "found\n" : "no such thing\n");

    return 0;
}
//! output
//!
//42 42 42
//42 42 42
//42 42 42
//42 42 42 999
//found


