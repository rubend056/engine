
# message("-- SOIL CMAKE_SYSTEM_NAME:      	${CMAKE_SYSTEM_NAME}")
# if(CMAKE_SYSTEM_NAME MATCHES "Android")
# 	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -DSOIL_ANDROID")
# endif()

add_library(soil
    source/src/image_helper.c
    source/src/stb_image_aug.c
    source/src/image_DXT.c
    source/src/SOIL.c
)
target_include_directories(soil PRIVATE
	source/src
)

# target_link_libraries(soil GL EGL)
target_include_directories(soil PUBLIC include)