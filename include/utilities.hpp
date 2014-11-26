//!
//! @author Yue Wang
//! @date   19.11.2014
//!

#ifndef UTILITTIES_hpp
#define UTILITTIES_hpp

#include <iostream>

namespace para{

template<typename Range>
inline std::ostream& println_range(Range const& rng)
{
    std::cout << "para> ";
    for(auto elem : rng)    std::cout << elem << " ";
    return std::cout << std::endl;
}

template<typename Printable>
inline std::ostream& println(Printable const& to_print)
{
    return std::cout << "para>" << to_print << std::endl;
}

template<typename Printable>
inline std::ostream& println(Printable && to_print)
{
    return std::cout << "para>" << std::move(to_print) << std::endl;
}

}//namespace
#endif 
