

# add_library(assets STATIC 
# 	src/inotify_imp.cpp
#     src/assets.cpp
#     src/assets_import.cpp
#     src/assets_inotify.cpp
# )

# add_library(helper STATIC
# 	src/type_name.cpp
# 	src/serialize.cpp
# )

# add_executable(testing src/serialize2.cpp)

# add_library(menus STATIC 
#     src/menus/inspector.cpp
#     src/menus/files.cpp
#     src/menus/text_editor.cpp
#     src/menus/stats.cpp
#     src/menus/imgui_engine.cpp
# )
# target_link_libraries(menus imgui assets helper)

# add_library(components STATIC 
#     src/gameobject.cpp
# 	src/components/camera.cpp
# 	src/components/transform.cpp
# )

# add_library(rendering STATIC
# 	src/rendering/mesh.cpp
# 	src/rendering/texture.cpp
# 	src/rendering/program.cpp
# )
# target_link_libraries(rendering menus imgui)

add_library(my_libs
	src/inotify_imp.cpp
	
    src/assets/assets.cpp
    src/assets/assets_import.cpp
	src/assets/gameobject.cpp
	src/assets/scene.cpp
	
	src/helper/type_name.cpp
	src/helper/file.cpp

	src/menus/inspector.cpp
	src/menus/files.cpp
	src/menus/text_editor.cpp
	src/menus/stats.cpp
	src/menus/menus.cpp
	src/menus/menus_gl.cpp
	
	src/components/camera.cpp
	src/components/transform.cpp
	src/components/source.cpp
	
	src/rendering/mesh.cpp
	src/rendering/texture.cpp
	src/rendering/program.cpp
)
target_link_libraries(my_libs imgui)