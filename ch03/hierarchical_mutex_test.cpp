//!
//! @author Yue Wang
//! @date   20.11.2014
//!
//! @brief  This code shows how to use hierachical mutex to prevent deadlock as well as
//!         testing para::HierachicalMutex.
//!
//! @note   This code implemented Listing 3.7, with modifications :
//!             void thread_a()     ->      std::thread thread_fine
//!             void thread_b()     ->      std::thread thread_wild
//!

#include <thread>
#include "hierarchical_mutex.hpp"
#include "../include/utilities.hpp"

para::HierachicalMutex high_level_mutex{10000};
para::HierachicalMutex low_level_mutex{5000};

int do_low_level_stuff()
{
    para::println("working on low level stuff");
    return 42;
}

int low_level_func()
{
    return do_low_level_stuff();
}

void high_level_stuff(int some_param)
{
    para::println("doing high level stuff " + std::to_string(some_param));
}

void high_level_func()
{
    std::lock_guard<para::HierachicalMutex> lock{high_level_mutex};
    high_level_stuff(low_level_func());
}

para::HierachicalMutex other_mutex{100};

void do_other_stuff()
{
    para::println("doing other stuff");
}

void other_stuff()
{
    high_level_func();
    do_other_stuff();
}

int main()
{
    //! thread that abides by the rules
    std::thread thread_fine{
        []{
            high_level_func();
        }
    };

    //! thread that disregards the rules
    //! As a result, an exception's going to be thrown
    std::thread thread_wild{
        []{
            std::lock_guard<para::HierachicalMutex> lock{other_mutex};
            other_stuff();
        }
    };

    thread_fine.join();
    thread_wild.join();
}
//! output
//!
//working on low level stuff
//doing high level stuff 42
//terminate called after throwing an instance of 'std::logic_error'
//  what():  mutex hierarchy violated
