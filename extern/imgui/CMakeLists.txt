
add_library(imgui 
	imgui.cpp
	imgui_draw.cpp
	imgui_widgets.cpp
	examples/imgui_impl_opengl3.cpp 
	examples/imgui_impl_sdl.cpp
	misc/cpp/imgui_stdlib.cpp
)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/examples)
target_link_libraries(imgui glad SDL2)
set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)