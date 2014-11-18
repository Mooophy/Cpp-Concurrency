#ifndef THREADSAFE_STACK_HPP
#define THREADSAFE_STACK_HPP

#include <exception>
#include <memory>
#include <mutex>
#include <stack>

namespace para {

/**
 * @brief EmptyStack
 *
 * inherited from std::exception
 */
struct EmptyStack : std::exception
{
    const char* what () const throw();
};

template<typename Value>
class ThreadsafeStack
{
    using Guard = std::lock_guard<std::mutex>;
public:
    //! defautl Ctor
    ThreadsafeStack(){}

    //! copy Ctor
    ThreadsafeStack(const ThreadsafeStack& other)
    {
        Guard g{other.mutex_};
        data_ = other.data_;
    }

    ThreadsafeStack& operator = (const ThreadsafeStack&) = delete;

    void push(Value new_val);
    std::shared_ptr<Value> pop();
    void pop(Value& val);
    bool empty()const;
private:
    std::stack<Value> data_;
    mutable std::mutex mutex_;

};
}//namespace
#endif // THREADSAFE_STACK_HPP
