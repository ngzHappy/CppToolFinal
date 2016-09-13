#include "Memory.hpp"

int main(){

    double * data_=reinterpret_cast<double*>(memory::malloc(512));
    *data_=123;
    memory::free( data_ );
    memory::freePoolMemory();
}


