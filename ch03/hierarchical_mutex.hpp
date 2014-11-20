//!
//! @author Yue Wang
//! @date   20.11.2014
//!
//! @brief  Implementation for para::HierachicalMutex.
//!
//!         As well as an example for a user-defined mutex class that satisfies
//!         the mutex concept :
//!             lock()      unlock()    try_lock()
//!
#ifndef HIERARCHICAL_MUTEX__HPP
#define HIERARCHICAL_MUTEX__HPP

#include <mutex>
#include <stdexcept>
#include <climits>

namespace para{

/**
 * @brief The HierachicalMutex class
 *
 * meets mutex concept :
 *      lock()      unlock()    try_lock()
 */
class HierachicalMutex
{
public:
    using Size_t = unsigned long;

    explicit HierachicalMutex(Size_t val):
        hierachy_value_{val},
        previous_hierachy_value_{0}
    {}

    void lock()
    {
        check_for_hierarchy_violation();
        internal_mutex_.lock();
        update_hierarchy_value();
    }

    void unlock()
    {
        this_thread_hierachy_value_ =   previous_hierachy_value_;
        internal_mutex_.unlock();
    }

    bool try_lock()
    {
        check_for_hierarchy_violation();
        if(!internal_mutex_.try_lock())     return false;
        update_hierarchy_value();
        return true;
    }

private:
    std::mutex internal_mutex_;
    Size_t const hierachy_value_;
    Size_t previous_hierachy_value_;
    static thread_local Size_t this_thread_hierachy_value_;

    void check_for_hierarchy_violation()const
    {
        if(this_thread_hierachy_value_ <= hierachy_value_)
            throw std::logic_error{"mutex hierarchy violated"};
    }

    void update_hierarchy_value()
    {
        previous_hierachy_value_    =   this_thread_hierachy_value_;
        this_thread_hierachy_value_ =   hierachy_value_;
    }

};

thread_local HierachicalMutex::Size_t
    HierachicalMutex::this_thread_hierachy_value_{ULONG_MAX};
}//namespace
#endif // HIERARCHICAL_MUTEX__HPP
