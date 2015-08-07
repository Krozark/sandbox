find_package(SFML 2 COMPONENTS system graphics window audio network)
if(NOT SFML_FOUND)
	message(FATAL_ERROR "SFML Modules not find. Please set the SFML_ROOT variable to your SFML root installation directory and retry")
endif()

#add SFML
include_directories(${SFML_INCLUDE_DIR})
set(LIBS ${SFML_LIBRARIES})

add_executable(sfml-raytracing raycasting.cpp)
target_link_libraries(sfml-raytracing ${LIBS})


