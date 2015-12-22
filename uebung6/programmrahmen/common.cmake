if(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=gnu++11)
elseif(APPLE)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
elseif(UNIX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=gnu++11)
endif()

#Create source groups under MSVC
macro(create_source_groups)
	if(MSVC)
		foreach(filename ${ARGN})
			# if not in flat mode, get path relative to current source dir
			get_filename_component(path "${filename}" REALPATH)
			file(RELATIVE_PATH path ${CMAKE_CURRENT_SOURCE_DIR} ${path})
			get_filename_component(path "${path}" PATH)

			string(REPLACE "/" "\\" path "${path}")
			source_group("${path}" FILES ${filename})
		endforeach()
	endif(MSVC)
endmacro()