//!
//! @author Yue Wang
//! @date   17 11 2014
//!
//! @note   As classes like std::unique_ptr, std::iostream, boost::asio::socket,etc,
//!         std::thread is moveable but can't be copied.
//!

#include <thread>

int some_function()
{
    return 0;
}

int some_other_function()
{
    return 0;
}

int main()
{
    std::thread t1{some_function};
    std::thread t2 = std::move(t1); //move constructor
    t1 = std::thread{some_other_function};
    //!  ^^^^^^^^^^^
    //!  As it's an Rvalue, move assignment is called.

    std::thread t3;
    t3 = std::move(t2);     //move assignment
    t2 = std::move(t3);     //move assignment
}
