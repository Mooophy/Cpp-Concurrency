//!
//! @author Yue Wang
//! @date   21.11.2014
//!
//! @brief  implementation for a non thread-safe queue, similar to Gcc's std::queue.
//!
//! @note   swap and other operators overloading are not implemented for simplicity.
//!

#ifndef ORDINARY_QUEUE_HPP
#define ORDINARY_QUEUE_HPP

#include <deque>

namespace para {

/**
 * @brief The Queue class template
 *
 * Non thread safe queue class template, similar to Gcc's implementation.
 */
template<typename T, typename Sequence = std::deque<T> >
class Queue
{
public:
    using Value             =   typename    Sequence::value_type;
    using Reference         =   typename    Sequence::reference;
    using ConstReference    =   typename    Sequence::const_reference;
    using Size              =   typename    Sequence::size_type;

    /**
     * @brief Ctor
     */
    explicit Queue(const Sequence& seq):
        c_(seq)
    {}

    /**
     * @brief Ctor by moving
     */
    explicit Queue(Sequence&& seq = Sequence{}):
        c_(seq)
    {}

    /**
     * @brief empty
     */
    bool empty()const
    {
        return c_.empty();
    }

    /**
     * @brief size
     */
    Size size()const
    {
        return c_.size();
    }

    /**
     * @brief front
     * @return read and write reference to the first elemnt.
     */
    Reference front()
    {
        return c_.front();
    }

    /**
     * @brief front
     * @return read only reference to the first elemnt.
     */
    ConstReference front() const
    {
        return c_.front();
    }

    /**
     * @brief back
     * @return read and write reference to the last elemnt.
     */
    Reference back()
    {
        return c_.back();
    }

    /**
     * @brief back
     * @return read only reference to the last elemnt.
     */
    ConstReference back()const
    {
        return c_.back();
    }

    /**
     * @brief push
     * @param val
     */
    void push(Value const& val)
    {
        c_.push_back(val);
    }

    /**
     * @brief push
     * @param val   by moving
     */
    void push(Value && val)
    {
        c_.push_back(std::move(val));
    }

    /**
     * @brief emplace
     * @param val   by constructing using metaprogramming
     */
    template<typename... Args>
    void emplace(Args&&... args)
    {
        c_.emplace_back(std::forward<Args>(args)...);
    }

    /**
     * @brief pop
     */
    void pop()
    {
        c_.pop_front();
    }

private:
    Sequence c_;
};

}//namespace
#endif // COMMON_QUEUE_HPP
