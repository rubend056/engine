add_library(glad STATIC src/glad.c)

add_library(menus STATIC 
    src/menus/inspector.cpp
    src/menus/files.cpp
    src/menus/text_editor.cpp
)
target_link_libraries(menus imgui)

add_library(components STATIC 
    src/gameobject.cpp
    src/components/camera.cpp
)