//!
//! @author Yue wang
//! @date   24.11.2014
//!
//! @brief  Implementation for Listing 4.7 with a few insignicant modifications.
//!

#include <future>
#include "../include/utilities.hpp"

namespace para {

struct X
{
    int foo(int i, std::string const&)
    {
        para::println("X::foo is working");
        return i;
    }

    std::string bar(std::string const& str)
    {
        para::println("X::bar is working");
        return str;
    }
};

template<typename T>
struct Y
{
     T operator ()(T val)
     {
        para::println("Y::operator () is working");
        return val;
     }
};

struct MoveOnly
{
    MoveOnly() = default;
    MoveOnly(MoveOnly &&){}
    MoveOnly(MoveOnly const& ) = delete;
    MoveOnly& operator =(MoveOnly &&) {return *this;}
    MoveOnly& operator =(MoveOnly const&) = delete;

    template<typename T>
    T operator ()(T val)
    {
        para::println("MoveOnly::operator () is working");
        return val;
    }
};
}//namespace

int main()
{
    para::X x;
    std::future<int> test_foo = std::async(&para::X::foo,&x,42,"");
    para::println(test_foo.get())       << "\n";
    std::future<std::string> test_bar = std::async(&para::X::bar,x,"hello");
    para::println(test_bar.get())       << "\n";

    para::Y<double> y;
    auto test_move_ctor =   std::async(para::Y<double>(), 3.14);
    para::println(test_move_ctor.get()) << "\n";
    auto test_ref       =   std::async(std::ref(y),1.11);
    para::println(test_ref.get())       << "\n";

    auto test_move_only =   std::async(para::MoveOnly{},42);
    para::println(test_move_only.get()) << "\n";

    return 0;
}
//! output
//!
//X::foo is working
//42

//X::bar is working
//hello

//Y::operator () is working
//3.14

//Y::operator () is working
//1.11

//MoveOnly::operator () is working
//42
