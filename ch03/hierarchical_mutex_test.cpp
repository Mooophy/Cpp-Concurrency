#include "hierarchical_mutex.hpp"
#include "../include/utilities.hpp"

para::HierachicalMutex high_level_mutex{10000};
para::HierachicalMutex low_level_mutex{5000};

int do_low_level_stuff()
{
    para::println("working on low level stuff");
    return 0;
}

int low_level_func()
{
    return do_low_level_stuff();
}

void high_level_stuff(int some_param)
{
    std::string msg = "working on high level stuff " + std::to_string(some_param);
    para::println(msg);
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

int main()
{

}
