find_package(SDL2 REQUIRED)
add_library(imgui STATIC 
	imgui/imgui.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_widgets.cpp
	imgui/examples/imgui_impl_opengl3.cpp 
	imgui/examples/imgui_impl_sdl.cpp
)
target_link_libraries(imgui SDL2::SDL2)