macro(set_option var default type docstring)
	if(NOT DEFINED ${var})
		set(${var} ${default})
	endif()
		set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

include(CMakeParseArguments)

macro(build_library target)
	cmake_parse_arguments(THIS "" "" "SOURCES;TYPE;DEPENDS;EXTERNAL_LIBS" ${ARGN})
	
	if(THIS_TYPE STREQUAL SHARED)
		set(lib ${target}-shared)
	elseif(THIS_TYPE STREQUAL STATIC)
		set(lib ${target}-static)
	else()
		message(FATAL_ERROR "No TYPE specify for the build. Please set STATIC or SHARED")
	endif()
	
	add_library(${lib} ${THIS_TYPE} ${THIS_SOURCES})

	set_target_properties(${lib} PROPERTIES
		OUTPUT_NAME ${target}
		CLEAN_DIRECT_OUTPUT 1
		#DEBUG_POSTFIX -d
		VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
		SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}
	)
	install(TARGETS ${lib}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib 
		ARCHIVE DESTINATION lib
	)

	if(THIS_DEPENDS)
		target_link_libraries(${lib} ${THIS_DEPENDS})
	endif()
	if(THIS_EXTERNAL_LIBS)
		target_link_libraries(${lib} ${THIS_EXTERNAL_LIBS})
	endif()
endmacro()


macro(deploy)
	cmake_parse_arguments(THIS "" "" "FILES;DESTINATION" ${ARGN})
	
	message(Deploy : ${THIS_FILES})
	
	file(GLOB DEPLOY_FILES_AND_DIRS "${THIS_FILES}")
	
	message(Deploy : ${DEPLOY_FILES_AND_DIRS})
	
	set(FILES_TO_DEPLOY "")
	set(DIRS_TO_DEPLOY "")
	
	foreach(ITEM ${DEPLOY_FILES_AND_DIRS})
		IF( IS_DIRECTORY "${ITEM}" )
			LIST( APPEND DIRS_TO_DEPLOY "${ITEM}" )
			message(DIR ${ITEM})
		ELSE()
			LIST( APPEND FILES_TO_DEPLOY "${ITEM}" )
			message(file ${ITEM})
		ENDIF()
	endforeach()
	INSTALL( FILES ${FILES_TO_DEPLOY} DESTINATION ${THIS_DESTINATION} )
	INSTALL( DIRECTORY ${DIRS_TO_DEPLOY} DESTINATION ${THIS_DESTINATION} )
endmacro()