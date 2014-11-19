#include <iostream>
#include "threadsafe_stack.hpp"
#include <thread>

/**
 * @brief test for para::EmptyStackException
 *
 * @output
 *          para's exception was thrown
 */
void test_para_excetion() throw()
{
    try{
        throw para::EmptyStackException{"para's exception was thrown\n"};
    }
    catch (const para::EmptyStackException& e)
    {
        std::cerr << e.what();
    }
}

int main()
{
    para::ThreadsafeStack<int> stk;
    auto init = [&]{
        for(auto&& i : {1,2,3,4,5,6,7,8})   stk.push(i);
    };

    std::thread init_stk
    {
        [&]{
            init();
        }
    };

    std::thread pop_with_pointer_n_print
    {
        [&]{
            while(!stk.empty())
                std::cout << *stk.pop() << " " << std::flush;
        }
    };

    std::thread pop_with_value_n_print
    {
        [&]{
            int buff=0;
            while(!stk.empty())
            {
                stk.pop(buff);
                std::cout << buff << " " << std::flush;
            }
        }
    };

    init_stk.join();
    pop_with_pointer_n_print.join();
    pop_with_value_n_print.join();

    return 0;
}
//! output
//! since the three threads have not been synchronized,
//! multiple types of outputs are expected.
//!
