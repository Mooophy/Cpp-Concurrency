//!
//! @author Yue Wang
//! @date   18.11.2014
//!
//!  Inspired by Listing 3.1, this code implemented:
//!      A generics, thread safe , moveable and uncopyable container handler.
//!

#include <iostream>
#include <list>
#include <mutex>
#include <algorithm>

namespace para {

/**
 * @brief       A thread safe container handler.
 *
 * @featurs     generics, thread safe , moveable and uncopyable
 */
template<typename Container>
class ContainerHandler
{
public:
    using Value     =   typename    Container::value_type;
    using Lock      =               std::lock_guard<std::mutex>;
    using ConstIter =   typename    Container::const_iterator;

    //!Ctor
    explicit ContainerHandler(Container& c):
        container_(c),
        mutex_{}
    {}

    //! move Ctor
    ContainerHandler(ContainerHandler&& ch)noexcept:
        container_(ch.container_),
        mutex_{}
    {}

    //! move assignment
    ContainerHandler& operator =(ContainerHandler&& rhs)noexcept
    {
        return *this = std::move(rhs);
    }

    ContainerHandler()                                      =   delete;
    ContainerHandler(const ContainerHandler&)               =   delete;
    ContainerHandler& operator =(const ContainerHandler&)   =   delete;

    /**
     * @brief add
     *
     * @feature thread safe
     */
    void add (const Value& new_val)
    {
        Lock lock{mutex_};
        container_.push_back(new_val);
    }

    /**
     * @brief if_contains
     *
     * @feature thread safe
     */
    bool if_contains(const Value& val_to_find)
    {
        Lock lock{mutex_};
        return std::find(begin(), end(), val_to_find) != end();
    }

    /**
     * @brief print
     *
     * @feature thread safe
     */
    std::ostream& print()
    {
        Lock lock{mutex_};
        for(const auto& elem : container_)  std::cout << elem << " ";
        return std::cout;
    }

private:
    Container& container_;
    std::mutex mutex_;

    ConstIter begin() const
    {
        return container_.cbegin();
    }

    ConstIter end() const
    {
        return container_.cend();
    }
};

}//namespace


int main()
{
    /**
     * @brief test for move semantics
     */
    std::list<int> list(3,42);
    para::ContainerHandler<std::list<int>> handler(list);
    handler.print()     <<  std::endl;

    auto handler2 (std::move(handler));
    handler2.print()    <<  std::endl;

    auto handler3 = std::move(handler2);
    handler3.print()    <<  std::endl;

    /**
     * @brief test for members
     */
    handler3.add(999);
    std::cout << (handler3.if_contains(999)? "found\n" : "no such thing\n");

    return 0;
}
//! output
//!
//42 42 42
//42 42 42
//42 42 42
//found

