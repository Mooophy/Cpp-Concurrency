#include <iostream>
#include "threadsafe_stack.hpp"

/**
 * @brief test for para::EmptyStackException
 *
 * @output para's exception was thrown
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

}
