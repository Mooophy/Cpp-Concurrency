#include "using_stdlock_to_avoid_deadlock.hpp"

int main()
{
    //! test for swap para::SomethingBig<T>
    para::SomethingBig<int> lhs{std::make_shared<int>(42)};
    para::SomethingBig<int> rhs{std::make_shared<int>(43)};

    std::cout << *lhs.data_ << std::endl;
    std::cout << *rhs.data_ << std::endl;
    para::swap(lhs,rhs);
    std::cout << *lhs.data_ << std::endl;
    std::cout << *rhs.data_ << std::endl;
}
