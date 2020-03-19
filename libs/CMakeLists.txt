add_library(glad STATIC src/glad.c)

add_library(assets STATIC 
	src/inotify_imp.cpp
    src/assets.cpp
    src/assets_import.cpp
    src/assets_inotify.cpp
)

add_library(menus STATIC 
    src/menus/inspector.cpp
    src/menus/files.cpp
    src/menus/text_editor.cpp
    src/menus/stats.cpp
    src/menus/imgui_engine.cpp
)
target_link_libraries(menus imgui assets)

add_library(components STATIC 
    src/gameobject.cpp
	src/components/camera.cpp
	
)

add_library(rendering STATIC
    src/rendering/gl_helper.cpp
)