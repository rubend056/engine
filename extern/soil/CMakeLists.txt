

add_library(soil
    source/src/image_helper.c
    source/src/stb_image_aug.c
    source/src/image_DXT.c
    source/src/SOIL.c
)
target_include_directories(soil PRIVATE
	source/src
)
target_include_directories(soil PUBLIC source/src/SOIL.h)