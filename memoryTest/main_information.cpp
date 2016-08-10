
#include<cassert>
#ifndef _APP_MAIN_IMPORT
#define _APP_MAIN_IMPORT
#endif
namespace app_main {

_APP_MAIN_IMPORT int argc();
_APP_MAIN_IMPORT char ** argv();
_APP_MAIN_IMPORT bool is_main_entered();
_APP_MAIN_IMPORT bool is_main_exited();

class _APP_MAIN_IMPORT main {
    static void set_main_argc(int);
    static void set_main_argv(char**);
    static void set_main_entered();
    static void set_main_exited();
public:
    main(int argc=0,char** argv=nullptr) {
        /*the class just init once*/
        assert((is_main_entered()==false)&&(is_main_exited()==false));
        set_main_argc(argc);
        set_main_argv(argv);
        set_main_entered();
    }
    virtual ~main() {
        set_main_exited();
        assert((is_main_entered()==true)&&(is_main_exited()==true));
    }
};


}


#include<atomic>
namespace app_main {

namespace app_main_data {
namespace {
static int argc;
static char **argv;
std::atomic<bool> is_main_entered;
std::atomic<bool> is_main_exited;
}/*namespace*/
}/*app_main_data*/

void main::set_main_argc(int arg) { app_main_data::argc=arg; }
void main::set_main_argv(char **arg) { app_main_data::argv=arg; }
void main::set_main_entered() { app_main_data::is_main_entered.store(true); }
void main::set_main_exited() { app_main_data::is_main_exited.store(true); }

int argc() { return app_main_data::argc; }
char ** argv() { return app_main_data::argv; }
bool is_main_entered() { return app_main_data::is_main_entered.load(); }
bool is_main_exited() { return app_main_data::is_main_exited.load(); }

}



int main(int argc,char **argv) {
    app_main::main app(argc,argv);
    app_main::is_main_entered();
    app_main::is_main_exited();
    (void)app;
}