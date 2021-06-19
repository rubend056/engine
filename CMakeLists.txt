cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
# message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
# message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
# message("CMAKE_CXX_FLAGS_MINSIZEREL is ${CMAKE_CXX_FLAGS_MINSIZEREL}")

include(BuildType.cmake)

project(engine)

message("-- CMAKE_SYSTEM_INFO_FILE: 	${CMAKE_SYSTEM_INFO_FILE}")
message("-- CMAKE_SYSTEM_NAME:      	${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_PROCESSOR: 	${CMAKE_SYSTEM_PROCESSOR}")
message("-- CMAKE_SYSTEM:           	${CMAKE_SYSTEM}")


if(NOT CMAKE_SYSTEM_NAME MATCHES "Android")
	find_package(ASSIMP REQUIRED)
	message("-- ASSIMP_INCLUDE_DIRS 		${ASSIMP_INCLUDE_DIRS}")
	message("-- ASSIMP_LIBRARIES 			${ASSIMP_LIBRARIES}")

	set(BUILD_SHARED_LIBS on)
	# set(CMAKE_POSITION_INDEPENDENT_CODE on)

	add_subdirectory(extern/soil)
endif()

# DEBUG DEFINES
SET(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS}   -DDEBUG_GUI")


add_subdirectory(extern/box2d)
add_subdirectory(extern/glad)
add_subdirectory(extern/sdl2-src)
add_subdirectory(extern/sdl2-mixer)
add_subdirectory(extern/imgui)




if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	find_program(LSB_RELEASE_EXEC lsb_release)
	execute_process(COMMAND ${LSB_RELEASE_EXEC} -is OUTPUT_VARIABLE LSB_RELEASE_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
	message("-- LSB_RELEASE_ID:		${LSB_RELEASE_ID}")

	configure_file(src/libs/include/config.h.in config.h)

	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

	SET(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS}  -gdwarf-2 -fdata-sections -ffunction-sections")
	# SET(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS}  -ftime-report")
	SET(GCC_COVERAGE_LINK_FLAGS    "${GCC_COVERAGE_LINK_FLAGS}       -Wl,--gc-sections")
	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
	SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

	

	include_directories(
		${ASSIMP_INCLUDE_DIRS}
		${CMAKE_BINARY_DIR}
	)
	
	
	add_subdirectory(src/libs)
	
	SET(ENGINE_NAME engine)
	SET(TEST_NAME engine_test)
	SET(TARGETS ${ENGINE_NAME} ${TEST_NAME})

	# SET Executables
	add_executable(${ENGINE_NAME} 
		src/main.cpp
	)
	target_include_directories(${ENGINE_NAME} PRIVATE src/libs/src/engine)
	add_executable(${TEST_NAME}
		src/main_test.cpp
	)
	target_include_directories(${TEST_NAME} PRIVATE extern)
	
	set(LIBRARIES
		my_libs
		imgui

		#Core libraries for engine
		soil
		
		SDL2
		SDL2_mixer
		
		glad

		${ASSIMP_LIBRARIES}
		
		box2d
		
		#Very low level
		inotifytools
		GL
		dl
		pthread
		
		stdc++fs.a
	)
	
	foreach(T IN LISTS TARGETS)
		target_link_libraries(${T} 
			${LIBRARIES}
		)
	endforeach()
	# link_libraries(${LIBRARIES})
	
	
elseif(CMAKE_SYSTEM_NAME MATCHES "Android")
	# armeabi-v7a requires cpufeatures library
	# include(AndroidNdkModules)
	# android_ndk_import_module_cpufeatures()

	# message("-- CMAKE_SYSTEM_INFO_FILE: 	${CMAKE_SYSTEM_INFO_FILE}")
	# message("-- CMAKE_SYSTEM_NAME:      	${CMAKE_SYSTEM_NAME}")
	# message("-- CMAKE_SYSTEM_PROCESSOR: 	${CMAKE_SYSTEM_PROCESSOR}")
	# message("-- CMAKE_SYSTEM:           	${CMAKE_SYSTEM}")


	# SDL sources are in a subfolder named "SDL"
	# add_subdirectory(box2d)
	# add_subdirectory(glad)
	# add_subdirectory(soil)
	# add_subdirectory(sdl2-src)
	# add_subdirectory(sdl2-mixer)
	# add_subdirectory(imgui)



	# Compilation of companion libraries
	#add_subdirectory(SDL_image)
	#add_subdirectory(SDL_mixer)
	#add_subdirectory(SDL_ttf)

	# Your game and its CMakeLists.txt are in a subfolder named "src"
	add_subdirectory(src)
endif()


