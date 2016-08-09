/*
 * 此文件是为应用程序开发，如果是热拔插的插件则会出现内存泄露
*/
#include <array>
#include <memory>
#include "Memory.hpp"

#include <boost/pool/pool.hpp>

namespace memory {

namespace {

static_assert(sizeof(char)==1,"core error!!");
/*static_assert(((std::intptr_t)(((char*)(0))+1))==1,"core error!!");*/

class FreeFunctionItem {
public:
    union {
        void(*data)(void *);
        void*_private_freefunctionitem_data
            /*the data must not small than a pointer*/;
    };
};

template<std::size_t N>
class MallocFreeType {
public:
    static void * malloc(std::size_t arg) {
        auto var=reinterpret_cast<FreeFunctionItem *>(
                    std::malloc(arg+sizeof(FreeFunctionItem)));
        var->data=&std::free;
        return reinterpret_cast<char *>(var)+sizeof(FreeFunctionItem);
    }
};

enum { MaxQuickMallocFree=1024 };

class __pool_type :
    public boost::pool<boost::default_user_allocator_malloc_free> {
    using _p_super=boost::pool<boost::default_user_allocator_malloc_free>;
public:
    __pool_type(std::size_t _n_):_p_super(_n_) {}
};

typedef __pool_type pool_type_private;
inline static char * __pools_value(){
    static char _d_pools_value[sizeof(std::array<pool_type_private,MaxQuickMallocFree>)];
    return static_cast<char*>(_d_pools_value);
}

inline std::array<pool_type_private,MaxQuickMallocFree> &memory_pools() {
    enum { __size=sizeof(FreeFunctionItem) };
    /*do not need delete*/
    static auto* varAns=[]() {
        auto var=reinterpret_cast<pool_type_private*>(__pools_value());
        {
            auto varI=var;
#include"memory_private/Memory.pool.hpp"
        }
        return reinterpret_cast<std::array<pool_type_private,MaxQuickMallocFree>*>(var);
    }();
    return *varAns;
}

template<std::size_t _N_>
class __MallocFreeType {
    enum { _Value_=_N_ };
    class MallocFreeStaticData {
    private:
        MallocFreeStaticData&operator=(const MallocFreeStaticData&)=delete;
        MallocFreeStaticData&operator=(MallocFreeStaticData&&)=delete;
        MallocFreeStaticData(const MallocFreeStaticData&)=delete;
        MallocFreeStaticData(MallocFreeStaticData&&)=delete;
    private:
        pool_type_private&pool_=memory_pools()[_Value_-1];
    public:
        MallocFreeStaticData();
        ~MallocFreeStaticData();

        void *malloc();
        void free(void*);
    };
    static MallocFreeStaticData&instance();
    static void free(void*arg) { instance().free(arg); }
public:
    static void * malloc() {
        auto var=reinterpret_cast<FreeFunctionItem *>(
                    instance().malloc(/*_Value_+sizeof(FreeFunctionItem)*/));
        var->data=&MallocFreeType<_Value_>::free;
        return reinterpret_cast<char *>(var)+sizeof(FreeFunctionItem);
    }
};

template<std::size_t _N_>
__MallocFreeType<_N_>::MallocFreeStaticData::MallocFreeStaticData() {
}

template<std::size_t _N_>
__MallocFreeType<_N_>::MallocFreeStaticData::~MallocFreeStaticData() {
}

template<std::size_t _N_>
auto __MallocFreeType<_N_>::instance()-> MallocFreeStaticData& {
    /*do not need delete*/
    static auto * var=[]() {
        auto ans=new MallocFreeStaticData/*c++11 thread safe*/;
        /*TODO:add debug test code*/
        return ans;
    }();
    return *var;
}

template<std::size_t _N_>
void * __MallocFreeType<_N_>::MallocFreeStaticData::malloc() {
    return this->pool_.malloc();
}

template<std::size_t _N_>
void __MallocFreeType<_N_>::MallocFreeStaticData::free(void * arg) {
    return this->pool_.free(arg);
}

#include"memory_private/Memory.template.hpp"
}/*namespace*/

void *malloc(std::size_t arg) {
    if (arg<1) { return nullptr; }
    if (arg>MaxQuickMallocFree) { return MallocFreeType<MaxQuickMallocFree+1>::malloc(arg); }
    switch (arg) {
        case 0:return nullptr;
#include"memory_private/Memory.case.hpp"
    }
    return nullptr;
}
void free(void*arg) {
    if (arg==nullptr) { return; }
    auto var=reinterpret_cast<FreeFunctionItem*>(
        (char*)(arg)-sizeof(FreeFunctionItem));
    auto varFreeFunction=var->data;
    /*TODO:add debug check code*/
    return varFreeFunction(var);
}

}/*namespace memory*/



