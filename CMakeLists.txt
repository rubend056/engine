cmake_minimum_required(VERSION 3.7)

project(engine)

include_directories(
	apps
	libs/include/
	extern/imgui
)

add_subdirectory(apps)
add_subdirectory(libs)
add_subdirectory(extern)

# find_package(GLEW REQUIRED)

find_package(SDL2 REQUIRED)
# message(“SDL2 dir: ${SDL2_DIR}”)

add_executable(engine apps/engine.cpp)
set_property(TARGET engine PROPERTY CXX_STANDARD 11)
target_link_options(engine PRIVATE "-lassimp")
target_link_libraries(engine imgui glad SDL2::SDL2)
