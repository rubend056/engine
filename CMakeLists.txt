cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (NOT CMAKE_VERSION VERSION_LESS 3.12.4) # Means greater than or equal
	# Uncoment this to debug
	add_compile_definitions(DEBUG_GUI)	
endif()

# list(INSERT CMAKE_MODULE_PATH 0 "extern/sdl2/lib/cmake extern/assimp/lib/cmake")
link_directories(
	extern/assimp/lib
	extern/sdl2/lib
	extern/soil/lib
	extern/glad
)

SET(CMAKE_BUILD_TYPE Debug)

# message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
# message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
# message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
# message("CMAKE_CXX_FLAGS_MINSIZEREL is ${CMAKE_CXX_FLAGS_MINSIZEREL}")

SET(GCC_COVERAGE_COMPILE_FLAGS "")
SET(GCC_COVERAGE_LINK_FLAGS    "")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# set(Boost_USE_STATIC_LIBS ON)
# find_package(Boost REQUIRED COMPONENTS algorithm)


project(engine)

include_directories(
	extern/assimp/include
	extern/sdl2/include/SDL2
	extern/soil/include
	
	extern
	extern/imgui
	extern/imgui/examples
	
	libs/apps
	libs/include
	libs/include/assets
	libs/include/components
	libs/include/groups
	libs/include/helper
	libs/include/menus
	libs/include/rendering
)

add_subdirectory(extern/imgui)
add_subdirectory(libs)


add_executable(engine 
	libs/apps/main.cpp
	libs/apps/engine.cpp
)

target_link_libraries(engine
	# assets
	# rendering
	# menus
	# components 
	my_libs
	
	imgui
	
	#Core libraries for engine
	libSOIL.a
	libSDL2d.a
	libSDL2maind.a
	libglad.a
	
	assimp
	libIrrXML.a
	libzlibstatic.a
	
	#Very low level
	inotifytools
	GL
	dl
	pthread
	stdc++fs
)

