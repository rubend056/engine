cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
# message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
# message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
# message("CMAKE_CXX_FLAGS_MINSIZEREL is ${CMAKE_CXX_FLAGS_MINSIZEREL}")

project(engine)

message("-- CMAKE_SYSTEM_INFO_FILE: 	${CMAKE_SYSTEM_INFO_FILE}")
message("-- CMAKE_SYSTEM_NAME:      	${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_PROCESSOR: 	${CMAKE_SYSTEM_PROCESSOR}")
message("-- CMAKE_SYSTEM:           	${CMAKE_SYSTEM}")

# SET(GLAD_OPENGL_CORE_VERSION "45" CACHE STRING "OPENGL_VERSION: either 30 or 45")
# message("-- GLAD_OPENGL_CORE_VERSION:	${GLAD_OPENGL_CORE_VERSION}")

# find_package(SDL2 REQUIRED)
# message("-- SDL2_INCLUDE_DIRS 			${SDL2_INCLUDE_DIRS}")
# message("-- SDL2_LIBRARIES 				${SDL2_LIBRARIES}")
find_package(ASSIMP REQUIRED)
message("-- ASSIMP_INCLUDE_DIRS 		${ASSIMP_INCLUDE_DIRS}")
message("-- ASSIMP_LIBRARIES 			${ASSIMP_LIBRARIES}")

# if (NOT CMAKE_VERSION VERSION_LESS 3.12.4) # Means greater than or equal
# endif

# DEBUG DEFINES
# Uncoment this to debug
SET(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS}   -DDEBUG_GUI")
# add_compile_definitions(DEBUG_GUI)	

# set(CMAKE_POSITION_INDEPENDENT_CODE on)

set(BUILD_SHARED_LIBS on)

add_subdirectory(extern/box2d)
set_property(TARGET box2d PROPERTY POSITION_INDEPENDENT_CODE ON)
add_subdirectory(extern/glad)
add_subdirectory(extern/soil)
add_subdirectory(extern/sdl2-src)
add_subdirectory(extern/sdl2-mixer)
# set_property(TARGET SDL2 PROPERTY POSITION_INDEPENDENT_CODE ON)
# find_library(SDL2 SDL2)

include_directories(extern/glad/${GLAD_OPENGL_CORE_VERSION})
add_subdirectory(extern/imgui)



if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	find_program(LSB_RELEASE_EXEC lsb_release)
	execute_process(COMMAND ${LSB_RELEASE_EXEC} -is OUTPUT_VARIABLE LSB_RELEASE_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
	message("-- LSB_RELEASE_ID:		${LSB_RELEASE_ID}")

	configure_file(libs/include/config.h.in config.h)

	# list(INSERT CMAKE_MODULE_PATH 0 "extern/sdl2/lib/cmake extern/assimp/lib/cmake")
	link_directories(
		# extern/assimp/lib
		# extern/sdl2/lib
		# extern/soil/lib
		# extern/glad/${GLAD_OPENGL_CORE_VERSION}
	)

	SET(CMAKE_BUILD_TYPE Debug)



	SET(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS}  -gdwarf-2 -fdata-sections -ffunction-sections")
	# SET(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS}  -ftime-report")
	SET(GCC_COVERAGE_LINK_FLAGS    "${GCC_COVERAGE_LINK_FLAGS}       -Wl,--gc-sections")
	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
	SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

	# set(Boost_USE_STATIC_LIBS ON)
	# find_package(Boost REQUIRED COMPONENTS algorithm)




	include_directories(
		# ${SDL2_INCLUDE_DIRS}
		${ASSIMP_INCLUDE_DIRS}
		${CMAKE_BINARY_DIR}
		# extern/assimp/include
		# extern/sdl2-src/include
		# extern/soil/include
		
		# extern
		# extern/imgui
		# extern/imgui/examples
	)
	
	add_subdirectory(libs)

	add_executable(engine 
		src/main.cpp
	)
	target_include_directories(engine PRIVATE libs/src/engine)
	add_executable(tests
		src/main_test.cpp
	)
	
	# if(LSB_RELEASE_ID MATCHES "Arch")
	# 	set(SDL2_LIBRARIES "${SDL2_LIBRARIES} SDL2::SDL2")
	# 	string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
	# 	# message("-- SDL2_LIBRARIES 		${SDL2_LIBRARIES}")
	# endif()z
	
	set(LIBRARIES
		my_libs
		imgui

		#Core libraries for engine
		# libSOIL.a
		soil
		
		SDL2
		SDL2_mixer
		# ${SDL2_LIBRARIES}
		
		# libSDL2d.a
		# libSDL2maind.a
		# libglad.a
		glad

		# assimp
		# libIrrXML.a
		# libzlibstatic.a
		${ASSIMP_LIBRARIES}
		
		box2d
		
		#Very low level
		inotifytools
		GL
		dl
		pthread
		stdc++fs
	)
	

	target_link_libraries(engine 
		${LIBRARIES}
	)
	target_link_libraries(tests
		${LIBRARIES}
	)
	
	
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	
	
endif()


