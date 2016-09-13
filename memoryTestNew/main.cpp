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

int main(){

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


