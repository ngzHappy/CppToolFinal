#include <stdexcept>
#include <iostream>
#if defined(QT_CORE_LIB)
#include<QtCore/qcoreapplication.h>
#endif

extern int run(int argc,char ** argv);

int main(int argc,char ** argv) try {
#if defined(QT_CORE_LIB)
    QCoreApplication app(argc,argv);
#endif
    return run(argc,argv);
#if defined(QT_CORE_LIB)
    (void)app;
#endif
}
catch (const std::exception & e) {
    std::cout<<e.what()<<std::endl;
    return -1;
}
catch (...) {
    std::cout<<"???"<<std::endl;
    return -9999;
}



