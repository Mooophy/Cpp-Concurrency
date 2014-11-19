//!
//! @author Yue Wang
//! @date   19.11.2014
//!

#ifndef THREADSAFE_STACK_HPP
#define THREADSAFE_STACK_HPP

#include <exception>
#include <memory>
#include <mutex>
#include <stack>

namespace para {

/**
 * @brief   EmptyStack
 *          inherited from std::exception
 *
 * @modifications
 *          Listing 3.4 provides declaration only, here's my implementation.
 */
class EmptyStackException : std::exception
{
public:
    EmptyStackException():
        msg_{"para exception : empty stack"}
    {}

    explicit EmptyStackException(const char* msg):
        msg_{msg}
    {}

    explicit EmptyStackException(const std::string& msg):
        msg_{msg.c_str()}
    {}

    explicit EmptyStackException(std::string&& msg)noexcept:
        msg_{std::move(msg).c_str()}
    {}

    /**
     * @brief what
     * @note        specifier `throw()` is extensively considered as uselessness and
     *              even a bad practise. Listing 3.4 using it ,however, here
     *              follows the its style.
     */
    virtual const char* what ()  const throw() override
    {
        return msg_.c_str();
    }

    ~EmptyStackException(){}

private:
    const std::string msg_;
};

template<typename Value>
class ThreadsafeStack
{
    using Guard         =   std::lock_guard<std::mutex>;
public:
    using SharedPointer =   std::shared_ptr<Value>;

    ThreadsafeStack(){}

    //! copy Ctor
    ThreadsafeStack(const ThreadsafeStack& other)
    {
        Guard g{other.mutex_};
        data_ = other.data_;
    }

    ThreadsafeStack& operator = (const ThreadsafeStack&) = delete;

    void push(Value new_val)
    {
        Guard g{mutex_};
        data_.push(new_val);
    }

    SharedPointer pop()
    {
        Guard g{mutex_};
        throw_exception_if_empty();
        const auto pointer = std::make_shared<Value>(data_.top());
        data_.pop();

        return pointer;
    }

    void pop(Value& dest)
    {
        Guard g{mutex_};
        throw_exception_if_empty();
        dest = data_.top();
        data_.pop();
    }

    bool empty()const
    {
        Guard g{mutex_};
        return data_.empty();
    }

private:
    std::stack<Value> data_;
    mutable std::mutex mutex_;

    void throw_exception_if_empty()
    {
        if(data_.empty())   throw para::EmptyStackException{};
    }

};
}//namespace
#endif // THREADSAFE_STACK_HPP
