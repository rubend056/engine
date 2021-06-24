

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

find_library(imgui imgui)
find_library(soil soil)

add_library(my_libs
	src/engine/engine.cpp

    src/assets/assets.cpp
    # src/assets/assets_import.cpp
	src/assets/gameobject.cpp
	src/assets/scene.cpp
	
	src/helper/type_name.cpp
	src/helper/inotify_imp.cpp
	src/helper/file.cpp
	src/helper/component.cpp
	src/helper/referentiable.cpp

	src/menus/inspector.cpp
	src/menus/files.cpp
	src/menus/text_editor.cpp
	src/menus/stats.cpp
	src/menus/menus.cpp
	src/menus/menus_gl.cpp
	
	src/components/camera.cpp
	src/components/transform.cpp
	src/components/source.cpp
	src/components/audio.c
	
	src/rendering/mesh.cpp
	src/rendering/texture.cpp
	src/rendering/program.cpp
	src/rendering/shader.cpp
	src/rendering/vao.cpp
	src/rendering/attribute.cpp
)
target_link_libraries(my_libs imgui soil box2d SDL2 assimp stdc++fs)
target_include_directories(my_libs PRIVATE
	# src/rendering
)
target_include_directories(my_libs PUBLIC
	include
	include/assets
	include/components
	include/groups
	include/helper
	include/menus
	include/rendering
)

# if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.16)
# 	target_precompile_headers(my_libs PRIVATE 
# 		include/engine_globals.h
# 		include/assets/assets.h
# 		src/assets/_assets.h
# 	)
# endif()