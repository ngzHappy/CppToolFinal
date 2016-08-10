#ifndef __APP_MAIN_IMPORT_HPP_0x00
#define __APP_MAIN_IMPORT_HPP_0x00

#include <memory>
#include <thread>
#include <cassert>

#ifndef _APP_MAIN_IMPORT
#define _APP_MAIN_IMPORT
#endif
namespace app_main {

_APP_MAIN_IMPORT int argc();
_APP_MAIN_IMPORT char ** argv();
_APP_MAIN_IMPORT bool is_main_entered();
_APP_MAIN_IMPORT bool is_main_exited();
_APP_MAIN_IMPORT void add_donot_delete_resource(std::shared_ptr<const void>);
_APP_MAIN_IMPORT std::thread::id main_thread_id();

class _APP_MAIN_IMPORT main {
    static void set_main_argc(int);
    static void set_main_argv(char**);
    static void set_main_entered();
    static void set_main_exited();
    static void set_main_thread_id();
public:
    main(int argc=0,char** argv=nullptr) {
        /*the class just init once*/
        assert((is_main_entered()==false)&&(is_main_exited()==false));
        set_main_argc(argc);
        set_main_argv(argv);
        set_main_thread_id();
        set_main_entered();
    }
    virtual ~main() {
        set_main_exited();
        assert((is_main_entered()==true)&&(is_main_exited()==true));
    }
};


}

#endif

/*cpp*/
#include <mutex>
#include <atomic>
#include <forward_list>
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

namespace {
auto * mutex_donot_delete_resource() {/*do not delete resource mutex*/
    static auto *var=new std::recursive_mutex;
    return var;
}
auto * data_donot_delete_resource() {/*do not delete resource data*/
    static auto *var=new std::forward_list<std::shared_ptr<const void>>{};
    return var;
}
}/*namespace*/
void add_donot_delete_resource(std::shared_ptr<const void>arg) {
    std::unique_lock<std::recursive_mutex> __lock(*mutex_donot_delete_resource());
    data_donot_delete_resource()->push_front(std::move(arg));
}

namespace __private_data{
std::atomic<std::thread::id> main_thread_id;
}
std::thread::id main_thread_id() {
    return __private_data::main_thread_id.load();
}
void main::set_main_thread_id() {
    __private_data::main_thread_id.store(std::this_thread::get_id());
}

}



int main(int argc,char **argv) {
    app_main::main app(argc,argv);
    app_main::is_main_entered();
    app_main::is_main_exited();
    (void)app;
}


