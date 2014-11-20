//!
//! @author Yue Wang
//! @date   20.11.2014
//!
//!
//! @brief  this code shows how to use std::condition_variable to transfer messages
//!         between two threads as well as an implementation for Listing 4.1
//!

#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>
#include <chrono>
#include "../include/utilities.hpp"


namespace para{

const unsigned NUM_OF_DATA = 4;
struct DataChunk{};

DataChunk prepare_data()
{
    return para::DataChunk{};
}

bool is_more_data()
{
    static unsigned count = 0;
    return count++ < NUM_OF_DATA;
}

void process(DataChunk)
{
    para::println("one data has been processed");
}

bool is_last_data(DataChunk)
{
    static unsigned count = 0;
    return count++ >= NUM_OF_DATA - 1;
}

void sleep_for(unsigned long count)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(count));
}
}//namespace

std::mutex mutex;
std::queue<para::DataChunk> queue;
std::condition_variable condition;

int main()
{
    //! thread 1
    std::thread data_preparation{
        []{
            for(    ;para::is_more_data();  para::sleep_for(1)){
                auto const data = para::prepare_data();
                {
                    std::lock_guard<std::mutex> lock{mutex};
                    queue.push(data);
                    condition.notify_one();
                }
                para::println("one data has been pushed");
            }
        }
    };

    //! thread 2
    std::thread data_processing{
        []{
            for(;;){
                std::unique_lock<std::mutex> lock{mutex};
                condition.wait(lock,[]{return !queue.empty();});
                auto data = queue.front();
                queue.pop();
                lock.unlock();
                para::process(data);

                if(para::is_last_data(data)) break;
            }
        }
    };

    data_preparation.join();
    data_processing.join();

    para::println("exit");
    return 0;
}
//! @output
//! @note   since iosream hasn't been synchronized,
//!         multiple different forms of output should be expected.
//!
//one data has been pushed
//one data has been processed
//one data has been pushed
//one data has been processed
//one data has been pushed
//one data has been processed
//one data has been pushed
//one data has been processed
//exit
