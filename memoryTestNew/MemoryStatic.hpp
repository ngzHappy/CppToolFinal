#ifndef _m_MEMORY_STATIC_HPP_CPP_
#define _m_MEMORY_STATIC_HPP_CPP_() 1

#include "Memory.hpp"
#include "Allocator.hpp"

namespace memory {

/*
the function just call once
the function must be called before any static/thread_local data not pod
*/
inline void constructStatic() {
    if (__memory__construct_static::__is_construct_static()) { return; }
    __memory__construct_static::__run_once([]() {
        /*set your code here***********************/
        /******************************************/
        __memory__construct_static::__set_construct_static();
    });
    return;
}

}/*~memory*/

#endif



