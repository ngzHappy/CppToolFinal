#ifndef __MALLOC_FREE_THISTYPE_PRAGMA_ONCE_0x0001
#define __MALLOC_FREE_THISTYPE_PRAGMA_ONCE_0x0001

#include<cstdint>
#include<cstddef>
#include<cinttypes>/*at least c++14*/

#ifndef _EXPORT_MALLOC_FREE_IMPORT
#define _EXPORT_MALLOC_FREE_IMPORT
#endif

namespace memory {

_EXPORT_MALLOC_FREE_IMPORT void *malloc(std::size_t);
_EXPORT_MALLOC_FREE_IMPORT void free(void*);

}

#endif
