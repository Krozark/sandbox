find_package(Lua51 REQUIRED)

include_directories(${LUA_INCLUDE_DIR})

find_library(LIB_LUABIND luabind REQUIRED)

add_executable(cppFromLua cppFromLua.cpp)
target_link_libraries(cppFromLua ${LUA_LIBRARY} ${LIB_LUABIND})

add_executable(luaFromCpp luaFromCpp.cpp)
target_link_libraries(luaFromCpp ${LUA_LIBRARY} ${LIB_LUABIND})

add_executable(cppMixLua cppMixLua.cpp)
target_link_libraries(cppMixLua ${LUA_LIBRARY} ${LIB_LUABIND})
