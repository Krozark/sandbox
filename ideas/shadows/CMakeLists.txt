find_package(SFML 2 COMPONENTS system graphics window)
if(NOT SFML_FOUND)
	message(FATAL_ERROR "SFML Modules not find. Please set the SFML_ROOT variable to your SFML root installation directory and retry")
endif()

#add SFML
include_directories(${SFML_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

set(LIBS ${SFML_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(sfml-shadows
    AllInclude.h
    Constructs.cpp
    Constructs.h
    ConvexHull.cpp
    ConvexHull.h
    Light.cpp
    Light.h
    LightSystem.cpp
    LightSystem.h
    main.cpp
    SDLOpenGL.cpp
    SDLOpenGL.h
    Utilities.cpp
    Utilities.h
)
target_link_libraries(sfml-shadows ${LIBS})


