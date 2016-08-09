
#include "Memory.hpp"
#include <fstream>
#include <iostream>

int main() {

    for(int i=0;i<1025;++i)
    {
        auto data=memory::malloc(i);
        memory::free(data);
    }

    for (int i=0; i<10; ++i) {

        auto data=memory::malloc(i);
        std::cout<<data<<",";
        memory::free(data);

        data=memory::malloc(i);
        std::cout<<data<<std::endl;
        memory::free(data);

        std::cout<<std::endl;
    }

}

