//!
//! @author Yue Wang
//! @date   25.11.2014
//!
//! @brief  Implementation for Listing 4.9, with modifications :
//!             using std::bind for arguments binding rather than template specialization
//!

#include <future>
#include <deque>
#include <mutex>
#include <thread>
#include <utility>
#include <iostream>

std::mutex mutex;
std::deque<std::packaged_task<void()> > task_queue;

void f(std::string input)
{
    std::cout << "a task was passed from ui : " << input << std::endl;
}

int main()
{
    std::thread ui{
        []{
            std::cout << ">pls enter something:" << std::endl;
            for(std::string buff; std::cin >> buff; )
            {
                std::packaged_task<void()> task(std::bind(f, buff));
                std::lock_guard<std::mutex> lock{mutex};
                task_queue.push_back(std::move(task));
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        }
    };

    std::thread background{
        []{
            for(;;) {
                std::packaged_task<void()> t;

                {
                    std::lock_guard<std::mutex> lock{mutex};
                    if(task_queue.empty())  continue;   //<--could jump over t();
                    t = std::move(task_queue.front());
                    task_queue.pop_front();
                }

                t();
            }
        }
    };

    ui.join();
    background.join();

    return 0;
}
