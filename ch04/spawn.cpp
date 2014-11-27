//!
//! @author Yue Wang
//! @date   25.11.2014
//!
//! @brief  Implementation and bugs fixing for Listing 4.14.
//!

#include <future>
#include "../include/utilities.hpp"

namespace para {

template<typename F, typename A>
using FutureResult = typename std::future< typename std::result_of<F(A&&)>::type>;

/**
 * @brief   spawn_task, implmenting Listing 4.14
 * @param   callable
 * @param   arguments
 * @return  future
 *
 * @bug     There is a bug in Listing 4.14 : two `typename`s have been omitted.
 *          The code has fixed it.
 *
 * @modifications
 *          using alias for better readablity.
 */
template<typename F, typename A>
para::FutureResult<F,A> spawn(F&& f,A&& a)
{
    using std::thread;
    using std::move;

    using ResultType = typename std::result_of<F(A&&)>::type;
    using Package    =          std::packaged_task<ResultType(A&&)>;
    using Future     =          std::future<ResultType>;

    Package     task{std::move(f)};
    Future      ret{task.get_future()};
    thread      t{move(task),move(a)};
    t.detach();
    return ret;
}
}//namespace

int main()
{
    auto func = [](int i){return i * i;};
    auto test = para::spawn(func, -42);

    para::println(test.get());
    return 0;
}
//! output
//!
//para>1764
