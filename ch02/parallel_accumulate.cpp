//!
//! @author Yue Wang
//! @date   18.11.2014
//!
//! Disscussion on SO:
//! http://stackoverflow.com/questions/26971625/whats-the-difference-between-functor-and-lambda-when-used-for-constructing-std
//!

#include <thread>
#include <algorithm>
#include <vector>
#include <iostream>

namespace para {

/**
 * @brief parallel_accumulate
 *
 * @note    based on Listing 2.8,   modifications :
 *          1. using Lambda instead of functor
 *          2. using std::size_t instead of const unigned long
 *          3. using for range instead of std::for_each to join all threads
 *
 * @note    On my laptop (2 cores 4 threads) parallel_accumulate() is faster than
 *          std::accumulate() by around 45%.
 *
 */
template<typename Iter, typename Value>
Value parallel_accumulate(Iter first, Iter last, Value init_val)
{
    using std::size_t;
    size_t length = std::distance(first, last);

    //! trivial case
    if(length == 0) return init_val;

    //! determine number of threads
    size_t min_per_thread = 25;
    size_t max_threads = (length + min_per_thread - 1) / min_per_thread;
    size_t hardware_threads = std::thread::hardware_concurrency();
    size_t num_threads = std::min((hardware_threads!=0 ? hardware_threads : 2), max_threads);
    size_t block_size = length/num_threads;
    std::vector<Value> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);

    //! construct threads vector
    Iter block_start = first;
    for(size_t idx=0; idx != (num_threads-1);  ++idx )
    {
        Iter block_end = block_start;
        std::advance(block_end, block_size);

        //! @brief  using lambda to construct the threads vector
        //! @attention  block_start and block_end must be captured by reference.
        //!             Otherwise all threads will reference the same objects, which is an
        //!             undefined behaviour.
        threads[idx] = std::thread
        {
            [=,&results]{
                results[idx] = std::accumulate(block_start, block_end, results[idx]);
            }
        };

        block_start = block_end;
    }
    results.back() = std::accumulate(block_start, last, results.back());

    //! join all threads and return result
    for(auto& t : threads)  t.join();
    return std::accumulate(results.begin(), results.end(), init_val);
}
}//namespace

int main()
{
    std::vector<int> v(500000000,1);
    auto sum = para::parallel_accumulate(v.begin(), v.end(), 0);
    std::cout << "sum = " << sum << std::endl;

    return 0;
}
//! output :
//!
//sum = 500000000
