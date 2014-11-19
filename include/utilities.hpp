//!
//! @author Yue Wang
//! @date   19.11.2014
//!

#ifndef UTILITTIES_hpp
#define UTILITTIES_hpp

#include <iostream>

namespace para{


template<typename Printable>
inline std::ostream& println(Printable const& to_print)
{
    return std::cout << to_print << std::endl;
}

template<typename Printable>
inline std::ostream& println(Printable && to_print)
{
    return std::cout << std::move(to_print) << std::endl;
}

}//namespace
#endif 
