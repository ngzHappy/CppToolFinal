#include <memory>
#include <type_traits>

#ifndef MEMORY_IS_PLUGIN
#define MEMORY_IS_PLUGIN false
#endif

namespace __private {
namespace __memory {

template<typename _T_>
class _P_StaticData {
    union Data {
    public:
        Data() {}
        ~Data() {}
        _T_ _m_value;
    };
    Data _m_data;
public:
    auto * pointer() { return &_m_data._m_value; }
    const auto * pointer() const { return &_m_data._m_value; }
};

/*****************************************************************/
template<typename _T_,bool _IsConstructible_>
class IsConstructible {
public:
    template<typename ...Args>
    static void construct(const void * argData,
        Args&&...args) {
        ::new(const_cast<void*>(argData)) _T_(std::forward<Args>(args)...);
    }
};

template<typename _T_>
class IsConstructible<_T_,false> {
public:
    template<typename ...Args>
    static void construct(const void * argData,
        Args&&...args) {
        ::new(const_cast<void*>(argData)) _T_{ std::forward<Args>(args)... };
    }
};
/*****************************************************************/

template<typename _PT_,bool _is_trivially_destructible_
    =std::is_trivially_destructible<_PT_>::value>
    class IsTriviallyDestructible {
    public:
        template<typename _U_>
        static void destruct(_U_*arg) {/*true*/ }
};

template<typename _PT_>
class IsTriviallyDestructible<_PT_,false> {
public:
    template<typename _U_>
    static void destruct(_U_*arg) { arg->~_U_(); }
};

/*****************************************************************/

}/*__memory*/
}/*__private*/

template<
    typename _t_Type,
    void(*_needClose_)(const void *)=nullptr,
    bool _isPlugin_=MEMORY_IS_PLUGIN,
    typename _not_used_=void
>
class StaticData {

    static_assert(std::is_reference<_t_Type>::value==false,
        "the type must not be reference");
    __private::__memory::_P_StaticData<_t_Type> _m_data;

    template<typename _U_>
    static void _p_close(_U_*arg) { _needClose_(arg); }

    template<typename _U_>
    void _p_destruct(_U_*arg) {
        __private::__memory::template IsTriviallyDestructible<_U_> \
            ::destruct(arg);
    }

    void _pf_close() { _p_close(_m_data.pointer()); }
    void _pf_destruct() { _p_destruct(_m_data.pointer()); }
public:

    typedef void(*CloseFunction)(const void *);

    StaticData(const StaticData&)=delete;
    StaticData(StaticData&&)=delete;
    StaticData&operator=(const StaticData&)=delete;
    StaticData&operator=(StaticData&&)=delete;

public:

    constexpr bool isPlugin() const { return _isPlugin_; }
    constexpr bool needClose() const { return _needClose_; }

    template<typename ... Args>
    StaticData(Args&&...args) {
        constexpr bool varIsConstructible
            =std::is_constructible<_t_Type,Args...>::value;
        using VarT=
            __private::__memory::IsConstructible<_t_Type,varIsConstructible>;
        /* new T(...) or new T{...} */
        VarT::construct(_m_data.pointer(),
            std::forward<Args>(args)...);
    }

    ~StaticData() {

        if /*constexpr c++17*/ (_needClose_) {
            _pf_close();
        }

        if /*constexpr c++17*/ (_isPlugin_) {
            /*if is main quited return*/
            _pf_destruct();
        }

    }

public:
    auto & operator*() { return *_m_data.pointer(); }
    const auto & operator*() const { return *_m_data.pointer(); }

    auto operator->() { return _m_data.pointer(); }
    const auto operator->()const { return _m_data.pointer(); }

};

#include <iostream>
class Test {
public:
    ~Test() { std::cout<<"~Test : "<<value<<std::endl; }
    void close() { std::cout<<"~Test : close"<<value<<std::endl; }
    int value;
};


int main() {

    //StaticData<Test> xx(12);


    {

        static StaticData<int[12]> yy(12);

        (*yy)[1];

    }

    {

    }

    int v=12;
    return ++v;

}

