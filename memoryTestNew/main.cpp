#include "Memory.hpp"
#include<list>
#include"Allocator.hpp"
//#include <allocator>

class Test {
    int value=12;
public:
    Test() {}
    ~Test(){}
};

#include<iostream>
#include<fstream>

#include<algorithm>
#include<random>

#include<chrono>

constexpr const std::size_t test_size=50240;
inline std::size_t (&test_index())[test_size] {
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

int main(){

    auto & index_=test_index();

    {
        auto begin_=std::chrono::high_resolution_clock::now();
        for (auto i:index_) {
            std::free(std::malloc(i));
        }
        auto end_=std::chrono::high_resolution_clock::now();

        std::cout<< std::chrono::duration_cast<
            std::chrono::duration<double,
            std::chrono::seconds::period>> (end_-begin_).count()
            <<std::endl;
    }

    {
        auto begin_=std::chrono::high_resolution_clock::now();
        for (auto i:index_) {
            memory::free(memory::malloc(i));
        }
        auto end_=std::chrono::high_resolution_clock::now();

        std::cout<< std::chrono::duration_cast<
            std::chrono::duration<double,
            std::chrono::seconds::period>> (end_-begin_).count()
            <<std::endl;
    }

    for (int n=0; n<100; ++n) {
        std::cout<<"------------------------"<<std::endl;

        {
            auto begin_=std::chrono::high_resolution_clock::now();
            for (auto i:index_) {
                std::free(std::malloc(i));
            }
            auto end_=std::chrono::high_resolution_clock::now();

            std::cout<<std::chrono::duration_cast<
                std::chrono::duration<double,
                std::chrono::seconds::period>> (end_-begin_).count()
                <<std::endl;
        }

        {
            auto begin_=std::chrono::high_resolution_clock::now();
            for (auto i:index_) {
                memory::free(memory::malloc(i));
            }
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


