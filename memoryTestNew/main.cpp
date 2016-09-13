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

int main(){

    double * data_=reinterpret_cast<double*>(memory::malloc(512));
    *data_=123;
    memory::free( data_ );
    memory::freePoolMemory();

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

}


