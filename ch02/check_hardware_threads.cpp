#include <thread>
#include <iostream>

int main()
{
    std::cout   <<  "hardware threads supported: \n"
                <<  std::thread::hardware_concurrency() << std::endl;
    return 0;
}
//! output  (on a 2 cores 4 threads laptop)
//!
//hardware threads supported:
//4

