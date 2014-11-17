//!
//! @author Yue Wang
//! @date   17 11 2014
//!
//! Based one the code in Listing 2.6 along with several significant differences :
//! 1.  Rvalue reference parameter is explicitly specified to prevent ambiguity.
//! 2.  Using Lambda for testing.
//! 3.  A one-micorsecond delay was added to for enough time taken for new thread setup.
//!

#include <thread>
#include <stdexcept>
#include <iostream>
#include <chrono>


namespace para{

/**
 * @brief       Ensuring life time of thread is within the scope
 *              by using RAII and move semantics.
 *
 * @note        based on Listing 2.6
 * @attention   Rvalue reference parameter is explicitly specified
 *              for avoiding Ctors matching ambiguity
 */
class ScopedThread
{
    std::thread t_;
public:
    explicit ScopedThread(std::thread&& thread):
        t_{std::move(thread)}     //!^^<---@attention required to prevent ambiguity error
    {
        if(!t_.joinable())  throw std::logic_error{"No thread!"};
    }

    ScopedThread(const std::thread& thread)         = delete;
    ScopedThread& operator=(const std::thread& rhs) = delete;

    ~ScopedThread()
    {
        t_.join();
        std::cout << "t_ has been join()\n";
    }
};
}//namespace

int main()
{
    int local_object = 0;

    //! construct ScopedThread t with lambda to capture local object.
    para::ScopedThread t
    {
        std::thread
        {
            [&]{
                local_object = 42;
                std::cout << "from thread t : " << local_object << std::endl;
            }
        }
    };

    //! @attention added a one microsec delay
    //!     this is for thread initilization, like stack setup etc.
    //!     this delay may be deleted to see the different output.
    //!     probably this is platform dependent.
    std::this_thread::sleep_for(std::chrono::microseconds{1});
    std::cout << "from thread main :" << local_object << std::endl;
}

//! output with delay:
//!
//from thread t : 42
//from thread main :42
//t_ has been join()
//!
//! output without delay:
//!
//from thread main :0
//from thread t : 42
//t_ has been join()

//!
//! @system info :
//!         cpu : Intel(R) Core(TM) i5-2410M CPU @ 2.30GHz
//!         Ubuntu 14.04 LTS
//!
