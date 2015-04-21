#include <iostream>

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <luabind/luabind.hpp>
#include <luabind/object.hpp>

void print_test(int number)
{
    std::cout<<"Hello "<<number<<std::endl;
}


class CppTest
{
    public:
        CppTest(int number): _i(number) {};

        void print(){
            std::cout<<"CppTest._i = "<<_i<<std::endl;
        };

        static void registerToLua(lua_State* state)
        {
            luabind::module(state)[
                luabind::class_<CppTest>("CppTest")
                .def(luabind::constructor<int>())
                .def("print",&CppTest::print)
                .def_readwrite("_i",&CppTest::_i)
                ];
        }


        int _i;
};

int main(int argc,char * argv[])
{
    lua_State* state = luaL_newstate();

    luaL_openlibs(state);
    luabind::open(state);

    luabind::module(state)[
        luabind::def("print_test",print_test)
    ];

    CppTest::registerToLua(state);

    luaL_dofile(state,"cppMixLua.lua");

    int i = luabind::call_function<int>(state, "Test",42);
    std::cout<<i<<std::endl;

    lua_close(state);

    
    return 0;
    
}
