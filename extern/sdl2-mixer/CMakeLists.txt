CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(SDL2_mixer VERSION 2.0.4)

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# find_library(SDL SDL)

set(SRC_C_FILES
    dynamic_flac.c
    dynamic_fluidsynth.c
    dynamic_mod.c
    # dynamic_modplug.c
    dynamic_mp3.c
    dynamic_ogg.c
    effect_position.c
    effect_stereoreverse.c
    effects_internal.c
    fluidsynth.c
    load_aiff.c
    load_flac.c
    # load_mp3.c
    load_ogg.c
    load_voc.c
    mixer.c
    music_cmd.c
    music_flac.c
    music_mad.c
    music_mod.c
    music_modplug.c
	music_ogg.c
	music.c
	# playmus.c
	# playwave.c
    wavestream.c
)

set(SRC_H_FILES
	dynamic_flac.h
	dynamic_fluidsynth.h
	dynamic_mod.h
	# dynamic_modplug.h
	dynamic_mp3.h
	dynamic_ogg.h
	# effect_position.h
	# effect_stereoreverse.h
	effects_internal.h
	fluidsynth.h
	load_aiff.h
	load_flac.h
	# load_mp3.h
	load_ogg.h
	load_voc.h
	# mixer.h
	music_cmd.h
	music_flac.h
	music_mad.h
	music_mod.h
	music_modplug.h
	music_ogg.h
	# music.h
	# playmus.h
	# playwave.h
	wavestream.h
)

set(SRC_FILES ${SRC_C_FILES} ${SRC_H_FILES})
add_library(SDL2_mixer ${SRC_FILES})

target_link_libraries(SDL2_mixer SDL2)
target_include_directories(SDL2_mixer PUBLIC include)
