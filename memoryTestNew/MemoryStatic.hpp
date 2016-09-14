#ifndef _m_MEMORY_STATIC_HPP_CPP_
#define _m_MEMORY_STATIC_HPP_CPP_() 1

#include "Memory.hpp"
#include "Allocator.hpp"

namespace memory {

_EXPORT_MALLOC_FREE_IMPORT bool __is_construct_static();
_EXPORT_MALLOC_FREE_IMPORT void __set_construct_static();

/*
the function just call once
the function must be called before any static/thread_local data not pod
*/
inline void constructStatic() {
    if (__is_construct_static()) { return; }
    static const bool _runonce_flag=[]() {
        /*set your code here***********************/
        /******************************************/
        __set_construct_static();
        return __is_construct_static();
    }();
}

}/*~memory*/

#endif



