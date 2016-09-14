#include "Memory.hpp"
#include<list>
#include"Allocator.hpp"
#include"MemoryStatic.hpp"
//#include <allocator>

class Test {
    int value=12;
    double value_1=234;
    float value_2=342;
public:
    Test() {}
    ~Test() {}
};

#include<iostream>
#include<fstream>

#include<algorithm>
#include<random>

#include<chrono>

constexpr const std::size_t test_size=1950240;

inline std::size_t(&test_index())[test_size] {
    static std::size_t test_data[test_size];

    std::random_device dev;
    std::default_random_engine engine{ dev() };
    std::uniform_int_distribution<> u(1,1024*30);

    for (auto & i:test_data) {
        i=u(engine);
    }

    std::random_shuffle(test_data,test_data+test_size);
    return test_data;
}

//inline void *(&test_tmp_data())[test_size] {
//    static void * test_data[test_size];
//    return test_data;
//}

#include<scoped_allocator>
#include<vector>
#include<string>

#include"MakeShared.hpp"
#include "MakeFunction.hpp"
int main() {

    {
        int a={};
        double b={};
        float c={};

        int a1={};
        double b1={};
        float c1={};

        auto f= memory::make_function<double(void)>(
                    [a,b,c,a1,b1,c1]()->double {
            auto x=a+b+c+a1+b1+c1;
            return x;
        });
        f();
    }

    {
        memory::constructStatic();
        memory::constructStatic();
    }

    {
        memory::make_shared<const Test>( );
        enum TTX {
            x=sizeof(Test)
        };
    }

    {
        memory::make_shared<const int>();
        memory::instance_shared<const double>(34.2);
    }

    {
        using String=std::basic_string<
            char,
            std::char_traits<char>,
            memory::Allocator<char>
        >;

        using T=std::scoped_allocator_adaptor<
            memory::Allocator<String>
        >;

        std::vector< String,T > v;
        v.push_back("12dgdgfdg3");
        v.push_back("4dd56");

        for (const auto & i:v) {
            std::cout<<i<<std::endl;
        }

    }

    auto & index_=test_index();

    {
        auto begin_=std::chrono::high_resolution_clock::now();
        void * tmp_data=nullptr;
        for (auto i:index_) {
            std::free(tmp_data);
            tmp_data=std::malloc(i);
        }
        std::free(tmp_data);
        auto end_=std::chrono::high_resolution_clock::now();

        std::cout<<std::chrono::duration_cast<
            std::chrono::duration<double,
            std::chrono::seconds::period>> (end_-begin_).count()
            <<std::endl;
    }

    {
        auto begin_=std::chrono::high_resolution_clock::now();
        void * tmp_data=nullptr;
        for (auto i:index_) {
            memory::free(tmp_data);
            tmp_data=memory::malloc(i);
        }
        memory::free(tmp_data);
        auto end_=std::chrono::high_resolution_clock::now();

        std::cout<<std::chrono::duration_cast<
            std::chrono::duration<double,
            std::chrono::seconds::period>> (end_-begin_).count()
            <<std::endl;
    }

    for (int n=0; n<1; ++n) {
        std::cout<<"------------------------"<<std::endl;

        {
            auto begin_=std::chrono::high_resolution_clock::now();
            void * tmp_data=nullptr;
            for (auto i:index_) {
                std::free(tmp_data);
                tmp_data=std::malloc(i);
            }
            std::free(tmp_data);
            auto end_=std::chrono::high_resolution_clock::now();

            std::cout<<std::chrono::duration_cast<
                std::chrono::duration<double,
                std::chrono::seconds::period>> (end_-begin_).count()
                <<std::endl;
        }

        {
            auto begin_=std::chrono::high_resolution_clock::now();
            void * tmp_data=nullptr;
            for (auto i:index_) {
                memory::free(tmp_data);
                tmp_data=memory::malloc(i);
            }
            memory::free(tmp_data);
            auto end_=std::chrono::high_resolution_clock::now();

            std::cout<<std::chrono::duration_cast<
                std::chrono::duration<double,
                std::chrono::seconds::period>> (end_-begin_).count()
                <<std::endl;
        }
    }

    {
        std::ofstream ofs("ask.txt");
        for (int i=0; i<(1024*4); ++i) {
            double * data_=reinterpret_cast<double*>(memory::malloc(i));
            ofs<<i<<std::endl;
            memory::free(data_);
        }

        for (int i=(1024*4); i<(1024*32); ++i) {
            double * data_=reinterpret_cast<double*>(memory::malloc(i));
            ofs<<i<<std::endl;
            memory::free(data_);
        }

    }

    {
        std::cout<<"~~~~~~~"<<std::endl;
        constexpr std::size_t size_test=590240;
        static void * test_ans[size_test];

        {
            auto begin_=std::chrono::high_resolution_clock::now();
            for (std::size_t i=0; i<size_test; ++i) {
                test_ans[i]=std::malloc(12);
            }
            for (std::size_t i=0; i<size_test; ++i) {
                std::free(test_ans[i]);
            }
            auto end_=std::chrono::high_resolution_clock::now();

            std::cout<<std::chrono::duration_cast<
                std::chrono::duration<double,
                std::chrono::seconds::period>> (end_-begin_).count()
                <<std::endl;
        }

        {
            auto begin_=std::chrono::high_resolution_clock::now();
            for (std::size_t i=0; i<size_test; ++i) {
                test_ans[i]=memory::malloc(12);
            }
            for (std::size_t i=0; i<size_test; ++i) {
                memory::free(test_ans[i]);
            }
            auto end_=std::chrono::high_resolution_clock::now();

            std::cout<<std::chrono::duration_cast<
                std::chrono::duration<double,
                std::chrono::seconds::period>> (end_-begin_).count()
                <<std::endl;
        }

    }

    {
        double * data_=reinterpret_cast<double*>(memory::malloc(512));
        *data_=123;
        memory::free(data_);
        //memory::freePoolMemory();
    }

    {
        double * data_=reinterpret_cast<double*>(memory::malloc(1024*32-16));
        *data_=123;
        memory::free(data_);
        //memory::freePoolMemory();
    }

    {
        std::list< Test,memory::Allocator<int> > test;

        test.resize(12);
        test.push_back({});
        test.push_back({});
        test.push_back({});
        test.push_back({});
        test.push_back({});
        test.push_back({});
    }

    memory::Allocator<int> a;
    memory::Allocator<double> b;
    a==b;
    system("pause");

}


