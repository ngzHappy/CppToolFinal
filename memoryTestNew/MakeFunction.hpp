#ifndef _m_MAKEFUNCTION_HPP_CPP_
#define _m_MAKEFUNCTION_HPP_CPP_() 1

#include "Memory.hpp"
#include "Allocator.hpp"
#include <functional>

namespace memory {

template<typename _FunctionType_,
    typename ... _Args_>
    std::function<_FunctionType_> make_function(_Args_&&..._m_args_) {
    return std::function<_FunctionType_>(
        std::allocator_arg,
        memory::Allocator< std::function<_FunctionType_> >{},
        std::forward<_Args_>(_m_args_)...);
}


}/*memory*/


#endif

