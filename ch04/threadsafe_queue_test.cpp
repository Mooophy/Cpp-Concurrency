//!
//! @author Yue Wang
//! @date   21.11.2014
//! 
//! @brief  This code tests para::ThreadsafeQueue with two threads.
//! 

#include <thread>
#include "threadsafe_queue.hpp"
#include "../include/utilities.hpp"

int main()
{
    //! construct
    para::ThreadsafeQueue<int> q;

    //! thread 1
    std::thread data_preparing{
        [&]{
            for(int count = 0; count < 3; ++count)
            {
                q.push(count);
                para::println("pushed " + std::to_string(count));
            }
        }
    };

    //! thread 2
    std::thread data_processing{
        [&]{
            for(int count = 0; count < 3; ++count)
                para::println("popped "  + std::to_string(*q.wait_and_pop()));
        }
    };

    //! join all
    data_preparing.join();
    data_processing.join();

    return 0;
}
//! @output
//! @note   Since ostream is not locked, so multiple output forms are expected.
//!         The below is one of them
//!
//pushed 0
//pushed 1
//pushed 2
//popped 0
//popped 1
//popped 2
