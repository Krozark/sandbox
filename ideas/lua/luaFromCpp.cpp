#include <iostream>

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <luabind/luabind.hpp>
#include <luabind/object.hpp>

int main(int argc,char * argv[])
{
    lua_State* state = luaL_newstate();

    luaL_openlibs(state);
    luabind::open(state);

    luaL_dofile(state,"lua/luaFromCpp.lua");


    int i = luabind::call_function<int>(state, "Test",42);
    std::cout<<i<<std::endl;
    

    lua_close(state);
    

    return 0;
}
