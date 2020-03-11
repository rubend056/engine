cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(engine)

# find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(ASSIMP REQUIRED)

include_directories(
	${ASSIMP_INCLUDE_DIR}
	libs/include/
	extern/imgui
	extern/imgui/examples

	apps
	libs/include
	libs/include/menus
)

add_subdirectory(apps)
add_subdirectory(extern)
add_subdirectory(libs)


add_executable(engine apps/engine.cpp)

target_link_libraries(engine 
	${ASSIMP_LIBRARY}
	SDL2::SDL2 
	
	imgui 
	glad 
	menus 
	components 
)
