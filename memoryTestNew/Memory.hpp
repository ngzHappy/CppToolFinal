#ifndef _m_NEW_MEMORY_HPP_CPP_
#define _m_NEW_MEMORY_HPP_CPP_() 1

#include <memory>
#include <utility>
#include <cstddef>
#include <cstdint>
#include <cinttypes>
#include <type_traits>

#ifndef _EXPORT_MALLOC_FREE_IMPORT
#define _EXPORT_MALLOC_FREE_IMPORT
#endif

namespace memory {

_EXPORT_MALLOC_FREE_IMPORT void * malloc(std::size_t);
_EXPORT_MALLOC_FREE_IMPORT void free(void *);
_EXPORT_MALLOC_FREE_IMPORT void freePoolMemory();

}

#endif

/*
*/

