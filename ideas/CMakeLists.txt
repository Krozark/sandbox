find_package(Lua51 REQUIRED)

include_directories(${LUA_INCLUDE_DIR})

find_library(LIB_LUABIND luabind REQUIRED)

add_executable(CppFromLua
    cppFromLua.cpp)
target_link_libraries(CppFromLua ${LUA_LIBRARY} ${LIB_LUABIND})
