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


class Test
{
    public:
        Test(int number): _i(number) {};

        void print(){
            std::cout<<"Test._i = "<<_i<<std::endl;
        };

        static void registerToLua(lua_State* state)
        {
            luabind::module(state)[
                luabind::class_<Test>("Test")
                .def(luabind::constructor<int>())
                .def("print",&Test::print)
                .def_readwrite("_i",&Test::_i)
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

    Test::registerToLua(state);


    luaL_dofile(state,"lua/cppFromLua.lua");
    

    lua_close(state);
    

    return 0;
}
