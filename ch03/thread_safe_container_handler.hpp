//!
//! @author Yue Wang
//! @date   18.11.2014
//!
//!  Inspired by Listing 3.1, this code implemented:
//!      A thread-safe container handler that supports generics and move semantics.
//!
#ifndef THREAD_SAFE_CONTAINER_HANDLER_HPP
#define THREAD_SAFE_CONTAINER_HANDLER_HPP

#include <iostream>
#include <list>
#include <mutex>
#include <algorithm>

namespace para {

/**
 * @brief       A thread safe container handler.
 *
 * @featurs     generic, thread safe , movable and uncopyable
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

    //! deleted
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
        return std::find(cbegin(), cend(), val_to_find) != cend();
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

    ConstIter cbegin() const
    {
        return container_.cbegin();
    }

    ConstIter cend() const
    {
        return container_.cend();
    }
};

}//namespace

#endif // THREAD_SAFE_CONTAINER_HANDLER_HPP
