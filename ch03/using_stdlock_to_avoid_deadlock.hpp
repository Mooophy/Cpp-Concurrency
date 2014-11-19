#ifndef USING_STDLOCK_TO_AVOID_DEADLOCK_HPP
#define USING_STDLOCK_TO_AVOID_DEADLOCK_HPP

#include <memory>
#include <mutex>
#include <iostream>

namespace para{

template<typename Data>
struct SomethingBig
{
    std::shared_ptr<Data> data_;
};

template<typename T>
void swap(SomethingBig<T>&lhs, SomethingBig<T>& rhs)
{
    if(lhs.data_ == rhs.data_)   return;

    auto tmp = rhs.data_;
    rhs.data_ = lhs.data_;
    lhs.data_ = tmp;

    std::cout << "\nnot from std::swap\n" << std::endl;
    return;
}

template<typename T>
class WrapperForSomethingBig
{
public:
    friend void swap(WrapperForSomethingBig &lhs, WrapperForSomethingBig &rhs)
    {
        if(&lhs == &rhs)    return;

        using std::swap;
        std::lock(lhs.data_,rhs.data_);
        std::lock_guard<std::mutex> lock_l(lhs.mutex_, std::adopt_lock);
        std::lock_guard<std::mutex> lock_r(rhs.mutex_, std::adopt_lock);
        swap(lhs.detail_,rhs.detail_);
    }

    explicit WrapperForSomethingBig(const SomethingBig<T>& sth):
        detail_{sth}
    {}

private:
    SomethingBig<T> detail_;
    std::mutex  mutex_;
};
}//namespace

#endif // USING_STDLOCK_TO_AVOID_DEADLOCK_HPP
