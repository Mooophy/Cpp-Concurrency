#include "threadsafe_stack.hpp"

int main()
{
    para::ThreadsafeStack<int> stk;
    auto stk2{stk};
}
