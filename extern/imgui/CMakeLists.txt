# cmake_minimum_required(VERSION 3.10)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(SDL2 SDL2)

include_directories(
	./
	./examples
)
add_library(imgui STATIC 
	imgui.cpp
	imgui_draw.cpp
	imgui_widgets.cpp
	examples/imgui_impl_opengl3.cpp 
	examples/imgui_impl_sdl.cpp
	misc/cpp/imgui_stdlib.cpp
)
target_link_libraries(imgui SDL2)