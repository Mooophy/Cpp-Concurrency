//!
//! @author Yue Wang
//! @date   19.11.2014
//!
//! test for Listing 3.6
//!

#include"../include/utilities.hpp"
#include "using_stdlock_to_avoid_deadlock.hpp"

int main()
{
    //! test for swap para::SomethingBig<T>
    para::SomethingBig<int> lhs{std::make_shared<int>(42)};
    para::SomethingBig<int> rhs{std::make_shared<int>(43)};

    std::cout << "lhs : " << lhs << std::endl;
    std::cout << "rhs : " << rhs << std::endl;

    para::swap(lhs,rhs);

    std::cout << "lhs : " << lhs << std::endl;
    std::cout << "rhs : " << rhs << std::endl;

    //! test for swap para::WrapperForSomethingBig<T>
    para::WrapperForSomethingBig<int> wrapper_for_lhs{lhs};
    para::WrapperForSomethingBig<int> wrapper_for_rhs{rhs};

    std::cout << "wrapper_for_lhs : " << wrapper_for_lhs << std::endl;
    std::cout << "wrapper_for_rhs : " << wrapper_for_rhs << std::endl;

    swap(wrapper_for_lhs,wrapper_for_rhs);

    std::cout << "wrapper_for_lhs : " << wrapper_for_lhs << std::endl;
    std::cout << "wrapper_for_rhs : " << wrapper_for_rhs << std::endl;
}
//! output
//!
//lhs : 42
//rhs : 43
//lhs : 43
//rhs : 42
//wrapper_for_lhs : 43
//wrapper_for_rhs : 42
//wrapper_for_lhs : 42
//wrapper_for_rhs : 43
