if(LAB_BUILD_LUA)
	add_subdirectory(lua)
endif()

if(LAB_BUILD_SFML)
	add_subdirectory(sfml)
endif()

if(LAB_BUILD_SHADOWS)
    add_subdirectory(shadows)
endif()
