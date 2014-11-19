//!
//! @author Yue Wang
//! @date   19.11.2014
//!
//! Implementation for Listing 3.6
//!

#ifndef USING_STDLOCK_TO_AVOID_DEADLOCK_HPP
#define USING_STDLOCK_TO_AVOID_DEADLOCK_HPP

#include <memory>
#include <mutex>
#include <iostream>

namespace para{

/**
 * @brief The SomethingBig struct
 *
 * Generic implementation for the class some_big_object in Listing 3.6.
 */
template<typename Data>
struct SomethingBig
{
    friend std::ostream&
    operator <<(std::ostream& os, SomethingBig const& rhs)
    {
        return  os << *rhs.data_;
    }

    std::shared_ptr<Data> data_;
};

/**
 * @brief swap
 *
 * Generic implementation for swap in Listing 3.6
 */
template<typename T>
void swap(SomethingBig<T>&lhs, SomethingBig<T>& rhs)
{
    if(lhs.data_ == rhs.data_)   return;

    auto tmp = rhs.data_;
    rhs.data_ = lhs.data_;
    lhs.data_ = tmp;
}

/**
 * @brief The WrapperForSomethingBig class
 *
 * Generic implementation for the class X in Listing 3.6
 */
template<typename T>
class WrapperForSomethingBig
{
    friend void
    swap(WrapperForSomethingBig &lhs, WrapperForSomethingBig &rhs)
    {
        if(&lhs == &rhs)    return;

        using std::swap;
        std::lock(lhs.mutex_,rhs.mutex_);
        std::lock_guard<std::mutex> lock_l(lhs.mutex_, std::adopt_lock);
        std::lock_guard<std::mutex> lock_r(rhs.mutex_, std::adopt_lock);
        swap(lhs.detail_,rhs.detail_);
    }

    friend std::ostream&
    operator <<(std::ostream& os, const WrapperForSomethingBig& rhs)
    {
        return os << rhs.detail_;
    }

public:
    explicit WrapperForSomethingBig(const SomethingBig<T>& sth):
        detail_(sth)//<-- a shallow copy
    {}

private:
    SomethingBig<T> detail_;
    mutable std::mutex  mutex_;
};
}//namespace

#endif // USING_STDLOCK_TO_AVOID_DEADLOCK_HPP
