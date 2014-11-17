//!
//! @author Yue Wang
//! @date   18.11.2014
//!
//! Disscussion on SO:
//! http://stackoverflow.com/questions/26971625/whats-the-difference-between-functor-and-lambda-when-used-for-constructing-std
//!
//! Based on Listing 2.8, serveral differences.
//!

#include <thread>
#include <algorithm>
#include <vector>
#include <iostream>

namespace para {

template<typename Iter, typename Value>
Value parallel_accumulate(Iter first, Iter last, Value init_val)
{
    using std::size_t;
    size_t length = std::distance(first, last);
    if(length == 0) return init_val;    // trivial case

    size_t min_per_thread = 25;
    size_t max_threads = (length + min_per_thread - 1) / min_per_thread;
    size_t hardware_threads = std::thread::hardware_concurrency();
    size_t num_threads = std::min((hardware_threads!=0 ? hardware_threads : 2), max_threads);
    size_t block_size = length/num_threads;
    std::vector<Value> results(num_threads);
    std::vector<std::thread> threads{num_threads - 1};

    Iter block_start = first;
    for(unsigned long idx=0; idx!=(num_threads-1);  ++idx )
    {
        Iter block_end = block_start;
        std::advance(block_end, block_size);
        Value & result = results[idx];

        threads[idx] = std::thread
        {
            [=,&result]{
                result = std::accumulate(block_start, block_end, result);
            }
        };

        block_start = block_end;
    }
    results.back() = std::accumulate(block_start, last, results.back());

    for(auto& t : threads)  t.join();
    return std::accumulate(results.begin(), results.end(), init_val);
}
}//namespace

int main()
{
    std::vector<int> v(10000,1);
    auto sum = para::parallel_accumulate(v.begin(), v.end(), 0);
    std::cout << "sum = " << sum << std::endl;

    return 0;
}
