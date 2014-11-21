#ifndef THREADSAFE_QUEUE_HPP
#define THREADSAFE_QUEUE_HPP

#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace para {

template<typename T>
class ThreadsafeQueue
{
public:
    using Guard         =   std::lock_guard<std::mutex>;
    using Lock          =   std::unique_lock<std::mutex>;
    using SharedPointer =   std::shared_ptr<T>;

    ThreadsafeQueue() = default;
    ThreadsafeQueue(ThreadsafeQueue const& other)
    {
        Guard g{other.mutex_};
        q_ = other.q_;
    }
    ThreadsafeQueue(ThreadsafeQueue && other)noexcept
    {
        //! @attention
        //! no lock here, see the discussion on SO:
        //! http://stackoverflow.com/questions/27071874/lock-an-rvalue-argument-for-thread-safe-queues-move-constructor/27072022#27072022
        q_ = std::move(other.q_);
    }

    void push(const T& new_val)
    {
        Guard g{mutex_};
        q_.push(new_val);
        cond_.notify_one();
    }

    void wait_and_pop(T& val)
    {
        Lock l{mutex_};
        cond_.wait(l,   [this]{return !q_.empty();});
        val = q_.front();
        q_.pop();
    }

    SharedPointer wait_and_pop()
    {
        Lock l{mutex_};
        cond_.wait(l,   [this]{return !q_.empty();});
        SharedPointer ret{std::make_shared<T>(q_.front())};
        q_.pop();

        return ret;
    }

    bool try_pop(T& val)
    {
        Lock l{mutex_};
        if(q_.empty())  return false;
        val = q_.front();
        q_.pop();

        return true;
    }

    SharedPointer try_pop()
    {
        Lock l{mutex_};
        if(q_.empty())  return std::make_shared<T>();
        auto ret = std::make_shared<T>(q_.front());
        q_.pop();

        return ret;
    }

    bool empty()const
    {
        Lock l{mutex_};
        return q_.empty();
    }

private:
    std::queue<T> q_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
};

}//namespace


#endif // THREADSAFE_QUEUE_HPP
