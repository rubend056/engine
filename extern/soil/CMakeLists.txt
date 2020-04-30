

include_directories(
    source/src
)
add_library(soil
    source/src/image_helper.c
    source/src/stb_image_aug.c
    source/src/image_DXT.c
    source/src/SOIL.c
)