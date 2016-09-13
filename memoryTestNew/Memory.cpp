﻿#include "Memory.hpp"
#include <atomic>
#include <boost/pool/pool.hpp>

namespace {
namespace __cpp {
namespace __private {
namespace __memory {
namespace memory {

/*
(use -ftemplate-depth= to increase the maximum)
Qt:QMAKE_CXXFLAGS += -ftemplate-depth=4100
*/
constexpr std::size_t max_malloc_size=1024*4;
typedef std::make_index_sequence<max_malloc_size> index_sequence_type;
typedef void(*FreeFunctionType)(void *);
typedef void*(*PoolMallocFunctionType)(void);
typedef boost::pool<boost::default_user_allocator_malloc_free> pool_type;

class DataItemType {
public:
    FreeFunctionType freeFunction;
};

/*gen data space*/
static char pool_plain_data[sizeof(pool_type)*max_malloc_size];
static PoolMallocFunctionType pool_malloc_functions[1+max_malloc_size];

/*0->1*/
template<std::size_t N>
void * atPoolMalloc() {
    auto * pool=reinterpret_cast<pool_type*>(
                const_cast<char *>(pool_plain_data))+N;
    auto * ans=reinterpret_cast<DataItemType*>(pool->malloc());
    if (ans==nullptr) { return nullptr; }
    ans->freeFunction=[](void * arg) {
        auto * pool=reinterpret_cast<pool_type*>(
                    const_cast<char *>(pool_plain_data))+N;
        pool->free(arg);
    };
    return ++ans;
}

template<typename >
class ConstructPools {
public:
    static void construct() {}
};

template<std::size_t ... I >
class ConstructPools< std::index_sequence<I...> > {
public:
    static void construct() {
        auto *functionMalloc=
            const_cast<PoolMallocFunctionType*>(
                reinterpret_cast<PoolMallocFunctionType const *>(pool_malloc_functions));
        new (++functionMalloc) PoolMallocFunctionType[max_malloc_size]{
            &atPoolMalloc<I>...
        };
    }
};

/*the module is use for application so do not need destruct*/
/*
void destructPools(){
    char * tmp=const_cast<char *>(pool_plain_data);
    auto * begin=reinterpret_cast<pool_type*>(tmp);
    auto * end=begin+max_malloc_size;
    for(;begin!=end;++begin){
        begin->~pool();
    }
}
*/

void constructPools() {
    {/*初始化内存池*/
        auto * begin=reinterpret_cast<pool_type*>(
            const_cast<char *>(pool_plain_data));
        for (std::size_t i=0; i<max_malloc_size; ++i) {
            new(begin) pool_type{ sizeof(DataItemType)+1+i };
            ++begin;
        }
    }
    {/*初始化malloc函数*/
        ConstructPools< index_sequence_type >::construct();
    }
}

std::atomic_bool is_pool_constructed;
/**/
void * malloc(std::size_t arg) {
    if (arg<=0) { return nullptr; }
    if (arg>max_malloc_size) {
        auto * ans=reinterpret_cast<DataItemType*>(
                    std::malloc(sizeof(DataItemType)+arg));
        if (ans==nullptr) { return nullptr; }
        ans->freeFunction=&std::free;
        return ++ans;
    }
    else {
        if (is_pool_constructed.load()) {
            return pool_malloc_functions[arg]();
        }
        else {
            static const bool is_pools_init=[]() {/*at least c++ 11*/
                constructPools();
                is_pool_constructed.store(true);
                return true;
            }();
            return pool_malloc_functions[arg]();
            (void)is_pools_init;
        }
    }
}

void free(void *arg) {
    if (arg==nullptr) { return; }
    auto * ans=reinterpret_cast<DataItemType*>(arg);
    --ans;
    {
        auto free_function=ans->freeFunction;
        return free_function(ans);
    }
}

void freePoolMemory() {
    if (is_pool_constructed.load()) {
        auto * begin=reinterpret_cast<pool_type*>(
            const_cast<char*>(pool_plain_data));
        auto * end=begin+max_malloc_size;
        for (; begin!=end; ++begin) {
            begin->release_memory();
        }
    }
}

}/*memory*/
}/*__memory*/
}/*__private*/
}/*__cpp*/
}/*~namespace*/

namespace memory {

void * malloc(std::size_t arg) {
    return __cpp::__private::__memory::memory::malloc(arg);
}

void free(void *arg) {
    return __cpp::__private::__memory::memory::free(arg);
}

void freePoolMemory() {
    return __cpp::__private::__memory::memory::freePoolMemory();
}

}/*memory*/

/*
*/







