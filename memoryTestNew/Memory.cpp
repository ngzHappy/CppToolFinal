#include "Memory.hpp"
#include <atomic>
#include <future>
#include <thread>
#include <boost/pool/pool.hpp>

//#define _TEST_MEMORY_MALLOC_FREE_
#if defined(_TEST_MEMORY_MALLOC_FREE_)
#include <fstream>
static std::ofstream ofs("req.txt");
#endif

namespace {
namespace __cpp {
namespace __private {
namespace __memory {
namespace memory {

namespace middle {
constexpr const std::size_t step=32;
constexpr const std::size_t step_sub_1=31;
constexpr const std::size_t step_exp=5;
}

/*
(use -ftemplate-depth= to increase the maximum)
Qt:QMAKE_CXXFLAGS += -ftemplate-depth=4100
*/
constexpr const std::size_t small_malloc_size=1024*4;
constexpr const std::size_t max_middle_malloc_size=1024*32;
constexpr const std::size_t middle_malloc_size=(max_middle_malloc_size-small_malloc_size)/middle::step;
std::atomic_bool _d_is_construct_static;
typedef std::make_index_sequence<small_malloc_size> index_sequence_type;
typedef void(*FreeFunctionType)(void *);
typedef void*(*PoolMallocFunctionType)(void);
typedef boost::pool<boost::default_user_allocator_malloc_free> pool_type;

class DataItemType {
public:
    FreeFunctionType freeFunction;
};

/*gen small data space*/
static char pool_plain_data[sizeof(pool_type)*small_malloc_size];
static PoolMallocFunctionType pool_malloc_functions[1+small_malloc_size];
/*gen middle data space*/
class Middle {
    static char pool_plain_data[middle_malloc_size*sizeof(pool_type)];
    static PoolMallocFunctionType pool_malloc_functions[1+middle_malloc_size];
    static std::atomic_bool is_pool_construct;
    static void constructPools();
    static void destructPools();
    typedef std::make_index_sequence<middle_malloc_size> index_sequence_type;

    /*0->1*/
    template<std::size_t N>
    static void * atPoolMalloc() {
        auto * pool=reinterpret_cast<pool_type*>(
            const_cast<char *>(pool_plain_data))+N;
        auto * ans=reinterpret_cast<DataItemType*>(pool->malloc());
#if defined(_TEST_MEMORY_MALLOC_FREE_)
        ofs<<pool->get_requested_size()<<std::endl;
#endif
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
            new (++functionMalloc) PoolMallocFunctionType[middle_malloc_size]{
                &atPoolMalloc<I>...
            };
        }
    };

public:
    static void * malloc(std::size_t arg) {
        if (is_pool_construct.load()) {
            return pool_malloc_functions[arg]();
        }
        else {
            static const bool _is_init_=[]() {
                constructPools();
                is_pool_construct.store(true);
                return true;
            }();
            return pool_malloc_functions[arg]();
            (void)_is_init_;
        }

        return nullptr;
    }

    static void freePoolMemory() {
        if (is_pool_construct.load()) {
            auto * begin=reinterpret_cast<pool_type*>(
                const_cast<char*>(pool_plain_data));
            auto * end=begin+middle_malloc_size;
            for (; begin!=end; ++begin) {
                begin->release_memory();
            }
        }
    }

};

std::atomic_bool Middle::is_pool_construct;
char Middle::pool_plain_data[middle_malloc_size*sizeof(pool_type)];
PoolMallocFunctionType Middle::pool_malloc_functions[1+middle_malloc_size];

/*0->1*/
template<std::size_t N>
void * atPoolMalloc() {
    auto * pool=reinterpret_cast<pool_type*>(
        const_cast<char *>(pool_plain_data))+N;
    auto * ans=reinterpret_cast<DataItemType*>(pool->malloc());
#if defined(_TEST_MEMORY_MALLOC_FREE_)
    ofs<<pool->get_requested_size()<<std::endl;
#endif
    if (ans==nullptr) { return nullptr; }
    ans->freeFunction=[](void * arg) {
        auto * pool=reinterpret_cast<pool_type*>(
            const_cast<char *>(pool_plain_data))+N;
        pool->free(arg);
    };
    return ++ans;
}

void Middle::constructPools() {
    {/*初始化内存池*/
        auto * begin=reinterpret_cast<pool_type*>(
            const_cast<char *>(pool_plain_data));
        for (std::size_t i=0; i<middle_malloc_size; ++i) {
            const auto item_size=i*middle::step+(small_malloc_size+middle::step);
            new(begin) pool_type{ item_size };
            ++begin;
        }
    }
    {/*初始化malloc函数*/
        ConstructPools< index_sequence_type >::construct();
    }
}

void Middle::destructPools() {
    char * tmp=const_cast<char *>(pool_plain_data);
    auto * begin=reinterpret_cast<pool_type*>(tmp);
    auto * end=begin+middle_malloc_size;
    for (; begin!=end; ++begin) {
        begin->~pool();
    }
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
        new (++functionMalloc) PoolMallocFunctionType[small_malloc_size]{
            &atPoolMalloc<I>...
        };
    }
};

/*the moudle is use for application so do not need destruct*/
/*
void destructPools(){
    char * tmp=const_cast<char *>(pool_plain_data);
    auto * begin=reinterpret_cast<pool_type*>(tmp);
    auto * end=begin+small_malloc_size;
    for(;begin!=end;++begin){
        begin->~pool();
    }
}
*/

void constructPools() {
    {/*初始化内存池*/
        auto * begin=reinterpret_cast<pool_type*>(
            const_cast<char *>(pool_plain_data));
        for (std::size_t i=0; i<small_malloc_size; ++i) {
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
    if (arg>small_malloc_size) {
        auto arg_add_4=arg+sizeof(DataItemType);
        if ((arg>max_middle_malloc_size)||((arg_add_4)>max_middle_malloc_size)) {
            /*大对象!!there may be a bug
            @ arg+sizeof(DataItemType) may be bigger than int_max*/
            auto * ans=reinterpret_cast<DataItemType*>(std::malloc(arg_add_4));
            if (ans==nullptr) { return nullptr; }
            ans->freeFunction=&std::free;
            return ++ans;
        }
        else {
            /*中对象*/
            arg_add_4-=small_malloc_size;
            auto quot=arg_add_4>>middle::step_exp;/*arg_add_4/middle::step;*/
            quot+=((arg_add_4&middle::step_sub_1)>0);
            return Middle::malloc(quot);
        }
    }
    else {
        /*小对象*/
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
    {
        if (is_pool_constructed.load()) {
            auto * begin=reinterpret_cast<pool_type*>(
                const_cast<char*>(pool_plain_data));
            auto * end=begin+small_malloc_size;
            for (; begin!=end; ++begin) {
                begin->release_memory();
            }
        }
    }
    {
        Middle::freePoolMemory();
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

bool __memory__construct_static::__is_construct_static() noexcept(true) {
    return __cpp::__private::__memory::memory::_d_is_construct_static.load();
}

void __memory__construct_static::__set_construct_static() noexcept(true) {
    return __cpp::__private::__memory::memory::_d_is_construct_static.store(true);
}

bool __memory__construct_static::__run_once(void(*arg)(void)) noexcept(true) {
    /*it will not be deleted*/
    static auto * _d_run_once_flag=new std::once_flag;
    try {
        std::call_once(*_d_run_once_flag,arg);
    }
    catch (...) {
        /*do nothing*/
        return false;
    }
    return true;
}

}/*memory*/

/*
*/








